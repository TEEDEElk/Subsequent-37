void Successor37AudioProcessorEditor::createOscillatorSection()
{
    // Oscillator waveform selector
    oscWaveformSelector.addItem("Saw", 1);
    oscWaveformSelector.addItem("Square", 2);
    oscWaveformSelector.addItem("Triangle", 3);
    oscWaveformSelector.addItem("Sine", 4);
    oscWaveformSelector.setSelectedId(1);
    addAndMakeVisible(oscWaveformSelector);
    oscWaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts, "oscWaveform", oscWaveformSelector);
    
    // Oscillator parameters
    createSlider(oscTuneSlider, oscTuneLabel, "TUNE", audioProcessor.apvts, "oscTune");
    createSlider(oscPWMSlider, oscPWMLabel, "PWM", audioProcessor.apvts, "oscPWM");
}

void Successor37AudioProcessorEditor::createFilterSection()
{
    createSlider(filterCutoffSlider, filterCutoffLabel, "CUTOFF", audioProcessor.apvts, "filterCutoff");
    createSlider(filterResonanceSlider, filterResonanceLabel, "RES", audioProcessor.apvts, "filterResonance");
    createSlider(filterDriveSlider, filterDriveLabel, "DRIVE", audioProcessor.apvts, "filterDrive");
    createSlider(filterEnvAmountSlider, filterEnvAmountLabel, "ENV AMT", audioProcessor.apvts, "filterEnvAmount");
}

void Successor37AudioProcessorEditor::createEnvelopeSection()
{
    // Amp envelope
    createSlider(attackSlider, attackLabel, "ATTACK", audioProcessor.apvts, "attack");
    createSlider(decaySlider, decayLabel, "DECAY", audioProcessor.apvts, "decay");
    createSlider(sustainSlider, sustainLabel, "SUSTAIN", audioProcessor.apvts, "sustain");
    createSlider(releaseSlider, releaseLabel, "RELEASE", audioProcessor.apvts, "release");
    
    // Filter envelope
    createSlider(filterAttackSlider, filterAttackLabel, "ATTACK", audioProcessor.apvts, "filterAttack");
    createSlider(filterDecaySlider, filterDecayLabel, "DECAY", audioProcessor.apvts, "filterDecay");
    createSlider(filterSustainSlider, filterSustainLabel, "SUSTAIN", audioProcessor.apvts, "filterSustain");
    createSlider(filterReleaseSlider, filterReleaseLabel, "RELEASE", audioProcessor.apvts, "filterRelease");
}

void Successor37AudioProcessorEditor::createModulationSection()
{
    createSlider(lfo1RateSlider, lfo1RateLabel, "RATE", audioProcessor.apvts, "lfo1Rate");
    createSlider(lfo1AmountSlider, lfo1AmountLabel, "AMOUNT", audioProcessor.apvts, "lfo1Amount");
    createSlider(modWheelAmountSlider, modWheelLabel, "MOD WHEEL", audioProcessor.apvts, "modWheelToFilter");
    createSlider(velocityAmountSlider, velocityLabel, "VELOCITY", audioProcessor.apvts, "velocityToFilter");
    createSlider(masterVolumeSlider, masterVolumeLabel, "VOLUME", audioProcessor.apvts, "masterVolume");
}
// PluginEditor.cpp
#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
Successor37AudioProcessorEditor::Successor37AudioProcessorEditor(Successor37AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set overall size
    setSize(1000, 600);
    
    // Load custom graphics
    loadCustomGraphics();
    
    // Set custom look and feel
    setLookAndFeel(&customLookAndFeel);
    
    // Create all UI components
    createAllComponents();
}

void Successor37AudioProcessorEditor::loadCustomGraphics()
{
    // Load knob sprite sheet
    bool knobsLoaded = customLookAndFeel.loadKnobGraphics(
        BinaryData::knob_strip_png, 
        BinaryData::knob_strip_pngSize
    );
    
    // Load background image
    bool backgroundLoaded = customLookAndFeel.loadBackgroundGraphics(
        BinaryData::panel_bg_png,
        BinaryData::panel_bg_pngSize
    );
    
    // Fallback if graphics don't load
    if (!knobsLoaded) {
        DBG("Knob graphics failed to load! Using fallback rendering.");
    }
    
    if (!backgroundLoaded) {
        DBG("Background graphics failed to load! Using fallback rendering.");
    }
}

void Successor37AudioProcessorEditor::paint(juce::Graphics& g)
{
    // Draw background using custom graphics or fallback
    if (customLookAndFeel.getBackgroundImage().isValid()) {
        g.drawImageWithin(customLookAndFeel.getBackgroundImage(),
                        0, 0, getWidth(), getHeight(),
                        juce::RectanglePlacement::fillDestination);
    } else {
        // Fallback gradient background
        juce::ColourGradient gradient(
            juce::Colour(0xff2c3e50), 0, 0,
            juce::Colour(0xff34495e), getWidth(), getHeight(),
            false
        );
        g.setGradientFill(gradient);
        g.fillAll();
    }
    
    // Draw section backgrounds
    drawSectionBackgrounds(g);
    
    // Draw title and dividers
    drawUIOverlays(g);
}

void Successor37AudioProcessorEditor::drawSectionBackgrounds(juce::Graphics& g)
{
    g.setColour(juce::Colour(0x5534495e)); // Semi-transparent dark blue
    
    // Section bounds
    const int sectionWidth = 240;
    const int padding = 10;
    
    juce::Rectangle<int> sections[] = {
        {padding, 40, sectionWidth, getHeight() - 50},
        {padding + sectionWidth, 40, sectionWidth, getHeight() - 50},
        {padding + sectionWidth * 2, 40, sectionWidth, getHeight() - 50},
        {padding + sectionWidth * 3, 40, sectionWidth - padding, getHeight() - 50}
    };
    
    for (const auto& section : sections) {
        g.fillRoundedRectangle(section.toFloat(), 8.0f);
        g.setColour(juce::Colour(0x44ffffff));
        g.drawRoundedRectangle(section.toFloat(), 8.0f, 1.5f);
    }
}

void Successor37AudioProcessorEditor::drawUIOverlays(juce::Graphics& g)
{
    // Draw title
    g.setColour(juce::Colour(0xffecf0f1));
    g.setFont(juce::Font("Bank Gothic", 28.0f, juce::Font::bold));
    g.drawText("SUCCESSOR 37", 0, 5, getWidth(), 30, 
               juce::Justification::centred, true);
    
    // Draw section dividers
    g.setColour(juce::Colour(0x88ffffff));
    g.drawLine(250, 50, 250, getHeight() - 10, 2);
    g.drawLine(495, 50, 495, getHeight() - 10, 2);
    g.drawLine(740, 50, 740, getHeight() - 10, 2);
    
    // Draw section headers
    g.setFont(juce::Font("Bank Gothic", 16.0f, juce::Font::bold));
    g.drawText("OSCILLATORS", 20, 45, 220, 20, juce::Justification::centred, true);
    g.drawText("FILTER", 270, 45, 220, 20, juce::Justification::centred, true);
    g.drawText("ENVELOPES", 515, 45, 220, 20, juce::Justification::centred, true);
    g.drawText("MODULATION", 760, 45, 220, 20, juce::Justification::centred, true);
}

void Successor37AudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(15);
    
    // Section dimensions
    const int sectionWidth = 240;
    const int sectionPadding = 10;
    const int knobSize = 70;
    const int smallKnobSize = 50;
    const int labelHeight = 20;
    const int sectionHeaderHeight = 30;
    
    // ==================== OSCILLATOR SECTION ====================
    auto oscSection = area.removeFromLeft(sectionWidth);
    
    // Oscillator section header
    oscSectionLabel.setBounds(oscSection.getX(), 15, sectionWidth, sectionHeaderHeight);
    oscSection.removeFromTop(sectionHeaderHeight + 10);
    
    // Oscillator 1 row
    auto osc1Row = oscSection.removeFromTop(100);
    oscWaveformSelector.setBounds(osc1Row.removeFromLeft(100).reduced(5));
    oscWaveformSelector.setSize(100, 30);
    oscTuneSlider.setBounds(osc1Row.removeFromLeft(knobSize).reduced(5));
    oscTuneLabel.setBounds(oscTuneSlider.getX(), oscTuneSlider.getBottom() + 2, knobSize, labelHeight);
    
    // Oscillator 2 row
    auto osc2Row = oscSection.removeFromTop(100);
    auto osc2LabelArea = osc2Row.removeFromLeft(100);
    oscPWMSlider.setBounds(osc2Row.removeFromLeft(knobSize).reduced(5));
    oscPWMLabel.setBounds(oscPWMSlider.getX(), oscPWMSlider.getBottom() + 2, knobSize, labelHeight);
    
    // Noise and Mix row
    auto noiseMixRow = oscSection.removeFromTop(100);
    auto noiseSlider = noiseMixRow.removeFromLeft(knobSize);
    auto mixSlider = noiseMixRow.removeFromLeft(knobSize);
    
    // ==================== FILTER SECTION ====================
    auto filterSection = area.removeFromLeft(sectionWidth);
    
    // Filter section header
    filterSectionLabel.setBounds(filterSection.getX(), 15, sectionWidth, sectionHeaderHeight);
    filterSection.removeFromTop(sectionHeaderHeight + 10);
    
    // Large cutoff knob (centerpiece)
    auto cutoffRow = filterSection.removeFromTop(120);
    filterCutoffSlider.setBounds(cutoffRow.getCentreX() - knobSize/2, cutoffRow.getY(), knobSize + 20, knobSize + 20);
    filterCutoffLabel.setBounds(filterCutoffSlider.getX(), filterCutoffSlider.getBottom() + 2, knobSize + 20, labelHeight);
    
    // Resonance and Drive row
    auto resDriveRow = filterSection.removeFromTop(90);
    filterResonanceSlider.setBounds(resDriveRow.removeFromLeft(knobSize).reduced(5));
    filterResonanceLabel.setBounds(filterResonanceSlider.getX(), filterResonanceSlider.getBottom() + 2, knobSize, labelHeight);
    
    filterDriveSlider.setBounds(resDriveRow.removeFromLeft(knobSize).reduced(5));
    filterDriveLabel.setBounds(filterDriveSlider.getX(), filterDriveSlider.getBottom() + 2, knobSize, labelHeight);
    
    // Env Amount and Keyboard Tracking row
    auto envTrackRow = filterSection.removeFromTop(90);
    filterEnvAmountSlider.setBounds(envTrackRow.removeFromLeft(knobSize).reduced(5));
    filterEnvAmountLabel.setBounds(filterEnvAmountSlider.getX(), filterEnvAmountSlider.getBottom() + 2, knobSize, labelHeight);
    
    // ==================== ENVELOPE SECTION ====================
    auto envSection = area.removeFromLeft(sectionWidth);
    
    // Envelope section header
    envSectionLabel.setBounds(envSection.getX(), 15, sectionWidth, sectionHeaderHeight);
    envSection.removeFromTop(sectionHeaderHeight + 10);
    
    // Split into two columns: Amp Env and Filter Env
    auto ampEnvColumn = envSection.removeFromLeft(sectionWidth / 2);
    auto filterEnvColumn = envSection;
    
    // Amp Envelope column
    ampEnvColumn.removeFromTop(10);
    juce::Label ampEnvLabel("Amp Env", "AMP ENV");
    ampEnvLabel.setBounds(ampEnvColumn.getX(), ampEnvColumn.getY() - 20, ampEnvColumn.getWidth(), 20);
    ampEnvLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(ampEnvLabel);
    
    attackSlider.setBounds(ampEnvColumn.removeFromTop(smallKnobSize + 25).reduced(2));
    attackLabel.setBounds(attackSlider.getX(), attackSlider.getBottom() + 2, smallKnobSize, labelHeight);
    
    decaySlider.setBounds(ampEnvColumn.removeFromTop(smallKnobSize + 25).reduced(2));
    decayLabel.setBounds(decaySlider.getX(), decaySlider.getBottom() + 2, smallKnobSize, labelHeight);
    
    sustainSlider.setBounds(ampEnvColumn.removeFromTop(smallKnobSize + 25).reduced(2));
    sustainLabel.setBounds(sustainSlider.getX(), sustainSlider.getBottom() + 2, smallKnobSize, labelHeight);
    
    releaseSlider.setBounds(ampEnvColumn.removeFromTop(smallKnobSize + 25).reduced(2));
    releaseLabel.setBounds(releaseSlider.getX(), releaseSlider.getBottom() + 2, smallKnobSize, labelHeight);
    
    // Filter Envelope column
    filterEnvColumn.removeFromTop(10);
    juce::Label filterEnvLabel("Filter Env", "FILTER ENV");
    filterEnvLabel.setBounds(filterEnvColumn.getX(), filterEnvColumn.getY() - 20, filterEnvColumn.getWidth(), 20);
    filterEnvLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(filterEnvLabel);
    
    filterAttackSlider.setBounds(filterEnvColumn.removeFromTop(smallKnobSize + 25).reduced(2));
    filterAttackLabel.setBounds(filterAttackSlider.getX(), filterAttackSlider.getBottom() + 2, smallKnobSize, labelHeight);
    
    filterDecaySlider.setBounds(filterEnvColumn.removeFromTop(smallKnobSize + 25).reduced(2));
    filterDecayLabel.setBounds(filterDecaySlider.getX(), filterDecaySlider.getBottom() + 2, smallKnobSize, labelHeight);
    
    filterSustainSlider.setBounds(filterEnvColumn.removeFromTop(smallKnobSize + 25).reduced(2));
    filterSustainLabel.setBounds(filterSustainSlider.getX(), filterSustainSlider.getBottom() + 2, smallKnobSize, labelHeight);
    
    filterReleaseSlider.setBounds(filterEnvColumn.removeFromTop(smallKnobSize + 25).reduced(2));
    filterReleaseLabel.setBounds(filterReleaseSlider.getX(), filterReleaseSlider.getBottom() + 2, smallKnobSize, labelHeight);
    
    // ==================== MODULATION SECTION ====================
    auto modSection = area;
    
    // Modulation section header
    lfoSectionLabel.setBounds(modSection.getX(), 15, modSection.getWidth(), sectionHeaderHeight);
    modSection.removeFromTop(sectionHeaderHeight + 10);
    
    // LFO 1 Section
    auto lfo1Area = modSection.removeFromTop(120);
    juce::Label lfo1Label("LFO1", "LFO 1");
    lfo1Label.setBounds(lfo1Area.getX(), lfo1Area.getY() - 20, lfo1Area.getWidth(), 20);
    lfo1Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lfo1Label);
    
    lfo1RateSlider.setBounds(lfo1Area.removeFromLeft(knobSize).reduced(5));
    lfo1RateLabel.setBounds(lfo1RateSlider.getX(), lfo1RateSlider.getBottom() + 2, knobSize, labelHeight);
    
    lfo1AmountSlider.setBounds(lfo1Area.removeFromLeft(knobSize).reduced(5));
    lfo1AmountLabel.setBounds(lfo1AmountSlider.getX(), lfo1AmountSlider.getBottom() + 2, knobSize, labelHeight);
    
    // LFO 2 Section
    auto lfo2Area = modSection.removeFromTop(120);
    juce::Label lfo2Label("LFO2", "LFO 2");
    lfo2Label.setBounds(lfo2Area.getX(), lfo2Area.getY() - 20, lfo2Area.getWidth(), 20);
    lfo2Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(lfo2Label);
    
    // Modulation Sources
    auto modSourcesArea = modSection.removeFromTop(120);
    juce::Label modSourcesLabel("ModSources", "MOD SOURCES");
    modSourcesLabel.setBounds(modSourcesArea.getX(), modSourcesArea.getY() - 20, modSourcesArea.getWidth(), 20);
    modSourcesLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(modSourcesLabel);
    
    modWheelAmountSlider.setBounds(modSourcesArea.removeFromLeft(knobSize).reduced(5));
    modWheelLabel.setBounds(modWheelAmountSlider.getX(), modWheelAmountSlider.getBottom() + 2, knobSize, labelHeight);
    
    velocityAmountSlider.setBounds(modSourcesArea.removeFromLeft(knobSize).reduced(5));
    velocityLabel.setBounds(velocityAmountSlider.getX(), velocityAmountSlider.getBottom() + 2, knobSize, labelHeight);
    
    // Master Output
    auto masterArea = modSection.removeFromTop(120);
    juce::Label masterLabel("Master", "MASTER");
    masterLabel.setBounds(masterArea.getX(), masterArea.getY() - 20, masterArea.getWidth(), 20);
    masterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(masterLabel);
    
    masterVolumeSlider.setBounds(masterArea.getCentreX() - knobSize/2, masterArea.getY(), knobSize, knobSize);
    masterVolumeLabel.setBounds(masterVolumeSlider.getX(), masterVolumeSlider.getBottom() + 2, knobSize, labelHeight);
}

//==============================================================================
void Successor37AudioProcessorEditor::createSlider(juce::Slider& slider, juce::Label& label, 
                                                  const juce::String& name, 
                                                  juce::AudioProcessorValueTreeState& apvts, 
                                                  const juce::String& paramID)
{
    // Configure slider
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff39c12));
    slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x6634495e));
    slider.setColour(juce::Slider::trackColourId, juce::Colour(0x00ffffff)); // Transparent track

    // Configure label
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredTop);
    label.setColour(juce::Label::textColourId, juce::Colour(0xffecf0f1));
    label.setFont(juce::Font(12.0f, juce::Font::bold));

    // Add to UI
    addAndMakeVisible(slider);
    addAndMakeVisible(label);

    // Create parameter attachment
    if (apvts.getParameter(paramID) != nullptr) {
        if (paramID == "filterCutoff") {
            filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "filterResonance") {
            filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "filterDrive") {
            filterDriveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "filterEnvAmount") {
            filterEnvAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "attack") {
            attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "decay") {
            decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "sustain") {
            sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "release") {
            releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "lfo1Rate") {
            lfo1RateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "lfo1Amount") {
            lfo1AmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "modWheelToFilter") {
            modWheelAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "velocityToFilter") {
            velocityAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        } else if (paramID == "masterVolume") {
            masterVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);
        }
        // Add more parameter attachments as needed
    }
}

void Successor37AudioProcessorEditor::createComboBox(juce::ComboBox& comboBox, const juce::String& name,
                                                    juce::AudioProcessorValueTreeState& apvts, 
                                                    const juce::String& paramID)
{
    comboBox.addItem("Sine", 1);
    comboBox.addItem("Saw", 2);
    comboBox.addItem("Square", 3);
    comboBox.addItem("Triangle", 4);
    
    addAndMakeVisible(comboBox);
    
    if (paramID == "oscWaveform") {
        oscWaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, paramID, comboBox);
    }
    // ... add more combo box attachments
}