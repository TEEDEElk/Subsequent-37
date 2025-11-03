// Chorus.cpp
#include "Chorus.h"

Chorus::Chorus()
{
    lfo.initialise([](float x) { return std::sin(x); });
}

void Chorus::prepare(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
    lfo.prepare({ newSampleRate, static_cast<juce::uint32>(samplesPerBlock), 1 });
    lfo.setFrequency(0.5f); // Default rate

    // Initialize delay buffers for each voice
    const int delayBufferSize = static_cast<int>(sampleRate * 0.05); // 50ms max delay
    for (auto& voice : voices)
    {
        voice.buffer.setSize(2, delayBufferSize);
        voice.buffer.clear();
        voice.writePosition = 0;
        voice.lfoPhase = 0.0f;
    }
}

void Chorus::process(juce::AudioBuffer<float>& buffer)
{
    if (!isEnabled)
        return;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    // Create dry/wet mix
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(buffer);

    buffer.clear();

    // Process each chorus voice
    for (size_t i = 0; i < voices.size(); ++i)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = dryBuffer.getReadPointer(channel);
            auto* outputData = buffer.getWritePointer(channel);

            processVoice(voices[i], const_cast<float*>(channelData), numSamples, channel);

            // Mix this voice into the output
            for (int sample = 0; sample < numSamples; ++sample)
            {
                outputData[sample] += voices[i].buffer.getSample(channel, 
                    (voices[i].writePosition + sample) % voices[i].buffer.getNumSamples());
            }
        }
    }

    // Apply mix and width
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* dryData = dryBuffer.getReadPointer(channel);
        auto* wetData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float wet = wetData[sample] / static_cast<float>(voices.size());
            float dry = dryData[sample];
            
            // Apply stereo width (more effect on channel 1)
            float channelMix = mix;
            if (channel == 1)
                channelMix = juce::jlimit(0.0f, 1.0f, mix * (1.0f + width));
            
            wetData[sample] = dry * (1.0f - channelMix) + wet * channelMix;
        }
    }
}

void Chorus::processVoice(Voice& voice, float* channelData, int numSamples, int channel)
{
    const int bufferSize = voice.buffer.getNumSamples();
    auto* delayData = voice.buffer.getWritePointer(channel);
    
    // LFO parameters for this voice
    const float lfoPhaseOffset = static_cast<float>(voices.data() - &voice) / static_cast<float>(voices.size());
    const float baseDelay = 0.005f; // 5ms base delay
    const float maxModulation = depth * 0.01f; // Up to 10ms modulation

    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Calculate LFO modulated delay time
        float lfoValue = lfo.processSample(0.0f);
        float delayTimeSamples = (baseDelay + lfoValue * maxModulation) * static_cast<float>(sampleRate);
        
        // Calculate read position with interpolation
        float readPos = voice.writePosition - delayTimeSamples;
        while (readPos < 0.0f)
            readPos += bufferSize;
        
        int readPosInt = static_cast<int>(readPos);
        float frac = readPos - readPosInt;
        
        // Linear interpolation
        float sample1 = delayData[readPosInt % bufferSize];
        float sample2 = delayData[(readPosInt + 1) % bufferSize];
        float delayedSample = sample1 + frac * (sample2 - sample1);
        
        // Write input to delay buffer
        delayData[voice.writePosition] = channelData[sample];
        
        // Store processed output
        delayData[voice.writePosition] = delayedSample;
        
        // Advance write position
        voice.writePosition = (voice.writePosition + 1) % bufferSize;
    }
}
