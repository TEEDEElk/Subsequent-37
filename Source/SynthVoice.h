// SynthVoice.h
#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"
#include "ADSREnvelope.h"
#include "MoogFilter.h"
#include "LFO.h"
#include "ModulationMatrix.h"

class SynthVoice : public juce::SynthesiserVoice {
public:
    SynthVoice();
    
    //==============================================================================
    // juce::SynthesiserVoice overrides
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
    //==============================================================================
    // Parameter setters
    void setSampleRate(double newSampleRate);
    
    // Oscillator parameters
    void setOscillatorWaveform(int waveform);
    void setOscillatorTune(float tuneSemitones);
    void setOscillatorPWM(float pwm);
    
    // Filter parameters
    void setFilterCutoff(float cutoffHz);
    void setFilterResonance(float resonance);
    void setFilterDrive(float drive);
    void setFilterEnvAmount(float amount);
    
    // Envelope parameters
    void setAmpAttackTime(float attackMs);
    void setAmpDecayTime(float decayMs);
    void setAmpSustainLevel(float sustain);
    void setAmpReleaseTime(float releaseMs);
    
    void setFilterAttackTime(float attackMs);
    void setFilterDecayTime(float decayMs);
    void setFilterSustainLevel(float sustain);
    void setFilterReleaseTime(float releaseMs);
    
    // LFO parameters
    void setLFO1Rate(float rateHz);
    void setLFO1Waveform(int waveform);
    void setLFO1ToFilterAmount(float amount);
    void setLFO1ToPitchAmount(float amount);
    void setLFO1ToPWMAmount(float amount);
    
    void setLFO2Rate(float rateHz);
    void setLFO2Waveform(int waveform);
    void setLFO2ToFilterAmount(float amount);
    void setLFO2ToPitchAmount(float amount);
    
    // Modulation parameters
    void setModWheelToFilterAmount(float amount);
    void setVelocityToFilterAmount(float amount);
    void setVelocityToAmpAmount(float amount);
    
    // Volume
    void setMasterVolume(float volume);
    
    //==============================================================================
    // State queries
    bool isActive() const;
    int getCurrentNote() const { return currentNoteNumber; }
    float getCurrentVelocity() const { return currentVelocity; }
    
    // Modulation matrix access
    ModulationMatrix& getModMatrix() { return modMatrix; }
    
    // LFO value getters for UI
    float getLFO1Value() const;
    float getLFO2Value() const;

private:
    // DSP Components
    Oscillator oscillator;
    ADSREnvelope ampEnvelope;
    ADSREnvelope filterEnvelope;
    MoogFilter filter;
    
    // Modulation sources
    LFO lfo1;
    LFO lfo2;
    ModulationMatrix modMatrix;
    
    // Current voice state
    int currentNoteNumber = -1;
    float currentVelocity = 0.0f;
    float currentPitchBend = 0.0f;
    float currentModWheel = 0.0f;
    float currentAftertouch = 0.0f;
    bool isNotePlaying = false;
    bool isTailOff = false;
    
    // Base parameter values
    float baseFilterCutoff = 1000.0f;
    float baseOscTune = 0.0f;
    float baseOscPWM = 0.5f;
    float masterVolume = 0.7f;
    
    // Modulation amounts
    float lfo1ToFilterAmount = 0.0f;
    float lfo1ToPitchAmount = 0.0f;
    float lfo1ToPWMAmount = 0.0f;
    float lfo2ToFilterAmount = 0.0f;
    float lfo2ToPitchAmount = 0.0f;
    float modWheelToFilterAmount = 0.0f;
    float velocityToFilterAmount = 0.0f;
    float velocityToAmpAmount = 1.0f; // Default to full velocity sensitivity
    float filterEnvAmount = 0.5f;
    
    // Helper functions
    float calculateFrequency(int midiNoteNumber, float pitchBend = 0.0f) const;
    void applyPitchBend();
    void updateModulationConnections();
    void updateParametersFromModMatrix();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthVoice)
};                                                                                      