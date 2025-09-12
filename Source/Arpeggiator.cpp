#include "Arpeggiator.h"

Arpeggiator::Arpeggiator() {}

void Arpeggiator::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    calculateStepSamples();
}

void Arpeggiator::process(juce::MidiBuffer& midiMessages, juce::MidiBuffer& outputBuffer, int numSamples)
{
    if (!isEnabled) {
        outputBuffer = midiMessages;
        return;
    }
    
    // Process input MIDI to track held notes
    for (const auto metadata : midiMessages) {
        auto message = metadata.getMessage();
        if (message.isNoteOn()) {
            heldNotes.add(message.getNoteNumber());
        } else if (message.isNoteOff()) {
            heldNotes.removeValue(message.getNoteNumber());
        }
    }
    
    // Build pattern if notes changed
    if (heldNotes.size() > 0 && (currentPatternNotes.isEmpty() || heldNotes.size() != currentPatternNotes.size())) {
        buildPatternNotes();
    }
    
    // Process arpeggiator
    for (int sample = 0; sample < numSamples; ++sample) {
        if (currentSample >= samplesPerStep) {
            currentSample = 0;
            
            // Turn off previous note
            if (currentStep < currentPatternNotes.size()) {
                int noteToTurnOff = currentPatternNotes[currentStep] + (currentOctave * 12);
                noteOff(outputBuffer, noteToTurnOff, sample);
            }
            
            // Move to next step
            currentStep = getNextNote();
            
            // Turn on new note
            if (currentStep < currentPatternNotes.size()) {
                int noteToPlay = currentPatternNotes[currentStep] + (currentOctave * 12);
                noteOn(outputBuffer, noteToPlay, 100, sample);
            }
        }
        currentSample++;
    }
    
    midiMessages.clear();
}

void Arpeggiator::setHostInfo(double bpm, bool isPlaying)
{
    hostBPM = bpm;
    hostIsPlaying = isPlaying;
    if (syncToHost && isPlaying) {
        calculateStepSamples();
    }
}

void Arpeggiator::calculateStepSamples()
{
    if (syncToHost && hostIsPlaying) {
        // Sync to host BPM (1/16 notes)
        double secondsPerBeat = 60.0 / hostBPM;
        double secondsPerStep = secondsPerBeat / 4.0; // 1/16 notes
        samplesPerStep = static_cast<int>(secondsPerStep * sampleRate);
    } else {
        // Free rate
        double secondsPerStep = 1.0 / rate;
        samplesPerStep = static_cast<int>(secondsPerStep * sampleRate);
    }
}

void Arpeggiator::buildPatternNotes()
{
    currentPatternNotes.clear();
    for (int note : heldNotes) {
        currentPatternNotes.add(note);
    }
    
    // Sort based on pattern
    switch (currentPattern) {
        case UP:
            currentPatternNotes.sort();
            break;
        case DOWN:
            currentPatternNotes.sort();
            for (int i = 0; i < currentPatternNotes.size() / 2; ++i) {
                currentPatternNotes.swap(i, currentPatternNotes.size() - 1 - i);
            }
            break;
        case UP_DOWN:
            currentPatternNotes.sort();
            for (int i = currentPatternNotes.size() - 2; i > 0; --i) {
                currentPatternNotes.add(currentPatternNotes[i]);
            }
            break;
        case RANDOM:
            juce::Random random;
            for (int i = currentPatternNotes.size() - 1; i > 0; --i) {
                currentPatternNotes.swap(i, random.nextInt(i + 1));
            }
            break;
        default:
            break;
    }
}

int Arpeggiator::getNextNote()
{
    int nextStep = currentStep;
    
    switch (currentPattern) {
        case UP:
            nextStep = (currentStep + 1) % currentPatternNotes.size();
            if (nextStep == 0) {
                currentOctave = (currentOctave + 1) % numOctaves;
            }
            break;
        case DOWN:
            nextStep = (currentStep - 1 + currentPatternNotes.size()) % currentPatternNotes.size();
            if (nextStep == currentPatternNotes.size() - 1) {
                currentOctave = (currentOctave - 1 + numOctaves) % numOctaves;
            }
            break;
        case UP_DOWN:
            if (ascending) {
                nextStep++;
                if (nextStep >= currentPatternNotes.size()) {
                    nextStep = currentPatternNotes.size() - 2;
                    ascending = false;
                }
            } else {
                nextStep--;
                if (nextStep < 0) {
                    nextStep = 1;
                    ascending = true;
                }
            }
            break;
        case RANDOM:
            juce::Random random;
            nextStep = random.nextInt(currentPatternNotes.size());
            break;
        default:
            break;
    }
    
    return nextStep;
}

void Arpeggiator::noteOn(juce::MidiBuffer& buffer, int noteNumber, int velocity, int sampleOffset)
{
    buffer.addEvent(juce::MidiMessage::noteOn(1, noteNumber, (juce::uint8)velocity), sampleOffset);
}

void Arpeggiator::noteOff(juce::MidiBuffer& buffer, int noteNumber, int sampleOffset)
{
    buffer.addEvent(juce::MidiMessage::noteOff(1, noteNumber), sampleOffset);
}