// Oscillator.h
#pragma once

class Oscillator {
public:
    Oscillator();
    
    void setFrequency(float frequency);
    void setSampleRate(float sampleRate);
    float process();
    void setWaveform(int waveform); // 0 = sin, 1 = saw, 2 = square, 3 = triangle
    
private:
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    float currentFrequency = 0.0f;
    float sampleRate = 44100.0f;
    int currentWaveform = 0;
    
    // Helper functions for each waveform
    float generateSine();
    float generateSaw();
    float generateSquare();
    float generateTriangle();
};