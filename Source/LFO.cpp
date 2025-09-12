// LFO.cpp
#include "LFO.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

LFO::LFO() {
    reset();
}

void LFO::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    phaseIncrement = static_cast<float>(frequency / sampleRate);
}

void LFO::setFrequency(float frequencyHz) {
    frequency = juce::jlimit(0.01f, 100.0f, frequencyHz); // 0.01Hz to 100Hz
    if (sampleRate > 0.0) {
        phaseIncrement = static_cast<float>(frequency / sampleRate);
    }
}

void LFO::setWaveform(int waveform) {
    currentWaveform = juce::jlimit(0, NumWaveforms - 1, waveform);
}

void LFO::setBipolar(bool isBipolar) {
    bipolar = isBipolar;
}

float LFO::getNextSample() {
    float output = 0.0f;
    
    switch (currentWaveform) {
        case Sine:         output = generateSine(); break;
        case Triangle:     output = generateTriangle(); break;
        case Sawtooth:     output = generateSawtooth(); break;
        case Square:       output = generateSquare(); break;
        case Random:       output = generateRandom(); break;
        case RandomSmooth: output = generateRandomSmooth(); break;
        default:           output = generateSine(); break;
    }
    
    // Advance phase
    phase += phaseIncrement;
    
    // Wrap phase between 0.0 and 1.0
    while (phase >= 1.0f) {
        phase -= 1.0f;
        
        // For random waveforms, pick new target values on phase wrap
        if (currentWaveform == Random || currentWaveform == RandomSmooth) {
            lastRandomValue = targetRandomValue;
            targetRandomValue = random.nextFloat() * 2.0f - 1.0f;
        }
    }
    
    // Convert to unipolar if needed
    if (!bipolar) {
        output = (output + 1.0f) * 0.5f;
    }
    
    return output;
}

void LFO::skipSamples(int numSamples) {
    phase += phaseIncrement * numSamples;
    
    // Wrap phase
    while (phase >= 1.0f) {
        phase -= 1.0f;
        
        // Update random values if needed
        if (currentWaveform == Random || currentWaveform == RandomSmooth) {
            lastRandomValue = targetRandomValue;
            targetRandomValue = random.nextFloat() * 2.0f - 1.0f;
        }
    }
}

void LFO::reset() {
    phase = 0.0f;
    lastRandomValue = random.nextFloat() * 2.0f - 1.0f;
    targetRandomValue = random.nextFloat() * 2.0f - 1.0f;
    randomPhase = 0.0f;
}

void LFO::setPhase(float newPhase) {
    phase = juce::jlimit(0.0f, 1.0f, newPhase);
}

// Waveform generation functions
float LFO::generateSine() {
    return std::sin(2.0f * static_cast<float>(M_PI) * phase);
}

float LFO::generateTriangle() {
    if (phase < 0.25f) {
        return 4.0f * phase; // 0 to 1
    } else if (phase < 0.75f) {
        return 2.0f - 4.0f * phase; // 1 to -1
    } else {
        return -4.0f + 4.0f * phase; // -1 to 0
    }
}

float LFO::generateSawtooth() {
    return 2.0f * phase - 1.0f; // -1 to 1
}

float LFO::generateSquare() {
    return (phase < 0.5f) ? 1.0f : -1.0f;
}

float LFO::generateRandom() {
    // Step random - new value each cycle
    return targetRandomValue;
}

float LFO::generateRandomSmooth() {
    // Smooth random - interpolate between values
    if (currentWaveform != RandomSmooth) {
        // Initialize if we just switched to this waveform
        lastRandomValue = targetRandomValue;
        randomPhase = 0.0f;
    }
    
    // Linear interpolation between random values
    randomPhase += phaseIncrement;
    if (randomPhase >= 1.0f) {
        randomPhase = 0.0f;
        lastRandomValue = targetRandomValue;
        targetRandomValue = random.nextFloat() * 2.0f - 1.0f;
    }
    
    return lastRandomValue + (targetRandomValue - lastRandomValue) * randomPhase;
}