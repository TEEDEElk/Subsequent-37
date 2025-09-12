// Oscillator.cpp
#include "Oscillator.h"
#include <JuceHeader.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Oscillator::Oscillator() {
    // Initialize with default values
    phase = 0.0f;
    phaseIncrement = 0.0f;
    currentFrequency = 0.0f;
    sampleRate = 44100.0f;
    currentWaveform = 0; // Sine wave by default
}

void Oscillator::setFrequency(float frequency) {
    currentFrequency = frequency;
    phaseIncrement = currentFrequency / sampleRate;
}

void Oscillator::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    // Recalculate phase increment with new sample rate
    if (currentFrequency > 0) {
        phaseIncrement = currentFrequency / sampleRate;
    }
}

float Oscillator::process() {
    // Generate the current sample based on waveform type
    float output = 0.0f;
    
    switch (currentWaveform) {
        case 0: output = generateSine(); break;
        case 1: output = generateSaw(); break;
        case 2: output = generateSquare(); break;
        case 3: output = generateTriangle(); break;
        default: output = generateSine(); break;
    }
    
    // Advance phase
    phase += phaseIncrement;
    
    // Wrap phase between 0.0 and 1.0
    if (phase >= 1.0f) {
        phase -= 1.0f;
    }
    
    return output;
}

void Oscillator::setWaveform(int waveform) {
    // Clamp waveform value to valid range
    currentWaveform = juce::jlimit(0, 3, waveform);
}

// Waveform generation functions
float Oscillator::generateSine() {
    return std::sin(2.0f * M_PI * phase);
}

float Oscillator::generateSaw() {
    // Basic sawtooth: ramp from -1 to 1
    return 2.0f * phase - 1.0f;
}

float Oscillator::generateSquare() {
    // Basic square wave (50% duty cycle)
    return (phase < 0.5f) ? 1.0f : -1.0f;
}

float Oscillator::generateTriangle() {
    // Triangle wave: two linear segments
    if (phase < 0.5f) {
        return 4.0f * phase - 1.0f; // Ramp up from -1 to 1
    } else {
        return 3.0f - 4.0f * phase; // Ramp down from 1 to -1
    }
}
