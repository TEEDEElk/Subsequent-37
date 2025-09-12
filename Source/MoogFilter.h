// MoogFilter.h
#pragma once

class MoogFilter {
public:
    MoogFilter();
    
    void setSampleRate(float sampleRate);
    void process(float* buffer, int numSamples);
    float processSample(float input);
    
    // Filter parameters
    void setCutoff(float cutoffHz);
    void setResonance(float resonance);
    void setDrive(float driveAmount); // 0.0 to 1.0+
    
    // Reset the filter state
    void reset();
    
    // Get current parameters
    float getCutoff() const { return cutoff; }
    float getResonance() const { return resonance; }
    float getDrive() const { return drive; }
    
private:
    float sampleRate;
    float cutoff;
    float resonance;
    float drive;
    
    // Filter state variables
    float y1, y2, y3, y4;
    float oldx;
    float oldy1, oldy2, oldy3;
    
    // Calculated coefficients
    float p, k;
    
    void calculateCoefficients();
};