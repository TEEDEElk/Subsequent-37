// LFO.h
#pragma once

#include <JuceHeader.h>

class LFO {
public:
    LFO();
    
    // Core functions
    void setSampleRate(double sampleRate);
    void setFrequency(float frequencyHz);
    void setRate(float rateHz) { setFrequency(rateHz); } // Alias for consistency
    void setWaveform(int waveform);
    void setBipolar(bool isBipolar); // -1 to +1 or 0 to +1 output
    
    // Processing
    float getNextSample();
    void skipSamples(int numSamples); // For sample-accurate modulation
    
    // Parameters with JUCE-friendly types
    enum Waveform {
        Sine = 0,
        Triangle,
        Sawtooth,
        Square,
        Random,
        RandomSmooth,
        NumWaveforms
    };
    
    // State control
    void reset();
    void setPhase(float phase); // 0.0 to 1.0
    
    // Utility functions
    bool isBipolar() const { return bipolar; }
    float getCurrentPhase() const { return phase; }
    
private:
    // Waveform generation functions
    float generateSine();
    float generateTriangle();
    float generateSawtooth();
    float generateSquare();
    float generateRandom();
    float generateRandomSmooth();
    
    // State variables
    double sampleRate = 44100.0;
    float frequency = 1.0f;
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    int currentWaveform = Sine;
    bool bipolar = true; // -1 to +1 output
    
    // Random number generation
    juce::Random random;
    float lastRandomValue = 0.0f;
    float targetRandomValue = 0.0f;
    float randomPhase = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFO)
};