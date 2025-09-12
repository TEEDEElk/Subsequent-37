#pragma once

#include <JuceHeader.h>

class Arpeggiator
{
public:
    enum Pattern {
        UP = 0,
        DOWN,
        UP_DOWN,
        RANDOM,
        PATTERN_COUNT
    };
    
    Arpeggiator();
    
    void prepare(double sampleRate);
    void process(juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer, int numSamples);
    
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setPattern(Pattern pattern) { currentPattern = pattern; }
    void setRate(float rateHz) { rate = rateHz; calculateStepSamples(); }
    void setSyncToHost(bool sync) { syncToHost = sync; }
    void setOctaves(int octaves) { numOctaves = juce::jlimit(1, 4, octaves); }
    void setGate(float gatePercent) { gate = gatePercent; }
    
    void setHostInfo(double bpm, bool isPlaying);
    
private:
    bool isEnabled = false;
    Pattern currentPattern = UP;
    float rate = 2.0f; // Hz
    bool syncToHost = false;
    int numOctaves = 1;
    float gate = 0.8f;
    
    double sampleRate = 44100.0;
    int samplesPerStep = 0;
    int currentSample = 0;
    int currentStep = 0;
    int currentOctave = 0;
    bool ascending = true;
    
    double hostBPM = 120.0;
    bool hostIsPlaying = false;
    
    juce::SortedSet<int> heldNotes;
    juce::Array<int> currentPatternNotes;
    
    void calculateStepSamples();
    void buildPatternNotes();
    int getNextNote();
    void noteOn(juce::MidiBuffer& buffer, int noteNumber, int velocity, int sampleOffset);
    void noteOff(juce::MidiBuffer& buffer, int noteNumber, int sampleOffset);
    void allNotesOff(juce::MidiBuffer& buffer, int sampleOffset);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};