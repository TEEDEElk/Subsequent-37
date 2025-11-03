// PluginEditor.cpp
#include "PluginEditor.h"

//==============================================================================
Successor37AudioProcessorEditor::Successor37AudioProcessorEditor(Successor37AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set overall size
    setSize(800, 600);
    
    // Set custom look and feel
    setLookAndFeel(&customLookAndFeel);
    
    // Create UI sections
    createOscillatorSection();
    createFilterSection();
    createEnvelopeSection();
    createModulationSection();
}

Successor37AudioProcessorEditor::~Successor37AudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void Successor37AudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background gradient
    juce::ColourGradient gradient(
        juce::Colour(0xff2c3e50), 0.0f, 0.0f,
        juce::Colour(0xff34495e), 0.0f, static_cast<float>(getHeight()),
        false
    );
    g.setGradientFill(gradient);
    g.fillAll();
    
    // Draw title
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(24.0f, juce::Font::bold));
    g.drawText("SUCCESSOR 37", getLocalBounds().removeFromTop(40), 
               juce::Justification::centred, true);
}

void Successor37AudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    area.removeFromTop(50); // Title space
    
    const int sectionHeight = area.getHeight() / 2;
    
    // Top row: Oscillator and Filter
    auto topRow = area.removeFromTop(sectionHeight);
    auto oscSection = topRow.removeFromLeft(area.getWidth() / 2);
    auto filterSection = topRow;
    
    // Bottom row: Envelopes and Modulation
    auto bottomRow = area;
    auto envSection = bottomRow.removeFromLeft(area.getWidth() / 2);
    auto modSection = bottomRow;
    
    // Layout oscillator section
    layoutOscillatorSection(oscSection);
    
    // Layout filter section
    layoutFilterSection(filterSection);
    
    // Layout envelope section
    layoutEnvelopeSection(envSection);
    
    // Layout modulation section
    layoutModulationSection(modSection);
}

//==============================================================================
void Successor37AudioProcessorEditor::createOscillatorSection()
{
    // Oscillator waveform selector
    oscWaveformSelector.addItem("Sine", 1);
    oscWaveformSelector.addItem("Saw", 2);
    oscWaveformSelector.addItem("Square", 3);
    oscWaveformSelector.addItem("Triangle", 4);
    oscWaveformSelector.setSelectedId(2);
    addAndMakeVisible(oscWaveformSelector);
    oscWaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), "oscWaveform", oscWaveformSelector);
    
    // Oscillator tune slider
    oscTuneSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscTuneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(oscTuneSlider);
    oscTuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "oscTune", oscTuneSlider);
    
    oscTuneLabel.setText("Tune", juce::dontSendNotification);
    oscTuneLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscTuneLabel);
    
    // PWM slider
    oscPWMSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscPWMSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(oscPWMSlider);
    oscPWMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "oscPWM", oscPWMSlider);
    
    oscPWMLabel.setText("PWM", juce::dontSendNotification);
    oscPWMLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscPWMLabel);
}

void Successor37AudioProcessorEditor::createFilterSection()
{
    // Filter cutoff
    filterCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(filterCutoffSlider);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterCutoff", filterCutoffSlider);
    
    filterCutoffLabel.setText("Cutoff", juce::dontSendNotification);
    filterCutoffLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterCutoffLabel);
    
    // Filter resonance
    filterResonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterResonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(filterResonanceSlider);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterResonance", filterResonanceSlider);
    
    filterResonanceLabel.setText("Resonance", juce::dontSendNotification);
    filterResonanceLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterResonanceLabel);
    
    // Filter drive
    filterDriveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterDriveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(filterDriveSlider);
    filterDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterDrive", filterDriveSlider);
    
    filterDriveLabel.setText("Drive", juce::dontSendNotification);
    filterDriveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterDriveLabel);
    
    // Filter envelope amount
    filterEnvAmountSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterEnvAmountSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(filterEnvAmountSlider);
    filterEnvAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterEnvAmount", filterEnvAmountSlider);
    
    filterEnvAmountLabel.setText("Env Amount", juce::dontSendNotification);
    filterEnvAmountLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterEnvAmountLabel);
}

void Successor37AudioProcessorEditor::createEnvelopeSection()
{
    // Amp envelope
    auto createAmpSlider = [this](juce::Slider& slider, juce::Label& label, 
                                   const juce::String& labelText, const juce::String& paramID)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        addAndMakeVisible(slider);
        
        label.setText(labelText, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
    };
    
    createAmpSlider(attackSlider, attackLabel, "A", "ampAttack");
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "ampAttack", attackSlider);
    
    createAmpSlider(decaySlider, decayLabel, "D", "ampDecay");
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "ampDecay", decaySlider);
    
    createAmpSlider(sustainSlider, sustainLabel, "S", "ampSustain");
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "ampSustain", sustainSlider);
    
    createAmpSlider(releaseSlider, releaseLabel, "R", "ampRelease");
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "ampRelease", releaseSlider);
    
    // Filter envelope
    createAmpSlider(filterAttackSlider, filterAttackLabel, "A", "filterAttack");
    filterAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterAttack", filterAttackSlider);
    
    createAmpSlider(filterDecaySlider, filterDecayLabel, "D", "filterDecay");
    filterDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterDecay", filterDecaySlider);
    
    createAmpSlider(filterSustainSlider, filterSustainLabel, "S", "filterSustain");
    filterSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterSustain", filterSustainSlider);
    
    createAmpSlider(filterReleaseSlider, filterReleaseLabel, "R", "filterRelease");
    filterReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "filterRelease", filterReleaseSlider);
}

void Successor37AudioProcessorEditor::createModulationSection()
{
    // LFO Rate
    lfo1RateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfo1RateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(lfo1RateSlider);
    lfo1RateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "lfo1Rate", lfo1RateSlider);
    
    lfo1RateLabel.setText("LFO Rate", juce::dontSendNotification);
    lfo1RateLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lfo1RateLabel);
    
    // LFO Amount (to filter)
    lfo1AmountSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfo1AmountSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(lfo1AmountSlider);
    lfo1AmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "lfo1ToFilter", lfo1AmountSlider);
    
    lfo1AmountLabel.setText("LFO > Filter", juce::dontSendNotification);
    lfo1AmountLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lfo1AmountLabel);
    
    // Master volume
    masterVolumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    masterVolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(masterVolumeSlider);
    masterVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "masterVolume", masterVolumeSlider);
    
    masterVolumeLabel.setText("Volume", juce::dontSendNotification);
    masterVolumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(masterVolumeLabel);
}

//==============================================================================
void Successor37AudioProcessorEditor::layoutOscillatorSection(juce::Rectangle<int> area)
{
    area.reduce(10, 10);
    
    auto row1 = area.removeFromTop(80);
    oscWaveformSelector.setBounds(row1.removeFromLeft(120).reduced(5));
    oscTuneSlider.setBounds(row1.removeFromLeft(80).reduced(5));
    oscTuneLabel.setBounds(oscTuneSlider.getBounds().removeFromBottom(20));
    
    auto row2 = area.removeFromTop(80);
    oscPWMSlider.setBounds(row2.removeFromLeft(80).reduced(5));
    oscPWMLabel.setBounds(oscPWMSlider.getBounds().removeFromBottom(20));
}

void Successor37AudioProcessorEditor::layoutFilterSection(juce::Rectangle<int> area)
{
    area.reduce(10, 10);
    
    auto row1 = area.removeFromTop(80);
    filterCutoffSlider.setBounds(row1.removeFromLeft(80).reduced(5));
    filterCutoffLabel.setBounds(filterCutoffSlider.getBounds().removeFromBottom(20));
    
    filterResonanceSlider.setBounds(row1.removeFromLeft(80).reduced(5));
    filterResonanceLabel.setBounds(filterResonanceSlider.getBounds().removeFromBottom(20));
    
    auto row2 = area.removeFromTop(80);
    filterDriveSlider.setBounds(row2.removeFromLeft(80).reduced(5));
    filterDriveLabel.setBounds(filterDriveSlider.getBounds().removeFromBottom(20));
    
    filterEnvAmountSlider.setBounds(row2.removeFromLeft(80).reduced(5));
    filterEnvAmountLabel.setBounds(filterEnvAmountSlider.getBounds().removeFromBottom(20));
}

void Successor37AudioProcessorEditor::layoutEnvelopeSection(juce::Rectangle<int> area)
{
    area.reduce(10, 10);
    
    // Amp envelope row
    auto ampRow = area.removeFromTop(80);
    attackSlider.setBounds(ampRow.removeFromLeft(70).reduced(5));
    attackLabel.setBounds(attackSlider.getBounds().removeFromBottom(20));
    
    decaySlider.setBounds(ampRow.removeFromLeft(70).reduced(5));
    decayLabel.setBounds(decaySlider.getBounds().removeFromBottom(20));
    
    sustainSlider.setBounds(ampRow.removeFromLeft(70).reduced(5));
    sustainLabel.setBounds(sustainSlider.getBounds().removeFromBottom(20));
    
    releaseSlider.setBounds(ampRow.removeFromLeft(70).reduced(5));
    releaseLabel.setBounds(releaseSlider.getBounds().removeFromBottom(20));
    
    // Filter envelope row
    auto filterRow = area.removeFromTop(80);
    filterAttackSlider.setBounds(filterRow.removeFromLeft(70).reduced(5));
    filterAttackLabel.setBounds(filterAttackSlider.getBounds().removeFromBottom(20));
    
    filterDecaySlider.setBounds(filterRow.removeFromLeft(70).reduced(5));
    filterDecayLabel.setBounds(filterDecaySlider.getBounds().removeFromBottom(20));
    
    filterSustainSlider.setBounds(filterRow.removeFromLeft(70).reduced(5));
    filterSustainLabel.setBounds(filterSustainSlider.getBounds().removeFromBottom(20));
    
    filterReleaseSlider.setBounds(filterRow.removeFromLeft(70).reduced(5));
    filterReleaseLabel.setBounds(filterReleaseSlider.getBounds().removeFromBottom(20));
}

void Successor37AudioProcessorEditor::layoutModulationSection(juce::Rectangle<int> area)
{
    area.reduce(10, 10);
    
    auto row1 = area.removeFromTop(80);
    lfo1RateSlider.setBounds(row1.removeFromLeft(80).reduced(5));
    lfo1RateLabel.setBounds(lfo1RateSlider.getBounds().removeFromBottom(20));
    
    lfo1AmountSlider.setBounds(row1.removeFromLeft(80).reduced(5));
    lfo1AmountLabel.setBounds(lfo1AmountSlider.getBounds().removeFromBottom(20));
    
    auto row2 = area.removeFromTop(80);
    masterVolumeSlider.setBounds(row2.removeFromLeft(80).reduced(5));
    masterVolumeLabel.setBounds(masterVolumeSlider.getBounds().removeFromBottom(20));
}
