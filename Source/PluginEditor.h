// PluginEditor.h
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h" // We'll create this next

class Successor37AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Successor37AudioProcessorEditor (Successor37AudioProcessor&);
    ~Successor37AudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Successor37AudioProcessor& audioProcessor;
    
    // Custom look and feel
    CustomLookAndFeel customLookAndFeel;
    
    // Background image
    std::unique_ptr<juce::Drawable> backgroundImage;
    
    // Section headers
    juce::Label oscSectionLabel, filterSectionLabel, envSectionLabel, lfoSectionLabel;
    
    // Oscillator Section
    juce::Slider oscWaveformSlider;
    juce::Slider oscTuneSlider;
    juce::Slider oscPWMSlider;
    juce::ComboBox oscWaveformSelector;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscTuneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscPWMAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveformAttachment;
    
    // Filter Section
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;
    juce::Slider filterDriveSlider;
    juce::Slider filterEnvAmountSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDriveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvAmountAttachment;
    
    // Amplitude Envelope Section
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    // Filter Envelope Section
    juce::Slider filterAttackSlider;
    juce::Slider filterDecaySlider;
    juce::Slider filterSustainSlider;
    juce::Slider filterReleaseSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterSustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterReleaseAttachment;
    
    // LFO Section
    juce::Slider lfo1RateSlider;
    juce::Slider lfo1AmountSlider;
    juce::ComboBox lfo1WaveformSelector;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1RateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1AmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfo1WaveformAttachment;
    
    // Modulation Section
    juce::Slider modWheelAmountSlider;
    juce::Slider velocityAmountSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modWheelAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityAmountAttachment;
    
    // Output Section
    juce::Slider masterVolumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolumeAttachment;
    
    // Labels for sliders
    juce::Label oscTuneLabel, oscPWMLabel, filterCutoffLabel, filterResonanceLabel;
    juce::Label filterDriveLabel, filterEnvAmountLabel, attackLabel, decayLabel;
    juce::Label sustainLabel, releaseLabel, lfo1RateLabel, lfo1AmountLabel;
    juce::Label modWheelLabel, velocityLabel, masterVolumeLabel;
    
    void createSlider(juce::Slider& slider, juce::Label& label, const juce::String& name, 
                     juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID);
    void createComboBox(juce::ComboBox& comboBox, const juce::String& name,
                       juce::AudioProcessorValueTreeState& apvts, const juce::String& paramID);

    // Helper methods for UI sections
    void createOscillatorSection();
    void createFilterSection();
    void createEnvelopeSection();
    void createModulationSection();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Successor37AudioProcessorEditor)
};