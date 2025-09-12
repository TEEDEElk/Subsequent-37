// ADSREnvelope.cpp
#include "ADSREnvelope.h"
#include <cmath>
#include <algorithm>
#include <c++/version> // Include for version check

// Custom clamp for compatibility if std::clamp is unavailable
template <typename T>
T clamp(T v, T lo, T hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

ADSREnvelope::ADSREnvelope() {
    calculateRates();
}

void ADSREnvelope::setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    calculateRates();
}

void ADSREnvelope::noteOn() {
    currentState = Attack;
}

void ADSREnvelope::noteOff() {
    if (currentState != Idle) {
        currentState = Release;
    }
}

float ADSREnvelope::getNextSample() {
    switch (currentState) {
        case Idle:
            currentLevel = 0.0f;
            break;
            
        case Attack:
            currentLevel += attackRate;
            if (currentLevel >= 1.0f) {
                currentLevel = 1.0f;
                currentState = Decay;
            }
            break;
            
        case Decay:
            currentLevel -= decayRate;
            if (currentLevel <= sustainLevel) {
                currentLevel = sustainLevel;
                currentState = Sustain;
            }
            break;
            
        case Sustain:
            // Stay at sustain level until noteOff
            currentLevel = sustainLevel;
            break;
            
        case Release:
            currentLevel -= releaseRate;
            if (currentLevel <= 0.0f) {
                currentLevel = 0.0f;
                currentState = Idle;
            }
            break;
    }
    
    return currentLevel;
}

void ADSREnvelope::setAttack(float attackMs) {
    attackTime = std::max(0.1f, attackMs);
    calculateRates();
}

void ADSREnvelope::setDecay(float decayMs) {
    decayTime = std::max(0.1f, decayMs);
    calculateRates();
}

void ADSREnvelope::setSustain(float level) {
    sustainLevel = clamp(level, 0.0f, 1.0f);
    calculateRates();
}

void ADSREnvelope::setRelease(float releaseMs) {
    releaseTime = std::max(0.1f, releaseMs);
    calculateRates();
}

bool ADSREnvelope::isActive() const {
    return currentState != Idle;
}

bool ADSREnvelope::isInAttack() const {
    return currentState == Attack;
}

bool ADSREnvelope::isInRelease() const {
    return currentState == Release;
}

void ADSREnvelope::setADSR(float attackMs, float decayMs, float sustainLevel, float releaseMs) {
    attackTime = std::max(0.1f, attackMs);
    decayTime = std::max(0.1f, decayMs);
    this->sustainLevel = clamp(sustainLevel, 0.0f, 1.0f);
    releaseTime = std::max(0.1f, releaseMs);
    calculateRates();
}

void ADSREnvelope::calculateRates() {
    attackRate = calculateRate(attackTime, 1.0f);
    decayRate = calculateRate(decayTime, 1.0f - sustainLevel);
    releaseRate = calculateRate(releaseTime, sustainLevel);
}

float ADSREnvelope::msToSamples(float ms) const {
    return ms * 0.001f * sampleRate;
}

float ADSREnvelope::calculateRate(float timeMs, float targetLevel) const {
    if (timeMs <= 0.0f || targetLevel <= 0.0f) {
        return targetLevel; // Instant transition
    }
    
    float samples = msToSamples(timeMs);
    return targetLevel / samples;
}