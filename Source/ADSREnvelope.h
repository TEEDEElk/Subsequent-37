// ADSREnvelope.h
#pragma once

class ADSREnvelope {
public:
    ADSREnvelope();
    
    // Core functions
    void setSampleRate(float newSampleRate);
    void noteOn();
    void noteOff();
    float getNextSample();
    
    // Parameter setters
    void setAttack(float attackMs);
    void setDecay(float decayMs);
    void setSustain(float sustainLevel); // 0.0 to 1.0
    void setRelease(float releaseMs);
    
    // State queries
    bool isActive() const;
    bool isInAttack() const;
    bool isInRelease() const;
    
    // Quick setup
    void setADSR(float attackMs, float decayMs, float sustainLevel, float releaseMs);
    
private:
    enum State {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };
    
    State currentState = Idle;
    float currentLevel = 0.0f;
    float sampleRate = 44100.0f;
    
    // Time parameters in milliseconds
    float attackTime = 50.0f;
    float decayTime = 100.0f;
    float releaseTime = 200.0f;
    float sustainLevel = 0.7f;
    
    // Calculated rates per sample
    float attackRate = 0.0f;
    float decayRate = 0.0f;
    float releaseRate = 0.0f;
    
    void calculateRates();
    float msToSamples(float ms) const;
    float calculateRate(float timeMs, float targetLevel) const;
};