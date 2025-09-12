#include <juce_dsp/juce_dsp.h>

void optimizedProcess(juce::AudioBuffer<float>& buffer)
{
    // Use JUCE's SIMD types for vectorized processing
    using SIMDType = juce::dsp::SIMDRegister<float>;
    const int numSamples = buffer.getNumSamples();
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        float* data = buffer.getWritePointer(channel);
        int i = 0;
        
        // Process in SIMD blocks
        for (; i < numSamples - SIMDType::size(); i += SIMDType::size()) {
            SIMDType block = SIMDType::fromRawArray(data + i);
            // Your SIMD processing here
            block = block * SIMDType(0.5f); // Example: gain reduction
            block.copyToRawArray(data + i);
        }
        
        // Process remaining samples
        for (; i < numSamples; ++i) {
            data[i] *= 0.5f;
        }
    }
}
#include "DelayEffect.h"
#include "ChorusEffect.h"
#include "Arpeggiator.h"
#include "PresetManager.h"
// Effect and utility members
DelayEffect delay;
ChorusEffect chorus;
Arpeggiator arpeggiator;
std::unique_ptr<PresetManager> presetManager;
double currentBPM = 120.0;
bool isPlaying = false;
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <algorithm>
#include <cmath>



Successor37AudioProcessor::Successor37AudioProcessor()
        // In PluginProcessor.cpp
        juce::AudioProcessorEditor* Successor37AudioProcessor::createEditor()
        {
            return new Successor37AudioProcessorEditor(*this);
        }

        bool Successor37AudioProcessor::hasEditor() const
        {
            return true;
        }
            // Get parameter references
            oscWaveformParam = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("oscWaveform"));
            oscTuneParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("oscTune"));
            filterCutoffParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterCutoff"));
            filterResonanceParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterResonance"));
            filterDriveParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("filterDrive"));
            attackParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("attack"));
            decayParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("decay"));
            sustainParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("sustain"));
            releaseParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("release"));
            masterVolumeParam = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("masterVolume"));

            // Create and add the synth sound
            synthSound = std::make_unique<SynthSound>();
            synth.addSound(synthSound.get());

            // Add 8 voices for polyphony
            for (int i = 0; i < 8; ++i) {
                synth.addVoice(new SynthVoice());
            }
        }

        Successor37AudioProcessor::~Successor37AudioProcessor() {
            // Clean up - JUCE Synthesiser will handle voice deletion
        }

        //==============================================================================
        void Successor37AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
            // Set sample rate for all voices
            for (int i = 0; i < synth.getNumVoices(); ++i) {
                if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
                    voice->setSampleRate(sampleRate);
                }
            }
            // Optional: Set processing preferences
            synth.setCurrentPlaybackSampleRate(sampleRate);

            // Prepare effects
            delay.prepare(sampleRate, samplesPerBlock);
            chorus.prepare(sampleRate, samplesPerBlock);
            arpeggiator.prepare(sampleRate);

            // Initialize preset manager
            presetManager = std::make_unique<PresetManager>(apvts);
        }

        void Successor37AudioProcessor::releaseResources() {
            // Clean up resources if needed
            // JUCE Synthesiser handles its own cleanup
        }

        void Successor37AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
        {
            updateHostInfo();

            // Process arpeggiator
            juce::MidiBuffer processedMidi;
            arpeggiator.process(midiMessages, processedMidi, buffer.getNumSamples());

            // Process synth
            synth.renderNextBlock(buffer, processedMidi, 0, buffer.getNumSamples());

            // Process effects
            delay.process(buffer);
            chorus.process(buffer);
        }
void Successor37AudioProcessor::updateHostInfo()
{
    auto playhead = getPlayHead();
    if (playhead != nullptr) {
        juce::AudioPlayHead::CurrentPositionInfo info;
        if (playhead->getCurrentPosition(info)) {
            currentBPM = info.bpm;
            isPlaying = info.isPlaying;

            // Update effects with host info
            delay.setHostInfo(currentBPM, isPlaying);
            arpeggiator.setHostInfo(currentBPM, isPlaying);
        }
    }
}

        //==============================================================================
        void Successor37AudioProcessor::updateParameters() {
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            // Oscillator parameters
            voice->setOscillatorWaveform(oscWaveformParam->getIndex());
            voice->setOscillatorTune(oscTuneParam->get());

            // Filter parameters
            voice->setFilterCutoff(filterCutoffParam->get());
            voice->setFilterResonance(filterResonanceParam->get());
            voice->setFilterDrive(filterDriveParam->get());

            // Envelope parameters
            voice->setAttackTime(attackParam->get());
            voice->setDecayTime(decayParam->get());
            voice->setSustainLevel(sustainParam->get());
            voice->setReleaseTime(releaseParam->get());

            // LFO parameters
            voice->setLFO1Rate(*parameters.getRawParameterValue("lfo1Rate"));
            voice->setLFO1Waveform(static_cast<int>(*parameters.getRawParameterValue("lfo1Waveform")));
            voice->setLFO1ToFilterAmount(*parameters.getRawParameterValue("lfo1ToFilter"));
            voice->setLFO1ToPitchAmount(*parameters.getRawParameterValue("lfo1ToPitch"));

            // Modulation parameters
            voice->setFilterEnvAmount(*parameters.getRawParameterValue("filterEnvAmount"));
            voice->setModWheelToFilterAmount(*parameters.getRawParameterValue("modWheelToFilter"));
            voice->setVelocityToFilterAmount(*parameters.getRawParameterValue("velocityToFilter"));
            voice->setVelocityToAmpAmount(*parameters.getRawParameterValue("velocityToAmp"));
        }
    }
        }

        //==============================================================================
        // Required AudioProcessor implementations (can be mostly boilerplate)
        //==============================================================================

        juce::AudioProcessorEditor* Successor37AudioProcessor::createEditor() {
            return new juce::GenericAudioProcessorEditor(*this); // Simple editor for now
        }

        bool Successor37AudioProcessor::hasEditor() const {
            return true;
        }

        const juce::String Successor37AudioProcessor::getName() const {
            return JucePlugin_Name;
        }

        bool Successor37AudioProcessor::acceptsMidi() const {
            return true;
        }

        bool Successor37AudioProcessor::producesMidi() const {
            return false;
        }

        bool Successor37AudioProcessor::isMidiEffect() const {
            return false;
        }

        double Successor37AudioProcessor::getTailLengthSeconds() const {
            return 0.0;
        }

        int Successor37AudioProcessor::getNumPrograms() {
            return 1;
        }

        int Successor37AudioProcessor::getCurrentProgram() {
            return 0;
        }

        void Successor37AudioProcessor::setCurrentProgram(int index) {
            juce::ignoreUnused(index);
        }

        const juce::String Successor37AudioProcessor::getProgramName(int index) {
            juce::ignoreUnused(index);
            return {};
        }

        void Successor37AudioProcessor::changeProgramName(int index, const juce::String& newName) {
            juce::ignoreUnused(index, newName);
        }

        void Successor37AudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
            // Save plugin state
            auto state = parameters.copyState();
            std::unique_ptr<juce::XmlElement> xml(state.createXml());
            copyXmlToBinary(*xml, destData);
        }

        void Successor37AudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
            // Load plugin state
            std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
            if (xmlState.get() != nullptr) {
                if (xmlState->hasTagName(parameters.state.getType())) {
                    parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
                }
            }
        }

        //==============================================================================
        // Parameter layout function (from previous steps)
        juce::AudioProcessorValueTreeState::ParameterLayout Successor37AudioProcessor::createParameterLayout() {
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
    
            // Envelope Parameters
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                "attack", "Attack",
                juce::NormalisableRange<float>(1.0f, 2000.0f, 0.1f, 0.5f),
                50.0f
            ));
    
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                "decay", "Decay",
                juce::NormalisableRange<float>(1.0f, 2000.0f, 0.1f, 0.5f),
                100.0f
            ));
    
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                "sustain", "Sustain",
                juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
                0.7f
            ));
    
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                "release", "Release",
                juce::NormalisableRange<float>(1.0f, 5000.0f, 0.1f, 0.5f),
                200.0f
            ));
    
            // Output Parameters
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                "masterVolume", "Master Volume",
                juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
                0.7f
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

            return { params.begin(), params.end() };
        }

        //==============================================================================
        // This creates new instances of the plugin
        juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
            return new Successor37AudioProcessor();
        }