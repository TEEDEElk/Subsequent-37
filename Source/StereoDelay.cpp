#include "StereoDelay.h"

StereoDelay::StereoDelay() {}

void StereoDelay::prepare(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
    
    // Create delay buffer with 2 seconds maximum delay
    int bufferSize = static_cast<int>(2.0 * sampleRate) + samplesPerBlock;
    delayBuffer.setSize(2, bufferSize);
    delayBuffer.clear();
    
    writePosition = 0;
    updateDelayTime();
}

void StereoDelay::process(juce::AudioBuffer<float>& buffer)
{
    if (!isEnabled) return;
    
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    const int bufferSize = delayBuffer.getNumSamples();
    
    for (int channel = 0; channel < numChannels; ++channel) {
        float* channelData = buffer.getWritePointer(channel);
        float* delayData = delayBuffer.getWritePointer(channel);
        
        int readPosition = (writePosition - delayTimeSamples + bufferSize) % bufferSize;
        
        for (int i = 0; i < numSamples; ++i) {
            const float in = channelData[i];
            const float delaySample = delayData[readPosition];
            
            // Write to delay buffer
            delayData[writePosition] = in + (delaySample * feedback);
            
            // Mix dry and wet
            channelData[i] = (in * (1.0f - mix)) + (delaySample * mix);
            
            // Update positions
            readPosition = (readPosition + 1) % bufferSize;
            writePosition = (writePosition + 1) % bufferSize;
        }
    }
}

void StereoDelay::setDelayTime(float timeMs)
{
    delayTimeMs = timeMs;
    updateDelayTime();
}

void StereoDelay::setHostInfo(double bpm, bool isPlaying)
{
    hostBPM = bpm;
    hostIsPlaying = isPlaying;
    if (syncToHost && isPlaying) {
        updateDelayTime();
    }
}

void StereoDelay::updateDelayTime()
{
    if (syncToHost && hostIsPlaying) {
        // Sync to quarter notes
        double secondsPerBeat = 60.0 / hostBPM;
        delayTimeMs = secondsPerBeat * 1000.0;
    }
    
    delayTimeSamples = static_cast<int>((delayTimeMs / 1000.0) * sampleRate);
    delayTimeSamples = juce::jmax(1, delayTimeSamples);
}