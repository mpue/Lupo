#ifndef STK_BLITSQUARE_H
#define STK_BLITSQUARE_H

#include "Generator.h"
#include <cmath>
#include <limits>

namespace stk {

/***************************************************/
/*! \class BlitSquare
    \brief STK band-limited square wave class.

    This class generates a band-limited square wave signal.  It is
    derived in part from the approach reported by Stilson and Smith in
    "Alias-Free Digital Synthesis of Classic Analog Waveforms", 1996.
    The algorithm implemented in this class uses a SincM function with
    an even M value to achieve a bipolar bandlimited impulse train.
    This signal is then integrated to achieve a square waveform.  The
    integration process has an associated DC offset so a DC blocking
    filter is applied at the output.

    The user can specify both the fundamental frequency of the
    waveform and the number of harmonics contained in the resulting
    signal.

    If nHarmonics is 0, then the signal will contain all harmonics up
    to half the sample rate.  Note, however, that this setting may
    produce aliasing in the signal when the frequency is changing (no
    automatic modification of the number of harmonics is performed by
    the setFrequency() function).  Also note that the harmonics of a
    square wave fall at odd integer multiples of the fundamental, so
    aliasing will happen with a lower fundamental than with the other
    Blit waveforms.  This class is not guaranteed to be well behaved
    in the presence of significant aliasing.

    Based on initial code of Robin Davies, 2005.
    Modified algorithm code by Gary Scavone, 2005--2006.
*/
/***************************************************/

class BlitSquare: public Generator
{
 public:
  //! Default constructor that initializes BLIT frequency to 220 Hz.
  BlitSquare( StkFloat frequency = 220.0 );

  //! Class destructor.
  ~BlitSquare();

  //! Resets the oscillator state and phase to 0.
  void reset();

  //! Set the phase of the signal.
  /*!
    Set the phase of the signal, in the range 0 to 1.
  */
  void setPhase( StkFloat phase ) { phase_ = PI * phase; };

  //! Get the current phase of the signal.
  /*!
    Get the phase of the signal, in the range [0 to 1.0).
  */
  StkFloat getPhase() const { return phase_ / PI; };

  //! Set the impulse train rate in terms of a frequency in Hz.
  void setFrequency( StkFloat frequency );

  //! Set the number of harmonics generated in the signal.
  /*!
    This function sets the number of harmonics contained in the
    resulting signal.  It is equivalent to (2 * M) + 1 in the BLIT
    algorithm.  The default value of 0 sets the algorithm for maximum
    harmonic content (harmonics up to half the sample rate).  This
    parameter is not checked against the current sample rate and
    fundamental frequency.  Thus, aliasing can result if one or more
    harmonics for a given fundamental frequency exceeds fs / 2.  This
    behavior was chosen over the potentially more problematic solution
    of automatically modifying the M parameter, which can produce
    audible clicks in the signal.
  */
  void setHarmonics( unsigned int nHarmonics = 0 );

  //! Set the pulse width (duty cycle) of the square wave.
  /*!
    Set the pulse width as a value between 0.0 and 1.0, where 0.5
    represents a standard square wave (50% duty cycle). Values closer
    to 0.0 or 1.0 will produce narrower pulses.
  */
  void setPulseWidth( StkFloat width );

  //! Get the current pulse width.
  StkFloat getPulseWidth() const { return pulseWidth_; };

  //! Return the last computed output value.
  StkFloat lastOut( void ) const { return lastFrame_[0]; };

  //! Compute and return one output sample.
  StkFloat tick( void );

  //! Fill a channel of the StkFrames object with computed outputs.
  /*!
    The \c channel argument must be less than the number of
    channels in the StkFrames argument (the first channel is specified
    by 0).  However, range checking is only performed if _STK_DEBUG_
    is defined during compilation, in which case an out-of-range value
    will trigger an StkError exception.
  */
  StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );

 protected:

  void updateHarmonics( void );

  unsigned int nHarmonics_;
  unsigned int m_;
  StkFloat rate_;
  StkFloat phase_;
  StkFloat p_;
  StkFloat a_;
  StkFloat lastBlitOutput_;
  StkFloat dcbState_;
  StkFloat pulseWidth_;  // Pulse width (duty cycle) 0.0 to 1.0
  StkFloat lastPulseOutput_;  // For pulse width filtering
};

inline StkFloat BlitSquare :: tick( void )
{
  // Simple pulse wave implementation with pulse width modulation
  StkFloat normalizedPhase = phase_ / TWO_PI;
  StkFloat output;
  
  // Generate the basic pulse wave
  if (normalizedPhase < pulseWidth_) {
    output = 1.0;
  } else {
    output = -1.0;
  }
  
  // Apply a simple one-pole low-pass filter to reduce aliasing
  // This helps make the pulse wave more band-limited
  StkFloat alpha = 0.1f;  // Filter coefficient (higher = more filtering)
  StkFloat filteredOutput = alpha * output + (1.0f - alpha) * lastPulseOutput_;
  lastPulseOutput_ = filteredOutput;

  // Apply DC blocker to remove any DC offset
  lastFrame_[0] = filteredOutput - dcbState_ + 0.999f * lastFrame_[0];
  dcbState_ = filteredOutput;

  // Update phase
  phase_ += rate_;
  if ( phase_ >= TWO_PI ) phase_ -= TWO_PI;

  return lastFrame_[0];
}

inline StkFrames& BlitSquare :: tick( StkFrames& frames, unsigned int channel )
{
#if defined(_STK_DEBUG_)
  if ( channel >= frames.channels() ) {
    oStream_ << "BlitSquare::tick(): channel and StkFrames arguments are incompatible!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }
#endif

  StkFloat *samples = &frames[channel];
  unsigned int hop = frames.channels();
  for ( unsigned int i=0; i<frames.frames(); i++, samples += hop )
    *samples = BlitSquare::tick();

  return frames;
}

} // stk namespace

#endif
