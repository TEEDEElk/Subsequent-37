// PluginProcessor.h
#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"
#include "Arpeggiator.h"
#include "StereoDelay.h"
#include "Chorus.h"
#include "PresetManager.h"

enum class NotePriority {
    LastNote,
    FirstNote,
    HighestNote,
    LowestNote
};

class Successor37AudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    Successor37AudioProcessor();
    ~Successor37AudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // Public accessors
    juce::AudioProcessorValueTreeState& getValueTreeState() { return parameters; }
    juce::Synthesiser& getSynth() { return synth; }
    PresetManager* getPresetManager() { return presetManager.get(); }

private:
    //==============================================================================
    // Audio processing components
    juce::Synthesiser synth;
    std::unique_ptr<SynthSound> synthSound;
    
    // Effects
    StereoDelay delay;
    Chorus chorus;
    Arpeggiator arpeggiator;

    // Preset system
    std::unique_ptr<PresetManager> presetManager;

    // Parameters
    juce::AudioProcessorValueTreeState parameters;
    
    // Parameter pointers (for faster access)
    juce::AudioParameterChoice* oscWaveformParam;
    juce::AudioParameterFloat* oscTuneParam;
    juce::AudioParameterFloat* oscPWMParam;
    juce::AudioParameterFloat* filterCutoffParam;
    juce::AudioParameterFloat* filterResonanceParam;
    juce::AudioParameterFloat* filterDriveParam;
    juce::AudioParameterFloat* filterEnvAmountParam;
    juce::AudioParameterFloat* ampAttackParam;
    juce::AudioParameterFloat* ampDecayParam;
    juce::AudioParameterFloat* ampSustainParam;
    juce::AudioParameterFloat* ampReleaseParam;
    juce::AudioParameterFloat* filterAttackParam;
    juce::AudioParameterFloat* filterDecayParam;
    juce::AudioParameterFloat* filterSustainParam;
    juce::AudioParameterFloat* filterReleaseParam;
    juce::AudioParameterFloat* masterVolumeParam;

    // Host info
    double currentBPM = 120.0;
    bool isPlaying = false;

    // Helper methods
    void updateHostInfo();
    void updateParameters();
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Successor37AudioProcessor)
};
