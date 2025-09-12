**Successor 37** is a professional virtual analog synthesizer VST plugin inspired by the classic Moog Subsequent 37 hardware. Built with JUCE and C++, it combines authentic analog character with modern digital enhancements.

---

## 🎹 Features

### Sound Generation
- **Dual Oscillators** + Sub Oscillator with Saw, Square, Triangle, and Sine waveforms
- **Hard Sync** and Pulse Width Modulation (PWM) capabilities
- **Noise Generator** for percussive and atmospheric textures

### Filter Section
- **Authentic Moog Ladder Filter** (24dB/oct low-pass)
- **Adjustable Resonance** with self-oscillation
- **MultiDrive** saturation for analog warmth
- **Key Tracking** and envelope modulation

### Modulation System
- **Dual LFOs** with multiple waveforms (Sine, Triangle, Saw, Square, Random)
- **Comprehensive Modulation Matrix** for flexible routing
- **ADSR Envelopes** for both amplitude and filter
- **MIDI Modulation** sources (Velocity, Mod Wheel, Aftertouch, Pitch Bend)

### Polyphony & Voice Architecture
- **Up to 8 voices** of true polyphony (exceeds hardware capabilities)
- **Multiple voice stealing algorithms**
- **Unison mode** with detune and spread controls

### Effects & Processing
- **Stereo Delay** with tempo sync and filtering
- **Chorus/Flanger** for width and movement
- **Professional-grade DSP** with SIMD optimization

### Interface
- **Hardware-inspired UI** with custom graphics
- **Resizable interface** for high-DPI displays
- **MIDI Learn** for easy controller mapping
- **Preset system** with factory and user banks

---

## 🚀 Getting Started

### Prerequisites
- **JUCE Framework** (v6.0+)
- **C++17** compatible compiler
- **VST3** compatible DAW (Ableton Live, FL Studio, Logic Pro, etc.)

### Building the Project

```bash
# Clone the repository
git clone https://github.com/your-username/successor37.git
cd successor37

# Configure with CMake
mkdir build && cd build
cmake .. -DJUCE_ROOT="path/to/JUCE"

# Build (choose one)
cmake --build . --config Release  # Release build
cmake --build . --config Debug    # Debug build
```

### Installation
- **Windows**: Copy `.vst3` file to `C:\Program Files\Common Files\VST3`
- **macOS**: Copy `.vst3` to `/Library/Audio/Plug-Ins/VST3` or `.component` to `/Library/Audio/Plug-Ins/Components`

---

## 🎚️ Quick Start Guide

1. **Load the plugin** in your DAW on a MIDI track
2. **Select a waveform** (Saw, Square, Triangle, or Sine)
3. **Adjust filter cutoff** to shape the tone
4. **Modulate parameters** using LFOs and envelopes
5. **Save your sounds** using the preset system

### Basic Sound Recipes

**Bass Sound:**
- Waveform: Saw
- Filter Cutoff: 40%
- Resonance: 60%
- Envelope: Fast Attack, Medium Decay

**Lead Sound:**
- Waveform: Square + PWM
- Filter Cutoff: 70%
- Add slight LFO to pitch for vibrato

**Pad Sound:**
- Dual oscillators slightly detuned
- Slow attack/release envelopes
- Add chorus and delay effects

---

## 🛠️ Development

### Project Structure
```
Successor37/
├── Source/           # Core C++ source files
│   ├── PluginProcessor.h/cpp
│   ├── PluginEditor.h/cpp
│   ├── Oscillator.h/cpp
│   ├── Filters/      # DSP components
│   ├── Modulation/   # LFOs, envelopes, matrix
│   └── Effects/      # Delay, chorus implementations
├── Assets/           # UI graphics and resources
├── Build/            # Build artifacts
└── Docs/             # Documentation
```

### Key Components
- **Modular DSP architecture** for easy expansion
- **Sample-accurate processing** for professional audio quality
- **Thread-safe parameter handling** for smooth automation
- **Custom graphics rendering** with sprite-based knobs

---

## 📋 System Requirements

### Minimum
- **CPU**: x64 processor with SSE2 support
- **RAM**: 4GB
- **OS**: Windows 10 (1809+) or macOS 10.14+
- **DAW**: VST3 compatible host

### Recommended
- **CPU**: Multi-core processor (Intel i5+ or AMD Ryzen 5+)
- **RAM**: 8GB+
- **Audio Interface**: ASIO/CoreAudio compatible
- **Display**: 1440×900 resolution minimum

---

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details on:
- Code style and standards
- Pull request process
- Bug reporting and feature requests

---

## 📜 License

This project is licensed under the **GPLv3 License** - see the [LICENSE.md](LICENSE.md) file for details.

---

## 🙏 Acknowledgments

- **JUCE Framework** for the incredible audio development platform
- **Moog Music Inc.** for the inspiration from their iconic instruments
- **Open source community** for various DSP algorithms and techniques

---
