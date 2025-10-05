#  LUPO SYNTHESIZER - ARCHITEKTUR ÜBERSICHT
## USER INTERFACE

```
┌─────────────────┬────────────────┬─────────────────┬─────────────┐
│  PluginEditor   │    MainUI      │ CyberpunkLAF    │  ArpPanel   │
│ (Main Window)   │ (Main Layout)  │ (Look & Feel)   │ (Arpeggio)  │
├─────────────────┼────────────────┼─────────────────┼─────────────┤
│  FilterPanel    │  ReverbPanel   │  ChorusPanel    │  OscPanel   │
│ (Filter Ctrls)  │ (Reverb FX)    │ (Chorus FX)     │(Oscillator) │
└─────────────────┴────────────────┴─────────────────┴─────────────┘
                              ↓
```

## PARAMETER MANAGEMENT

```
┌─────────────────────────────────────────────────────────────────┐
│                      AttachmentFactory                          │
├─────────────────┬─────────────────┬─────────────────────────────┤
│ SliderAttach.   │ ComboAttach.    │ ButtonAttachment            │
│ (Rotary/Linear) │ (Dropdowns)     │ (Toggles)                   │
└─────────────────┴─────────────────┴─────────────────────────────┘
                              ↓
```

## AUDIO PROCESSING CORE

```
┌──────────────────────────────────────┐
│      LupoAudioProcessor              │
│   (Main Plugin Processor)            │
└──────────────────────────────────────┘
                  ↓
┌─────────────────────────────────────────────────────────────────┐
│                         LupoSynth                               │
│                    (Synthesis Engine)                           │
├─────────────────┬─────────────────┬─────────────────────────────┤
│     Model       │   ModMatrix     │      Voice Pool             │
│ (Patch Data)    │ (Modulation)    │  (Polyphonic Voices)        │
└─────────────────┴─────────────────┴─────────────────────────────┘
                              ↓
```

## AUDIO ENGINE COMPONENTS

```
┌─────────────────┬─────────────────┬─────────────────┬─────────────┐
│  Arpeggiator    │  Oscillators    │    Filters      │   Effects   │
├─────────────────┼─────────────────┼─────────────────┼─────────────┤
│ • Up/Down/Rand  │ • Sawtooth      │ • Multimode     │ • Reverb    │
│ • Clock Sync    │ • Multiple Osc  │ • Cutoff/Res    │ • Chorus    │
│ • Tempo Control │ • Wave Shaping  │ • Envelope Mod  │ • Delay     │
│ • MIDI/Internal │ • Phase/Detune  │ • LFO Mod       │ • Distortion│
└─────────────────┴─────────────────┴─────────────────┴─────────────┘
                              ↓
```

## MIDI & AUDIO I/O

```
┌──────────────────┬─────────────────────┬───────────────────┐
│  MIDI INPUT      │ AUDIO PROCESSING    │   AUDIO OUTPUT    │
├──────────────────┼─────────────────────┼───────────────────┤
│ • Note On/Off    │ • Sample Rate       │ • Stereo Out      │
│ • CC Messages    │ • Buffer Processing │ • Plugin Format   │
│ • Clock Sync     │ • Real-time Proc.   │ • DAW Integration │
│ • Arp Input      │ • Voice Management  │ • Standalone Mode │
└──────────────────┴─────────────────────┴───────────────────┘
```

---

#  ARCHITEKTUR-DETAILS

##  HAUPTKOMPONENTEN

### 1. **UI LAYER (Presentation)**
- **PluginEditor**: Haupt-Editor-Fenster 
- **MainUI**: Layout-Management für alle Panels
- **CyberpunkLookAndFeel**: Visueller Stil (Neon-Farben, kleinere Fonts)
- **Panels**: Spezialisierte UI-Komponenten für verschiedene Features

### 2. **PARAMETER LAYER (Control)**
- **AttachmentFactory**: Zentrale Verwaltung aller Parameter-Verbindungen
- **AudioProcessorValueTreeState**: JUCE Parameter-System
- **Slider/Combo/Button Attachments**: UI ↔ Parameter Bindungen

### 3. **AUDIO ENGINE (Business Logic)**
- **LupoAudioProcessor**: Haupt-Plugin-Processor (JUCE AudioProcessor)
- **LupoSynth**: Zentrale Synthesizer-Engine
- **Model**: Patch-Daten und Zustandsverwaltung
- **ModMatrix**: Modulations-Routing-System

### 4. **SYNTHESIS COMPONENTS (Audio Processing)**
- **Arpeggiator**: MIDI-Arpeggiation mit verschiedenen Modi
- **Oscillators**: Wellenform-Generierung (Sawtooth, etc.)
- **Filters**: Frequenz-Filterung mit Modulationen
- **Effects**: Audio-Effekte (Reverb, Chorus, etc.)
- **Voice**: Polyphonie-Management

## DATENFLUSS

1. **MIDI Input** → **Arpeggiator** → **Voice Management**
2. **UI Controls** → **AttachmentFactory** → **Parameters**
3. **Parameters** → **LupoSynth** → **Audio Processing**
4. **Oscillators** → **Filters** → **Effects** → **Audio Output**

##  DESIGN PATTERNS

- **MVC**: UI-Panels, Model-Klasse, Controller über AttachmentFactory
- **Observer**: ChangeListener/ChangeBroadcaster für Updates
- **Factory**: AttachmentFactory für Parameter-Erstellung
- **Strategy**: Verschiedene Oscillator-Modi und Arpeggiator-Modi