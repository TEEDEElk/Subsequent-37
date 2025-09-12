#include "SynthSound.h"
#include "SynthVoice.h"

#pragma once

#include "Oscillator.h"
#include "ADSREnvelope.h"
#include "MoogFilter.h"
#include <JuceHeader.h>

enum class NotePriority {
    LastNote,
    FirstNote,
    HighestNote,
    LowestNote
};

class Successor37AudioProcessor : public juce::AudioProcessor {
public:
    // ... existing code ...

    juce::AudioProcessorValueTreeState& getValueTreeState() { return parameters; }
    juce::Synthesiser& getSynth() { return synth; }

// ...existing code...
#include "Arpeggiator.h"
#include "StereoDelay.h"
#include "Chorus.h"
#include "PresetManager.h"

class Successor37AudioProcessor : public juce::AudioProcessor
{
    // ... existing code ...

private:
    // Effects
    StereoDelay delay;
    Chorus chorus;
    Arpeggiator arpeggiator;

    // Preset system
    std::unique_ptr<PresetManager> presetManager;

    // Host info
    double currentBPM = 120.0;
    bool isPlaying = false;

    void updateHostInfo();
};

    // Old MIDI state variables, oscillator, envelope, filter instances removed

    // Parameters remain the same
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioParameterChoice* oscWaveformParam;
    // ... other parameter references

    void updateParameters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Successor37AudioProcessor)
};