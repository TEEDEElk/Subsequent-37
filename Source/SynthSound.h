
#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    SynthSound();

    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSound);
};