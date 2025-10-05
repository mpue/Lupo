# LUPO SYNTH - AUDIO PROCESSING FLOW DIAGRAM

## LUPO SYNTH MAIN FLOW

### INITIALIZATION

```
┌─────────────────────────────────────────────────┐
│          LupoSynth Constructor                  │
│ ┌─────────────────┬─────────────────────────┐   │
│ │     Model*      │      ModMatrix*         │   │
│ │  (Patch Data)   │   (Modulation Routing)  │   │
│ └─────────────────┴─────────────────────────┘   │
└─────────────────────────────────────────────────┘
                      ↓
┌─────────────────────────────────────────────────┐
│            prepareToPlay()                      │
│  • Set Sample Rate & Buffer Size                │
│  • Initialize Voices                            │
│  • Configure Oscillators                        │
│  • Setup Effects Chain                          │
└─────────────────────────────────────────────────┘
```

---

## MAIN PROCESSING LOOP

### processBlock()

```
┌─────────────────────────────────────────────────┐
│              processMidi()                      │
│                                                 │
│  MIDI INPUT PROCESSING:                         │
│                                                 │
│  For each MIDI message in buffer:               │
│                                                 │
│  Note On?                                       │
│  ├─► findFreeVoice(noteNumber)                  │
│  ├─► voice->startNote()                         │
│  └─► Configure Voice Parameters                 │
│                                                 │
│  Note Off?                                      │
│  ├─► Find active voice for note                 │
│  └─► voice->stopNote()                          │
│                                                 │
│  Control Change?                                │
│  └─► Update Parameter Values                    │
└─────────────────────────────────────────────────┘
                      ↓
```

### VOICE PROCESSING

```
┌─────────────────────────────────────────────────┐
│  For each active Voice:                         │
│                                                 │
│  OSCILLATOR STAGE:                              │
│  ├─► Generate base waveforms                    │
│  ├─► Apply pitch/detune                         │
│  ├─► Mix multiple oscillators                   │
│  └─► Apply FM synthesis                         │
│                                                 │
│  MODULATION STAGE:                              │
│  ├─► Update LFOs                                │
│  ├─► Update Envelopes                           │
│  └─► Apply ModMatrix routing                    │
│                                                 │
│  FILTER STAGE:                                  │
│  ├─► Apply multimode filtering                  │
│  ├─► Apply envelope modulation                  │
│  └─► Apply LFO modulation                       │
│                                                 │
│  AMPLIFIER STAGE:                               │
│  ├─► Apply amplitude envelope                   │
│  ├─► Apply velocity sensitivity                 │
│  └─► Apply panning                              │
└─────────────────────────────────────────────────┘
                      ↓
```

### ARPEGGIATOR STAGE

```
┌─────────────────────────────────────────────────┐
│  if (arpeggiator.enabled):                      │
│                                                 │
│  Note Collection:                               │
│  ├─► Collect held notes                         │
│  └─► Sort by pitch/time                         │
│                                                 │
│  Timing Control:                                │
│  ├─► Internal Clock: Use tempo                  │
│  └─► MIDI Clock: Sync to host                   │
│                                                 │
│  Pattern Generation:                            │
│  ├─► Mode::Up: Ascending sequence               │
│  ├─► Mode::Down: Descending sequence            │
│  └─► Mode::Random: Random selection             │
│                                                 │
│  Octave Expansion:                              │
│  └─► Repeat pattern across octaves              │
└─────────────────────────────────────────────────┘
                      ↓
```

### EFFECTS PROCESSING

```
┌─────────────────────────────────────────────────┐
│  Audio Signal Pipeline:                         │
│                                                 │
│  DISTORTION:                                    │
│  ├─► Apply drive/saturation                     │
│  └─► Multiple distortion modes                  │
│                                                 │
│  CHORUS:                                        │
│  ├─► Modulated delay lines                      │
│  ├─► LFO modulation                             │
│  └─► Feedback control                           │
│                                                 │
│  DELAY:                                         │
│  ├─► Left/Right delay times                     │
│  ├─► Feedback amount                            │
│  └─► Wet/Dry mix                                │
│                                                 │
│  REVERB:                                        │
│  ├─► Room size simulation                       │
│  ├─► Damping control                            │
│  └─► Stereo width                               │
└─────────────────────────────────────────────────┘
                      ↓
```

### FINAL MIXING STAGE

```
┌─────────────────────────────────────────────────┐
│  MAIN VOLUME CONTROL                            │
│  STEREO OUTPUT                                  │
│  PEAK LEVEL MONITORING                          │
│                                                 │
│  • Mix all voices to stereo output              │
│  • Apply master volume                          │
│  • Update peak meters                           │
│  • Output to audio buffer                       │
└─────────────────────────────────────────────────┘
```

---

## PARAMETER UPDATE CALLBACKS

### parameterChanged()

**UI Parameter Updates:**

```
Real-time Parameter Changes:

• Oscillator Settings
• Filter Parameters
• Envelope Timings
• LFO Rates
• Effect Parameters
• Arpeggiator Settings

──► Update internal model
──► Apply to active voices
──► Update modulation routing
```

### updateState()

**Preset Loading:**

```
• Load preset from ValueTree
• Parse modulation matrix config
• Update all parameters
• Reconfigure synthesis engine
• Reset voice states
```

---

## KEY COMPONENTS DETAILS

### VOICE MANAGEMENT

- **findFreeVoice()** - Allocates voices for new notes
- **Voice lifecycle**: Note On → Processing → Note Off → Release
- **Polyphonic voice pool** with intelligent allocation

### MODULATION SYSTEM

- **ModMatrix** routes modulators to destinations
- **LFOs** provide cyclic modulation
- **Envelopes** provide time-based modulation
- **Real-time parameter** interpolation

### OSCILLATOR ENGINE

- **Multiple oscillator types** (Sawtooth, etc.)
- **FM synthesis** capabilities
- **Phase/pitch modulation**
- **Voice-specific** oscillator instances

### PARAMETER SYSTEM

- **AttachmentFactory** manages UI↔Engine connections
- **AudioProcessorValueTreeState** handles automation
- **Real-time parameter updates** without clicks
- **Preset save/load** functionality

### PERFORMANCE MONITORING

- **Peak level detection** for meters
- **Voice allocation** tracking
- **CPU usage** optimization
- **Buffer underrun** protection