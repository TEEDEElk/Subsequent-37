// PluginEditor.h
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

class Successor37AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    Successor37AudioProcessorEditor(Successor37AudioProcessor&);
    ~Successor37AudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    Successor37AudioProcessor& audioProcessor;
    
    // Custom look and feel
    CustomLookAndFeel customLookAndFeel;
    
    // Oscillator Section
    juce::ComboBox oscWaveformSelector;
    juce::Slider oscTuneSlider;
    juce::Slider oscPWMSlider;
    juce::Label oscTuneLabel;
    juce::Label oscPWMLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveformAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscTuneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oscPWMAttachment;
    
    // Filter Section
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;
    juce::Slider filterDriveSlider;
    juce::Slider filterEnvAmountSlider;
    juce::Label filterCutoffLabel;
    juce::Label filterResonanceLabel;
    juce::Label filterDriveLabel;
    juce::Label filterEnvAmountLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDriveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterEnvAmountAttachment;
    
    // Amplitude Envelope Section
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    // Filter Envelope Section
    juce::Slider filterAttackSlider;
    juce::Slider filterDecaySlider;
    juce::Slider filterSustainSlider;
    juce::Slider filterReleaseSlider;
    juce::Label filterAttackLabel;
    juce::Label filterDecayLabel;
    juce::Label filterSustainLabel;
    juce::Label filterReleaseLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterSustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterReleaseAttachment;
    
    // LFO and Modulation Section
    juce::Slider lfo1RateSlider;
    juce::Slider lfo1AmountSlider;
    juce::Label lfo1RateLabel;
    juce::Label lfo1AmountLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1RateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lfo1AmountAttachment;
    
    // Master Volume
    juce::Slider masterVolumeSlider;
    juce::Label masterVolumeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolumeAttachment;
    
    // Helper methods for creating UI
    void createOscillatorSection();
    void createFilterSection();
    void createEnvelopeSection();
    void createModulationSection();
    
    // Layout helpers
    void layoutOscillatorSection(juce::Rectangle<int> area);
    void layoutFilterSection(juce::Rectangle<int> area);
    void layoutEnvelopeSection(juce::Rectangle<int> area);
    void layoutModulationSection(juce::Rectangle<int> area);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Successor37AudioProcessorEditor)
};
