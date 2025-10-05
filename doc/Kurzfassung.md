# Developing a Software Synthesizer

## A Comprehensive Engineering & DSP Study

*Prepared by ChatGPT | OpenAI o3 | 28 June 2025*

---

## Abstract

This study paper provides a **deep‑dive, end‑to‑end blueprint** for designing and implementing a modern, polyphonic software synthesizer. It covers the **physical and psychoacoustic foundations** of sound, the **digital‑signal‑processing (DSP) theory** that underpins waveform generation and filtering, and the **software‑engineering practices** required to deliver a reliable, performant plug‑in or standalone instrument.  Each major building block—oscillators, envelopes, filters, modulators, effects, polyphony, and hosting—is examined in detail, with **annotated C++/JUCE code examples**, design trade‑offs, and references to seminal literature.

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [Acoustic & Psychoacoustic Foundations](#2-acoustic--psychoacoustic-foundations)
3. [Digital Audio Fundamentals](#3-digital-audio-fundamentals)
4. [Subtractive Synthesis Architecture](#4-subtractive-synthesis-architecture)
5. [Digital Oscillator Design](#5-digital-oscillator-design)
6. [Band‑Limited Techniques (BLIT/BLEP/BLEP‑AD)](#6-band-limited-techniques--blitblepblep--ad)
7. [Additive & Fourier‑Domain Synthesis](#7-additive--fourier-domain-synthesis)
8. [Digital Filters](#8-digital-filters)
9. [Envelope Generators & LFOs](#9-envelope-generators--lfos)
10. [Modulation Matrix & Macro Controls](#10-modulation-matrix--macro-controls)
11. [Polyphony, Voice Allocation & MIDI 2.0](#11-polyphony-voice-allocation--midi-20)
12. [Effects Processing](#12-effects-processing)
13. [System Architecture & Real‑Time Threading](#13-system-architecture--real-time-threading)
14. [Performance Optimisation](#14-performance-optimisation)
15. [Testing, Validation & Tooling](#15-testing-validation--tooling)
16. [Conclusion](#16-conclusion)
17. [Appendices](#17-appendices)
18. [References](#18-references)

*Click any heading to jump directly to that section.*

---

## 1. Introduction

The history of electronic sound synthesis stretches from the early **Theremin (1920s)** through **modular analogue systems (1960s–70s)** to today’s deeply‑integrated **software instruments**.  Modern soft‑synths must deliver **alias‑free audio**, **sample‑accurate modulation**, and **DAW‑friendly automation**, all while running on commodity CPUs.  This paper follows a *bottom‑up* methodology—beginning with physics, moving through discrete‑time DSP theory, and culminating in object‑oriented C++ designs suitable for JUCE, VST3, AU, and CLAP plug‑in targets.

### 1.1 Scope & Goals

* Provide sufficient theory for graduate‑level DSP students and practising engineers.
* Offer practical C++ snippets that compile under **JUCE 7.x** with minimal dependencies.
* Highlight **performance pitfalls** and **real‑time safety** concerns.
* Present **design patterns** (Factory, Visitor, Graph) useful in large audio codebases.

### 1.2 Prerequisites

* Intermediate C++20 knowledge (templates, move semantics).
* Familiarity with linear‐time‑invariant (LTI) systems and z‑transform.
* Basic DAW/MIDI workflow understanding.

---

## 2. Acoustic & Psychoacoustic Foundations

### 2.1 Physics of Sound

Sound in air is a **longitudinal pressure wave** described by the **1‑D wave equation**: $\partial^2 p / \partial t^2 = c^2 \partial^2 p / \partial x^2$.  *Key variables:*

* **Frequency $f$** (Hz) – perceived as pitch.
* **Amplitude $A$** (Pa or dB SPL) – perceived as loudness.
* **Phase $\varphi$** (rad) – crucial for interference & localisation.
* **Harmonic series** – integer multiples of the fundamental in periodic tones.

### 2.2 Psychoacoustics

* **Equal‑loudness contours** (ISO 226) show our ear’s reduced sensitivity at low & very high frequencies.
* **Critical bands / Bark scale** – informs filter bank design for perceptual loudness & masking.
* **Masking** – louder sounds hide quieter ones; leveraged in dither & lossy codecs.

### 2.3 Timbre Dimensions

Spectral centroid (brightness), inharmonicity, envelope shape, and modulation depth shape perceived timbre.  These parameters guide oscillator & filter choices.

---

## 3. Digital Audio Fundamentals

### 3.1 Sampling Theory & Anti‑Aliasing

The **Nyquist‑Shannon theorem** demands $F_s > 2F_{max}$.  Practical converters employ *oversampling* and *half‑band FIR* decimators.  For soft‑synths, alias‑free generation often requires either **band‑limited synthesis** or **oversampling the whole engine** (2–8×) followed by polyphase filters.

### 3.2 Quantisation, Dither & Noise Shaping

16‑bit audio has \~96 dB theoretical SNR.  Adding **TPDF dither** decorrelates quantisation noise; **noise shaping** pushes energy above 15 kHz to exploit ear masking.  Floating‑point (32F or 64F) inside the DSP chain eliminates most headroom concerns but can suffer from **denormals**, fixed with `std::fesetenv(FE_DFL_DISABLE_DENORMS)` or FPU flags.

### 3.3 Fixed‑ vs Floating‑Point Trade‑offs

Fixed‑point arithmetic (e.g. Q1.31) shines on microcontrollers (ARM M‑series) but demands manual saturation & rounding.  Desktop soft‑synths overwhelmingly use **SIMD floating‑point** (SSE, AVX, NEON), enabling vectorised oscillators and filters.

---

## 4. Subtractive Synthesis Architecture

### 4.1 Historical Context

Pioneered by Bob Moog & Don Buchla, subtractive synthesis popularised voltage‑controlled oscillators (VCOs), voltage‑controlled filters (VCFs), and envelope generators (EGs).  Modern soft‑synths preserve this flow but add extended modulation and polyphony.

### 4.2 Core Block Diagram

```
[MIDI] ─▶ [Voice Allocator] ─▶ (N Voices)
                      │
               ┌──────┴──────┐
               │   VOICE_i   │ (polyphonic)
               │             │
               │  [Osc Bank] ──▶ [Mixer] ──▶ [VCF] ──▶ [VCA] ─▶ Σ ─▶ [FX] ─▶ Audio Out
               │        ▲          ▲         ▲          │
               │   [LFOs]     [Env]      [Amp Env]      │
               └──────────────┴─────────────────────────┘
```

*Envelope retrigger mode (per‑voice) vs global.*

### 4.3 Oscillator Mixer Strategies

* **Pre‑filter mix** (default).  Avoids aliasing amplification by the VCF.
* **Post‑filter mix** (useful for noise layering).
* **Cross‑modulation bus** for ring‑mod, FM.

### 4.4 Filter Drive & Saturation Path

Analogue synths distort *before* the filter (Minimoog) or *in* the ladder (MS‑20).  Software can emulate this with **tanh‑based waveshaping** fed into zero‑delay feedback filters (Section 8.4).

---

## 5. Digital Oscillator Design

Modern oscillators must be **band‑limited, pitch‑stable, and cheap**.  We examine three families:

### 5.1 Phase‑Accumulator Sine

With linear‑interpolated 16‑k sample wavetable the error < ‑120 dB.  Use **polyphase fixed‑point resampling** for memory savings.

```cpp
class LWTSine final {
public:
    void prepare(double sampleRate) {
        phaseInc = tableSize * frequency / sampleRate;
    }
    float process() {
        auto idx = static_cast<uint32_t>(phase);
        float frac = phase - idx;
        float y0 = table[idx];
        float y1 = table[(idx+1) & (tableSize-1)];
        phase = std::fmod(phase + phaseInc, tableSize);
        return std::lerp(y0, y1, frac);
    }
private:
    static constexpr uint32_t tableSize = 16384;
    std::array<float, tableSize> table { /* pre‑computed */ };
    double phase{}, phaseInc{};
};
```

### 5.2 Naïve Saw & Square (for illustration)

Generated by sign/stepped waveforms—*never* use in production above \~1 kHz.  Alias energy increases \~6 dB per octave.

### 5.3 Table‑Lookup Oscillators (TLO)

Prepare **harmonic‑number‑limited tables** per octave (or per semitone) – common in wavetable synths such as *Serum*.

### 5.4 Phase‑Distortion & Waveshaping

Casio CZ‑series style: distort phase accumulator before a sine lookup.  Cheap but powerful timbres.  Integrates with BLEP for anti‑alias.

---

## 6. Band‑Limited Techniques (BLIT/BLEP/BLEP‑AD)

### 6.1 BLIT – Analytic Formula

A BLIT is an impulse train with spectral sinc envelope limiting partials $\le N$.  Discrete form:
$\text{BLIT}(n) = \frac{\sin\big(\pi N \tfrac{n}{N_s}\big)}{N\,\sin\big(\pi \tfrac{n}{N_s}\big)}$
Convolving / integrating this train yields band‑limited saw/square, but integration adds DC and modal instability—hence **BLEP**.

### 6.2 PolyBLEP (Polynomial BLEP)

Adds a *correction polynomial* across discontinuities.  Complexity O(1) per discontinuity.

#### Implementation Tips

* Compute **dt = phaseInc** in fractional phase units.
* Apply polyBLEP at *each* edge of discontinuity (saw: one, square: two).
* For triangle, integrate a band‑limited square to reduce aliasing further.

#### Example Implementation


Convolving / integrating this train yields band‑limited saw/square, but integration adds DC and modal instability — hence **BLEP**.

---

### 6.1a BLIT Oscillator in C++

A **Band-Limited Impulse Train (BLIT)** oscillator can be implemented efficiently in modern C++ using leaky integration to produce alias-free waveforms. The following code illustrates how to create a basic but powerful BLIT oscillator.

#### 🔧 Features:
- Efficient O(1) BLIT impulse generation
- Leaky integration for DC-reduced **Saw**
- Support for **Square** and **Triangle**
- Optional DC compensation
- Clean, modern C++17 interface

---

### 🔢 Core Equations

- `BLIT(t) = sin(πN·f·t) / (N·sin(π·f·t))`  
- Discrete: `BLIT[n] = sin(πN·n/Ns) / (N·sin(π·n/Ns))`
- Number of harmonics: `N = floor(fs / (2·f))`

---

### 🧠 Implementation (Header-only)


Convolving / integrating this train yields band‑limited saw/square, but integration adds DC and modal instability — hence **BLEP**.

---

### 6.1a BLIT Oscillator in C++

A **Band-Limited Impulse Train (BLIT)** oscillator can be implemented efficiently in modern C++ using leaky integration to produce alias-free waveforms. The following code illustrates how to create a basic but powerful BLIT oscillator.

#### 🔧 Features:
- Efficient O(1) BLIT impulse generation
- Leaky integration for DC-reduced **Saw**
- Support for **Square** and **Triangle**
- Optional DC compensation
- Clean, modern C++17 interface

---

### 🔢 Core Equations

- `BLIT(t) = sin(πN·f·t) / (N·sin(π·f·t))`  
- Discrete: `BLIT[n] = sin(πN·n/Ns) / (N·sin(π·n/Ns))`
- Number of harmonics: `N = floor(fs / (2·f))`

---

### 🧠 Implementation (Header-only)

```cpp
#pragma once
#include <cmath>
#include <algorithm>

constexpr double PI      = 3.14159265358979323846;
constexpr double TWO_PI  = 6.28318530717958647692;

class BLITOscillator
{
public:
    enum class Waveform { Saw, Square, Triangle };

    void prepare(double sampleRate, Waveform wf = Waveform::Saw)
    {
        fs        = sampleRate;
        invFs     = 1.0 / fs;
        setWaveform(wf);
        reset();
    }

    void setFrequency(double freqHz)
    {
        freq      = std::clamp(freqHz, 1.0, 0.49 * fs);
        phaseInc  = freq * invFs;
        N         = static_cast<int>(fs * 0.5 / freq);
        norm      = 1.0 / static_cast<double>(N);
        dcCorrect = (waveform != Waveform::Saw && waveform != Waveform::Triangle) ? 0.0 : 0.5;
    }

    void setWaveform(Waveform wf) { waveform = wf; }
    void reset() { phase = 0.0; integ = 0.0; }

    template<typename T = double>
    inline T process()
    {
        double t   = phase - 0.5;
        double num = std::sin(PI * N * t);
        double den = std::sin(PI * t);
        double blit= (den != 0.0 ? (num / den) * norm : 1.0);

        double y = 0.0;
        switch (waveform)
        {
            case Waveform::Saw:
                integ = LEAK * integ + blit;
                y     = integ - dcCorrect;
                break;

            case Waveform::Square:
                {
                    double sawA = integStoreA;
                    integStoreA = LEAK * integStoreA + blit;
                    y = (integStoreA - dcCorrect) - (sawA - dcCorrect);
                }
                break;

            case Waveform::Triangle:
                {
                    double square = processInternalSquare(blit);
                    integ = LEAK * integ + square;
                    y     = integ - triDc;
                }
                break;
        }

        phase += phaseInc;
        if (phase >= 1.0) phase -= 1.0;

        return static_cast<T>(y);
    }

private:
    double processInternalSquare(double blit)
    {
        integStoreA = LEAK * integStoreA + blit;
        double square = (integStoreA - dcCorrect) - (prevSaw - dcCorrect);
        prevSaw = integStoreA;
        return square;
    }

    double fs = 44100.0, invFs = 1.0 / 44100.0;
    double freq = 440.0, phaseInc = 440.0 / 44100.0;
    double phase = 0.0;
    int    N = 50;
    double norm = 1.0 / 50.0;
    double integ = 0.0;
    double integStoreA = 0.0, prevSaw = 0.0;
    double dcCorrect = 0.0;
    double triDc = 0.0;
    Waveform waveform = Waveform::Saw;

    const double LEAK = 0.999;
};
```

Here's an example on how to use

```cpp
BLITOscillator osc;
osc.prepare(48000.0, BLITOscillator::Waveform::Saw);
osc.setFrequency(110.0);

for (int n = 0; n < numSamples; ++n)
    buffer[n] = osc.process<float>();
```





### 6.3 BLEP‑AD (Antiderivative Antialiased)

Replace step discontinuity with pre‑integrated **ADA kernel**.  Superior for hard‑clipped oscillators & PWM.  See Välimäki (2023).

---

## 7. Additive & Fourier‑Domain Synthesis

### 7.1 Classic Additive Engine

**N = 128 partials**, each with independent **AD envelopes** and **phase accumulation**.  CPU ≈ *N × 6 mul/add* per sample—okay with AVX512, heavy on mobile.

### 7.2 FFT/IFFT Resynthesis

*Frame size = 2048, hop = 256*.  Magnitude/phase stored per frame; overlap‑add inverse FFT reconstructs.  Enables **spectral morphing** & **formant shifting**.

### 7.3 Sparse Additive (Synchronous Overlap‑Add)

Track only peaks above ‑70 dB; reduces oscillator count dramatically.  Good for *organ* and *vocal* modelling.

---

## 8. Digital Filters

### 8.1 FIR vs IIR Trade‑offs

*FIR*: linear phase, large taps, perfect DC behaviour.  *IIR*: low order, low CPU, but nonlinear phase and potential instability.

### 8.2 Biquad Cookbook

Robert Bristow‑Johnson formulas (1999) remain standard.  Optimise by **transposing** to `DirectFormI` for better SIMD locality.

### 8.3 Zero‑Delay‑Feedback (ZDF) SVF

Solves implicit equation per sample with **Newton–Raphson** or algebraic re‑arrangement.  Enables *Moog ladder clones* with accurate resonance up to self‑oscillation.

```cpp
struct ZDFSVF {
    void setCutoff(float f) { g = std::tan(PI * f / Fs); }
    float process(float x) noexcept {
        float v1 = g * (x - z2) + z1;        // high‑pass in v1
        float v2 = g * v1 + z2;              // band‑pass in v1, low‑pass in v2
        z1 = g * v1 + v1;
        z2 = g * v2 + v2;
        return v2;                           // low‑pass
    }
    float g{}, z1{}, z2{};
};
```

### 8.4 Nonlinear & Analogue Modelling

*Moog ladder*, *Korg MS‑20*, *Roland OTA* can be modelled with **tanh waveshaping in feedback** plus accurate pole positioning.  Use **TPT (Topology‑Preserving Transform)** for stability.

---

## 9. Envelope Generators & LFOs

### 9.1 ADSR Exponential Curves

Coefficient for target *T* in *N* samples: $c = e^{\ln(1-T)/N}$.  Store per‑stage coefficient for sample‑accurate smoothing.

### 9.2 Multi‑Stage & Looping Envelopes

Use vector of `{level, time, curve}` tuples.  During runtime, pre‑compute **per‑sample increment** to avoid branching.

### 9.3 Sample‑Accurate LFO

LFOs up to 20 Hz can be scalar; above that (audio‑rate FM) must share oscillator technology with Section 5.

### 9.4 Modulation Oscillators

Expose **phase‑reset** on MIDI NoteOn for per‑voice vibrato, or run in free‑phase for vintage feel.

---

## 10. Modulation Matrix & Macro Controls

### 10.1 Data Structures

```cpp
struct ModSlot {
    const float* src;   // pointer to modulation source buffer
    float*       dst;   // pointer to destination parameter
    float        depth; // bipolar depth (‑1…+1)
};
std::vector<ModSlot> matrix;
```

Process **once per audio block** to avoid per‑sample costs when destinations are *control‑rate* (cutoff freq < 20 kHz).  Provide **slew limiters** (1‑pole low‑pass) to suppress zipper noise for fast UI automation.

### 10.2 Macro Controls

Expose up to eight *knobs* that scale groups of ModSlots.  Saves users from assigning dozens of routings.

---

## 11. Polyphony, Voice Allocation & MIDI 2.0

### 11.1 Voice Object

Encapsulates **OscBank, Filter, AmpEnv, ModState**.  Use *SoA* layout for SIMD (e.g. process four voices in lockstep).

### 11.2 Allocation Algorithms

1. **Steal Oldest** – safest for pads.
2. **Steal Quietest** – good for percussive.
3. **Reassign Same Note** – needed for MIDI legato.

### 11.3 MIDI 2.0 Per‑Note Controllers

Each note has its own 32‑bit controller namespace.  Requires **per‑voice CC state** arrays; memory ≈ (voices × controllers).

---

## 12. Effects Processing

### 12.1 Delay & Chorus

*Fractional delay lines* via **4‑point Lagrange** or **cubic Hermite** interpolation.  Chorus = multi‑tap delay with slow LFO modulation.

### 12.2 Algorithmic Reverb

*Schroeder/Moorer* topology: 4× comb → 2× all‑pass → EQ.  For better tails, use **8‑ch Feedback Delay Network** with Householder feedback matrix.

### 12.3 Distortion & Waveshapers

*Soft clipping*: $y = \tanh(gx)$.  *Wavefolder*: `y = sin(x)` for Buchla timbres.  For alias‑free results oversample 4×, process, then decimate.

### 12.4 Master FX Rack

Organise as **plugin graph**—dynamically connects blocks via topological sort, reuse JUCE `dsp::ProcessGraph`.

---

## 13. System Architecture & Real‑Time Threading

### 13.1 Threads & Queues

*Audio thread* (high‑priority, no locks).  *Message thread* (GUI, presets).  *Background thread* (sample loading, convolution IR prep).

Use **lock‑free ring buffers** (single‐producer/single‑consumer) to push parameter changes.

### 13.2 Parameter Smoothing

Implement 1‑pole or 4‑pole smoothing at **480 Hz control rate** to reconcile GUI updates (±60 Hz) with 48 kHz DSP.

### 13.3 State Serialization

JUCE `ValueTree` with `AudioProcessor::getStateInformation()` / `setStateInformation()` ensures DAW session recall.

---

## 14. Performance Optimisation

### 14.1 Hot‑Loop Profiling

Use `perf`, VTune, or Xcode Instruments.  Prioritise optimising **oscillator inner loops** (≥30 % CPU) and **filter banks**.

### 14.2 SIMD Intrinsics vs JUCE Vector Ops

Hand‑rolled intrinsics (`_mm256_fmadd_ps`) outperform JUCE abstractions by \~10 %, but hide portability.  Use **DSP‑Graph batching** to process 8 voices × 8 samples (AVX2) at once.

### 14.3 Cache & Branching

Structure of Arrays (SoA) lets the CPU stream contiguous buffers.  Replace `if (envStage==...)` with **pre‑decoded function pointers** or **finite‑state tables** processed in blocks.

### 14.4 Denormal Protection

Flush‑to‑zero & DAZ hardware flags + add `1e‑24f` barefoot offset inside feedback loops.

---

## 15. Testing, Validation & Tooling

### 15.1 Unit & Integration Tests

*GoogleTest* for DSP algorithms: assert oscillator frequency within 0.1 ¢, filter magnitude response within ±0.2 dB.  Use **ImpulseTV** to auto‑plot impulse responses.

### 15.2 Audio Analyser Scripts

Python + Matplotlib: sweep cutoff 20 Hz–20 kHz, capture THD+N (< ‑100 dB for clean path).

### 15.3 Continuous Integration

GitHub Actions matrix: macOS/Windows/Linux, x64/ARM, Release/Debug.  Run headless **JUCE command‑line unit tests**.

### 15.4 User Beta Testing

Collect CPU load, crash logs.  Provide in‑plugin **profiling overlay** (FPS, XRuns).

---

## 16. Conclusion

A successful software synthesizer balances **DSP accuracy**, **user‑experience expressiveness**, and **engineering pragmatism**.  By applying band‑limited oscillator techniques, modern filter topologies, robust modulation architecture, and disciplined real‑time coding practices, developers can craft instruments that rival or surpass classic hardware.  Continuous listening, measurement, and iteration refine the instrument from prototype to polished product.

---

## Appendices

### A. BLEP Kernel Tables

*64‑sample polyBLEP kernel coefficients* for offline pre‑integration.

### B. MIDI 2.0 UMP Packet Layout

Diagram & C++ structs for parsing Universal MIDI Packets.

### C. JUCE Plug‑in Boilerplate

Minimal `AudioProcessor` subclass with **ValueTreeState** parameters and **dsp::ProcessSpec** initialisation.

---

## References

1. R. Bristow‑Johnson, “Audio EQ Cookbook,” 1999.
2. V. Välimäki, “Antiderivative Anti‑Aliasing,” *IEEE SPM*, 2023.
3. T. Stilson & J. Smith, “Alias‑Free Digital Synthesis of Classic Analog Waveforms,” ICMC 1996.
4. Will Pirkle, *Designing Audio Effect Plug‑Ins*, Focal 2019.
5. U. Zölzer (ed.), *DAFX – Digital Audio Effects*, Wiley 2011.
6. JUCE, “DSP Module,” online docs 2025.
