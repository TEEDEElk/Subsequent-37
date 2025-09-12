#pragma once

#include <JuceHeader.h>

class PresetManager
{
public:
    PresetManager(juce::AudioProcessorValueTreeState& apvts);
    
    void savePreset(const juce::String& presetName);
    void loadPreset(const juce::String& presetName);
    void deletePreset(const juce::String& presetName);
    
    juce::StringArray getAllPresets() const;
    juce::String getCurrentPreset() const { return currentPresetName; }
    
    static const juce::File getPresetDirectory();
    
private:
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::String currentPresetName;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetManager)
};