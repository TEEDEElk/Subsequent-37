// Alternative MoogFilter.cpp with better accuracy
#include "MoogFilter.h"
#include <cmath>
#include <algorithm>

// Custom clamp for compatibility if std::clamp is unavailable
template <typename T>
T clamp(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

MoogFilter::MoogFilter() {
    sampleRate = 44100.0f;
    cutoff = 1000.0f;
    resonance = 0.1f;
    drive = 1.0f;
    reset();
}

void MoogFilter::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    calculateCoefficients();
}

void MoogFilter::setCutoff(float cutoffHz) {
    cutoff = clamp(cutoffHz, 20.0f, sampleRate * 0.45f);
    calculateCoefficients();
}

void MoogFilter::setResonance(float res) {
    resonance = clamp(res, 0.0f, 1.0f);
    calculateCoefficients();
}

void MoogFilter::setDrive(float driveAmount) {
    drive = std::max(0.0f, driveAmount);
}

void MoogFilter::reset() {
    y1 = y2 = y3 = y4 = 0.0f;
    oldx = oldy1 = oldy2 = oldy3 = 0.0f;
    calculateCoefficients();
}

void MoogFilter::calculateCoefficients() {
    // More accurate coefficient calculation
    float wd = 2.0f * M_PI * cutoff;
    float T = 1.0f / sampleRate;
    float wa = (2.0f / T) * std::tan(wd * T / 2.0f);
    float g = wa * T / 2.0f;
    
    // Feedforward coefficients
    float G = g / (1.0f + g);
    
    p = G;
    k = 4.0f * resonance * (G * G * G * G);
}

float MoogFilter::processSample(float input) {
    // Apply drive and input saturation
    input *= drive;
    input = std::tanh(input);
    
    // Main filter processing
    float x = input - k * y4;
    
    // Four cascaded integrators
    y1 = p * (x + oldx) + (1.0f - p) * y1;
    y2 = p * (y1 + oldy1) + (1.0f - p) * y2;
    y3 = p * (y2 + oldy2) + (1.0f - p) * y3;
    y4 = p * (y3 + oldy3) + (1.0f - p) * y4;
    
    // Update state
    oldx = x;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;
    
    return y4;
}

void MoogFilter::process(float* buffer, int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        buffer[i] = processSample(buffer[i]);
    }
}s