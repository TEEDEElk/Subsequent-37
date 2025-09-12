// SynthVoice.cpp

#include <vector>
#include "SynthVoice.h"
#include "SynthSound.h"

SynthVoice::SynthVoice() {
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
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    // This voice can only play SynthSound objects
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    currentNoteNumber = midiNoteNumber;
    currentVelocity = velocity;
    isNotePlaying = true;
    // ...existing code for starting a note...
}

// ...existing parameter setters and helper functions...

// ...existing parameter setters and helper functions...