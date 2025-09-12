#pragma once

#include <JuceHeader.h>

class StereoDelay
{
public:
    StereoDelay();
    
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setDelayTime(float timeMs);
    void setFeedback(float feedback) { this->feedback = juce::jlimit(0.0f, 0.95f, feedback); }
    void setMix(float mix) { this->mix = juce::jlimit(0.0f, 1.0f, mix); }
    void setSyncToHost(bool sync) { syncToHost = sync; }
    
    void setHostInfo(double bpm, bool isPlaying);
    
private:
    bool isEnabled = false;
    float delayTimeMs = 250.0f;
    float feedback = 0.5f;
    float mix = 0.3f;
    bool syncToHost = false;
    
    double sampleRate = 44100.0;
    int writePosition = 0;
    
    juce::AudioBuffer<float> delayBuffer;
    
    double hostBPM = 120.0;
    bool hostIsPlaying = false;
    
    void updateDelayTime();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoDelay)
};