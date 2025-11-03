// PluginProcessor.cpp
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Successor37AudioProcessor::Successor37AudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "Parameters", createParameterLayout())
{
    // Get parameter references
    oscWaveformParam = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("oscWaveform"));
    oscTuneParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("oscTune"));
    oscPWMParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("oscPWM"));
    filterCutoffParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterCutoff"));
    filterResonanceParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterResonance"));
    filterDriveParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterDrive"));
    filterEnvAmountParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterEnvAmount"));
    ampAttackParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("ampAttack"));
    ampDecayParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("ampDecay"));
    ampSustainParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("ampSustain"));
    ampReleaseParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("ampRelease"));
    filterAttackParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterAttack"));
    filterDecayParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterDecay"));
    filterSustainParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterSustain"));
    filterReleaseParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterRelease"));
    masterVolumeParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("masterVolume"));

    // Create and add the synth sound
    synthSound = std::make_unique<SynthSound>();
    synth.addSound(synthSound.get());

    // Add 8 voices for polyphony
    for (int i = 0; i < 8; ++i) {
        synth.addVoice(new SynthVoice());
    }
}

Successor37AudioProcessor::~Successor37AudioProcessor()
{
}

//==============================================================================
const juce::String Successor37AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Successor37AudioProcessor::acceptsMidi() const
{
    return true;
}

bool Successor37AudioProcessor::producesMidi() const
{
    return false;
}

bool Successor37AudioProcessor::isMidiEffect() const
{
    return false;
}

double Successor37AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Successor37AudioProcessor::getNumPrograms()
{
    return 1;
}

int Successor37AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Successor37AudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String Successor37AudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void Successor37AudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void Successor37AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Set sample rate for all voices
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->setSampleRate(sampleRate);
        }
    }
    
    synth.setCurrentPlaybackSampleRate(sampleRate);

    // Prepare effects
    delay.prepare(sampleRate, samplesPerBlock);
    chorus.prepare(sampleRate, samplesPerBlock);
    arpeggiator.prepare(sampleRate);

    // Initialize preset manager
    presetManager = std::make_unique<PresetManager>(parameters);
}

void Successor37AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool Successor37AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // This checks if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void Successor37AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Update host info
    updateHostInfo();

    // Update voice parameters
    updateParameters();

    // Process arpeggiator
    juce::MidiBuffer processedMidi;
    arpeggiator.process(midiMessages, processedMidi, buffer.getNumSamples());

    // Process synth
    synth.renderNextBlock(buffer, processedMidi, 0, buffer.getNumSamples());

    // Process effects
    delay.process(buffer);
    chorus.process(buffer);
}

//==============================================================================
void Successor37AudioProcessor::updateHostInfo()
{
    auto playhead = getPlayHead();
    if (playhead != nullptr)
    {
        juce::Optional<juce::AudioPlayHead::PositionInfo> positionInfo = playhead->getPosition();
        if (positionInfo.hasValue())
        {
            auto bpm = positionInfo->getBpm();
            if (bpm.hasValue())
                currentBPM = *bpm;
            
            auto playing = positionInfo->getIsPlaying();
            isPlaying = playing;

            // Update effects with host info
            delay.setHostInfo(currentBPM, isPlaying);
            arpeggiator.setHostInfo(currentBPM, isPlaying);
        }
    }
}

void Successor37AudioProcessor::updateParameters()
{
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // Oscillator parameters
            if (oscWaveformParam)
                voice->setOscillatorWaveform(oscWaveformParam->getIndex());
            if (oscTuneParam)
                voice->setOscillatorTune(oscTuneParam->get());
            if (oscPWMParam)
                voice->setOscillatorPWM(oscPWMParam->get());

            // Filter parameters
            if (filterCutoffParam)
                voice->setFilterCutoff(filterCutoffParam->get());
            if (filterResonanceParam)
                voice->setFilterResonance(filterResonanceParam->get());
            if (filterDriveParam)
                voice->setFilterDrive(filterDriveParam->get());
            if (filterEnvAmountParam)
                voice->setFilterEnvAmount(filterEnvAmountParam->get());

            // Amplitude envelope parameters
            if (ampAttackParam)
                voice->setAmpAttackTime(ampAttackParam->get());
            if (ampDecayParam)
                voice->setAmpDecayTime(ampDecayParam->get());
            if (ampSustainParam)
                voice->setAmpSustainLevel(ampSustainParam->get());
            if (ampReleaseParam)
                voice->setAmpReleaseTime(ampReleaseParam->get());

            // Filter envelope parameters
            if (filterAttackParam)
                voice->setFilterAttackTime(filterAttackParam->get());
            if (filterDecayParam)
                voice->setFilterDecayTime(filterDecayParam->get());
            if (filterSustainParam)
                voice->setFilterSustainLevel(filterSustainParam->get());
            if (filterReleaseParam)
                voice->setFilterReleaseTime(filterReleaseParam->get());

            // LFO parameters
            voice->setLFO1Rate(*parameters.getRawParameterValue("lfo1Rate"));
            voice->setLFO1Waveform(static_cast<int>(*parameters.getRawParameterValue("lfo1Waveform")));
            voice->setLFO1ToFilterAmount(*parameters.getRawParameterValue("lfo1ToFilter"));
            voice->setLFO1ToPitchAmount(*parameters.getRawParameterValue("lfo1ToPitch"));

            voice->setLFO2Rate(*parameters.getRawParameterValue("lfo2Rate"));
            voice->setLFO2Waveform(static_cast<int>(*parameters.getRawParameterValue("lfo2Waveform")));
            voice->setLFO2ToFilterAmount(*parameters.getRawParameterValue("lfo2ToFilter"));
            voice->setLFO2ToPitchAmount(*parameters.getRawParameterValue("lfo2ToPitch"));

            // Modulation parameters
            voice->setModWheelToFilterAmount(*parameters.getRawParameterValue("modWheelToFilter"));
            voice->setVelocityToFilterAmount(*parameters.getRawParameterValue("velocityToFilter"));
            voice->setVelocityToAmpAmount(*parameters.getRawParameterValue("velocityToAmp"));

            // Master volume
            if (masterVolumeParam)
                voice->setMasterVolume(masterVolumeParam->get());
        }
    }
}

//==============================================================================
bool Successor37AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Successor37AudioProcessor::createEditor()
{
    return new Successor37AudioProcessorEditor(*this);
}

//==============================================================================
void Successor37AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Successor37AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout Successor37AudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Oscillator Parameters
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "oscWaveform", "Oscillator Waveform",
        juce::StringArray{"Sine", "Saw", "Square", "Triangle"}, 1
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "oscTune", "Oscillator Tune",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "oscPWM", "Pulse Width Modulation",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f
    ));

    // Filter Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterCutoff", "Filter Cutoff",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 0.1f, 0.3f),
        1000.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterResonance", "Filter Resonance",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.1f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterDrive", "Filter Drive",
        juce::NormalisableRange<float>(0.1f, 5.0f, 0.01f),
        1.0f
    ));

    // Amplitude Envelope Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ampAttack", "Amp Attack",
        juce::NormalisableRange<float>(1.0f, 2000.0f, 0.1f, 0.5f),
        50.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ampDecay", "Amp Decay",
        juce::NormalisableRange<float>(1.0f, 2000.0f, 0.1f, 0.5f),
        100.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ampSustain", "Amp Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.7f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ampRelease", "Amp Release",
        juce::NormalisableRange<float>(1.0f, 5000.0f, 0.1f, 0.5f),
        200.0f
    ));

    // Filter Envelope Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterAttack", "Filter Attack",
        juce::NormalisableRange<float>(1.0f, 2000.0f, 0.1f, 0.5f),
        50.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterDecay", "Filter Decay",
        juce::NormalisableRange<float>(1.0f, 2000.0f, 0.1f, 0.5f),
        100.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterSustain", "Filter Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.7f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterRelease", "Filter Release",
        juce::NormalisableRange<float>(1.0f, 5000.0f, 0.1f, 0.5f),
        200.0f
    ));

    // LFO 1 Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfo1Rate", "LFO 1 Rate",
        juce::NormalisableRange<float>(0.01f, 20.0f, 0.01f, 0.3f),
        1.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "lfo1Waveform", "LFO 1 Waveform",
        juce::StringArray{"Sine", "Triangle", "Saw", "Square", "Random", "Smooth Random"},
        0
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfo1ToFilter", "LFO 1 > Filter",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfo1ToPitch", "LFO 1 > Pitch",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.0f
    ));

    // LFO 2 Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfo2Rate", "LFO 2 Rate",
        juce::NormalisableRange<float>(0.01f, 20.0f, 0.01f, 0.3f),
        1.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "lfo2Waveform", "LFO 2 Waveform",
        juce::StringArray{"Sine", "Triangle", "Saw", "Square", "Random", "Smooth Random"},
        0
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfo2ToFilter", "LFO 2 > Filter",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "lfo2ToPitch", "LFO 2 > Pitch",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.0f
    ));

    // Modulation Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "filterEnvAmount", "Filter Env > Cutoff",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "modWheelToFilter", "Mod Wheel > Filter",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "velocityToFilter", "Velocity > Filter",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "velocityToAmp", "Velocity > Amp",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        1.0f
    ));

    // Output Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "masterVolume", "Master Volume",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.7f
    ));

    return { params.begin(), params.end() };
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Successor37AudioProcessor();
}
