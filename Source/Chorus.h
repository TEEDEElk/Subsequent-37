#pragma once

#include <JuceHeader.h>

class Chorus
{
public:
    Chorus();
    
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setRate(float rateHz) { lfo.setFrequency(rateHz); }
    void setDepth(float depth) { this->depth = juce::jlimit(0.0f, 1.0f, depth); }
    void setMix(float mix) { this->mix = juce::jlimit(0.0f, 1.0f, mix); }
    void setWidth(float width) { this->width = juce::jlimit(0.0f, 1.0f, width); }
    
private:
    bool isEnabled = false;
    float depth = 0.5f;
    float mix = 0.5f;
    float width = 0.5f;
    
    double sampleRate = 44100.0;
    
    struct Voice {
        juce::AudioBuffer<float> buffer;
        int writePosition = 0;
        float lfoPhase = 0.0f;
    };
    
    std::array<Voice, 3> voices; // 3 voice chorus
    juce::dsp::Oscillator<float> lfo;
    
    void processVoice(Voice& voice, float* channelData, int numSamples, int channel);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Chorus)
};