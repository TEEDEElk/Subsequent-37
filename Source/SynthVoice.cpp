// SynthVoice.cpp
#include "SynthVoice.h"
#include "SynthSound.h"
#include <cmath>

SynthVoice::SynthVoice()
{
    // Setup modulation matrix connections
    modMatrix.setLFO1(&lfo1);
    modMatrix.setLFO2(&lfo2);
    modMatrix.setFilterEnv(&filterEnvelope);
    modMatrix.setAmpEnv(&ampEnvelope);

    // Set default envelope times
    setAmpAttackTime(50.0f);
    setAmpDecayTime(100.0f);
    setAmpSustainLevel(0.7f);
    setAmpReleaseTime(200.0f);

    setFilterAttackTime(50.0f);
    setFilterDecayTime(100.0f);
    setFilterSustainLevel(0.0f);
    setFilterReleaseTime(200.0f);

    // Set LFO defaults
    setLFO1Rate(1.0f);
    setLFO1Waveform(LFO::Sine);
    setLFO2Rate(0.5f);
    setLFO2Waveform(LFO::Triangle);

    // Initial modulation connections
    updateModulationConnections();
}

//==============================================================================
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, 
                          juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    juce::ignoreUnused(sound);
    
    currentNoteNumber = midiNoteNumber;
    currentVelocity = velocity;
    isNotePlaying = true;
    isTailOff = false;

    // Convert pitch wheel to bend amount (-2 to +2 semitones)
    currentPitchBend = (currentPitchWheelPosition - 8192) / 8192.0f * 2.0f;

    // Calculate frequency
    float frequency = calculateFrequency(midiNoteNumber, currentPitchBend);
    oscillator.setFrequency(frequency);

    // Trigger envelopes
    ampEnvelope.noteOn();
    filterEnvelope.noteOn();

    // Reset LFOs
    lfo1.reset();
    lfo2.reset();

    // Update modulation matrix with velocity
    modMatrix.setVelocity(velocity);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    juce::ignoreUnused(velocity);
    
    isNotePlaying = false;

    if (allowTailOff)
    {
        isTailOff = true;
        ampEnvelope.noteOff();
        filterEnvelope.noteOff();
    }
    else
    {
        clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    // Convert to semitones (-2 to +2)
    currentPitchBend = (newPitchWheelValue - 8192) / 8192.0f * 2.0f;
    applyPitchBend();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    float normalizedValue = newControllerValue / 127.0f;

    switch (controllerNumber)
    {
        case 1: // Mod Wheel
            currentModWheel = normalizedValue;
            modMatrix.setModWheel(normalizedValue);
            break;
        case 64: // Sustain pedal - handled by JUCE
            break;
        default:
            break;
    }
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, 
                                int startSample, int numSamples)
{
    if (!isVoiceActive())
        return;

    // Process each sample
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Generate oscillator output
        float oscOutput = oscillator.process();

        // Get envelope values
        float ampEnvValue = ampEnvelope.getNextSample();
        float filterEnvValue = filterEnvelope.getNextSample();

        // Get LFO values
        float lfo1Value = lfo1.getNextSample();
        float lfo2Value = lfo2.getNextSample();

        // Calculate modulated filter cutoff
        float modulatedCutoff = baseFilterCutoff;
        modulatedCutoff += filterEnvValue * filterEnvAmount * 5000.0f;
        modulatedCutoff += lfo1Value * lfo1ToFilterAmount * 3000.0f;
        modulatedCutoff += lfo2Value * lfo2ToFilterAmount * 3000.0f;
        modulatedCutoff += currentModWheel * modWheelToFilterAmount * 5000.0f;
        modulatedCutoff += currentVelocity * velocityToFilterAmount * 3000.0f;
        modulatedCutoff = juce::jlimit(20.0f, 20000.0f, modulatedCutoff);

        // Apply filter
        filter.setCutoff(modulatedCutoff);
        float filteredOutput = filter.processSample(oscOutput);

        // Apply amplitude envelope and velocity
        float velocityScale = 1.0f - velocityToAmpAmount + (currentVelocity * velocityToAmpAmount);
        float finalOutput = filteredOutput * ampEnvValue * velocityScale * masterVolume;

        // Mix into output buffer (stereo)
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, startSample + sample, finalOutput);
        }
    }

    // Check if voice should be released
    if (isTailOff && !ampEnvelope.isActive())
    {
        clearCurrentNote();
    }
}

//==============================================================================
// Parameter setters
void SynthVoice::setSampleRate(double newSampleRate)
{
    oscillator.setSampleRate(static_cast<float>(newSampleRate));
    ampEnvelope.setSampleRate(static_cast<float>(newSampleRate));
    filterEnvelope.setSampleRate(static_cast<float>(newSampleRate));
    filter.setSampleRate(static_cast<float>(newSampleRate));
    lfo1.setSampleRate(newSampleRate);
    lfo2.setSampleRate(newSampleRate);
}

void SynthVoice::setOscillatorWaveform(int waveform)
{
    oscillator.setWaveform(waveform);
}

void SynthVoice::setOscillatorTune(float tuneSemitones)
{
    baseOscTune = tuneSemitones;
    if (currentNoteNumber >= 0)
    {
        float frequency = calculateFrequency(currentNoteNumber, currentPitchBend + tuneSemitones);
        oscillator.setFrequency(frequency);
    }
}

void SynthVoice::setOscillatorPWM(float pwm)
{
    baseOscPWM = pwm;
    // PWM implementation would go here if oscillator supports it
}

void SynthVoice::setFilterCutoff(float cutoffHz)
{
    baseFilterCutoff = cutoffHz;
}

void SynthVoice::setFilterResonance(float resonance)
{
    filter.setResonance(resonance);
}

void SynthVoice::setFilterDrive(float drive)
{
    filter.setDrive(drive);
}

void SynthVoice::setFilterEnvAmount(float amount)
{
    filterEnvAmount = amount;
}

void SynthVoice::setAmpAttackTime(float attackMs)
{
    ampEnvelope.setAttack(attackMs);
}

void SynthVoice::setAmpDecayTime(float decayMs)
{
    ampEnvelope.setDecay(decayMs);
}

void SynthVoice::setAmpSustainLevel(float sustain)
{
    ampEnvelope.setSustain(sustain);
}

void SynthVoice::setAmpReleaseTime(float releaseMs)
{
    ampEnvelope.setRelease(releaseMs);
}

void SynthVoice::setFilterAttackTime(float attackMs)
{
    filterEnvelope.setAttack(attackMs);
}

void SynthVoice::setFilterDecayTime(float decayMs)
{
    filterEnvelope.setDecay(decayMs);
}

void SynthVoice::setFilterSustainLevel(float sustain)
{
    filterEnvelope.setSustain(sustain);
}

void SynthVoice::setFilterReleaseTime(float releaseMs)
{
    filterEnvelope.setRelease(releaseMs);
}

void SynthVoice::setLFO1Rate(float rateHz)
{
    lfo1.setRate(rateHz);
}

void SynthVoice::setLFO1Waveform(int waveform)
{
    lfo1.setWaveform(waveform);
}

void SynthVoice::setLFO1ToFilterAmount(float amount)
{
    lfo1ToFilterAmount = amount;
}

void SynthVoice::setLFO1ToPitchAmount(float amount)
{
    lfo1ToPitchAmount = amount;
}

void SynthVoice::setLFO1ToPWMAmount(float amount)
{
    lfo1ToPWMAmount = amount;
}

void SynthVoice::setLFO2Rate(float rateHz)
{
    lfo2.setRate(rateHz);
}

void SynthVoice::setLFO2Waveform(int waveform)
{
    lfo2.setWaveform(waveform);
}

void SynthVoice::setLFO2ToFilterAmount(float amount)
{
    lfo2ToFilterAmount = amount;
}

void SynthVoice::setLFO2ToPitchAmount(float amount)
{
    lfo2ToPitchAmount = amount;
}

void SynthVoice::setModWheelToFilterAmount(float amount)
{
    modWheelToFilterAmount = amount;
}

void SynthVoice::setVelocityToFilterAmount(float amount)
{
    velocityToFilterAmount = amount;
}

void SynthVoice::setVelocityToAmpAmount(float amount)
{
    velocityToAmpAmount = amount;
}

void SynthVoice::setMasterVolume(float volume)
{
    masterVolume = volume;
}

//==============================================================================
// State queries
bool SynthVoice::isActive() const
{
    return isNotePlaying || ampEnvelope.isActive();
}

float SynthVoice::getLFO1Value() const
{
    return lfo1.getCurrentPhase();
}

float SynthVoice::getLFO2Value() const
{
    return lfo2.getCurrentPhase();
}

//==============================================================================
// Helper functions
float SynthVoice::calculateFrequency(int midiNoteNumber, float pitchBendSemitones) const
{
    // MIDI note to frequency conversion: f = 440 * 2^((n-69)/12)
    float totalSemitones = midiNoteNumber + pitchBendSemitones + baseOscTune;
    return 440.0f * std::pow(2.0f, (totalSemitones - 69.0f) / 12.0f);
}

void SynthVoice::applyPitchBend()
{
    if (currentNoteNumber >= 0)
    {
        float frequency = calculateFrequency(currentNoteNumber, currentPitchBend);
        oscillator.setFrequency(frequency);
    }
}

void SynthVoice::updateModulationConnections()
{
    // This could set up routing in the modulation matrix
    // For now, modulation is handled directly in renderNextBlock
}

void SynthVoice::updateParametersFromModMatrix()
{
    // Get modulated values from modulation matrix
    // This is a placeholder for future modulation matrix integration
}
