// ModulationMatrix.cpp
#include "ModulationMatrix.h"
#include "LFO.h"
#include "ADSREnvelope.h"

ModulationMatrix::ModulationMatrix() {
    // Initialize arrays
    baseValues.fill(0.0f);
    modulatedValues.fill(0.0f);
}

void ModulationMatrix::addConnection(Source source, Destination destination, float amount) {
    // Remove any existing connection for this source->destination
    removeConnection(source, destination);
    
    // Add new connection
    connections.emplace_back(source, destination, amount);
}

void ModulationMatrix::removeConnection(Source source, Destination destination) {
    connections.erase(
        std::remove_if(connections.begin(), connections.end(),
            [source, destination](const Connection& conn) {
                return conn.source == source && conn.destination == destination;
            }),
        connections.end()
    );
}

void ModulationMatrix::clearConnections() {
    connections.clear();
}

bool ModulationMatrix::hasConnection(Source source, Destination destination) const {
    return std::any_of(connections.begin(), connections.end(),
        [source, destination](const Connection& conn) {
            return conn.source == source && conn.destination == destination;
        });
}

void ModulationMatrix::processBlock(int numSamples) {
    // Reset modulated values to base values
    modulatedValues = baseValues;
    
    // Process each sample in the block
    for (int sample = 0; sample < numSamples; ++sample) {
        applyModulation(sample);
    }
}

float ModulationMatrix::getDestinationValue(Destination dest) const {
    if (dest >= 0 && dest < NumDestinations) {
        return modulatedValues[dest];
    }
    return 0.0f;
}

void ModulationMatrix::setBaseValue(Destination dest, float value) {
    if (dest >= 0 && dest < NumDestinations) {
        baseValues[dest] = value;
    }
}

float ModulationMatrix::getSourceValue(Source source, int sampleIndex) const {
    switch (source) {
        case LFO1:
            return lfo1 ? lfo1->getNextSample() : 0.0f;
            
        case LFO2:
            return lfo2 ? lfo2->getNextSample() : 0.0f;
            
        case FilterEnv:
            return filterEnv ? filterEnv->getNextSample() : 0.0f;
            
        case AmpEnv:
            return ampEnv ? ampEnv->getNextSample() : 0.0f;
            
        case Velocity:
            return velocity;
            
        case ModWheel:
            return modWheel;
            
        case Aftertouch:
            return aftertouch;
            
        case PitchBend:
            return pitchBend;
            
        case None:
        default:
            return 0.0f;
    }
}

void ModulationMatrix::applyModulation(int sampleIndex) {
    for (const auto& connection : connections) {
        float sourceValue = getSourceValue(connection.source, sampleIndex);
        float modulationAmount = sourceValue * connection.amount;
        
        // Apply modulation to the destination
        int destIndex = static_cast<int>(connection.destination);
        if (destIndex >= 0 && destIndex < NumDestinations) {
            modulatedValues[destIndex] = baseValues[destIndex] + modulationAmount;
            
            // Apply constraints based on destination
            switch (connection.destination) {
                case FilterCutoff:
                    modulatedValues[destIndex] = juce::jlimit(20.0f, 20000.0f, modulatedValues[destIndex]);
                    break;
                    
                case FilterResonance:
                    modulatedValues[destIndex] = juce::jlimit(0.0f, 1.0f, modulatedValues[destIndex]);
                    break;
                    
                case Osc1Pitch:
                case Osc2Pitch:
                    // Pitch modulation in semitones (±12 semitones max)
                    modulatedValues[destIndex] = juce::jlimit(-12.0f, 12.0f, modulatedValues[destIndex]);
                    break;
                    
                case Osc1PWM:
                case Osc2PWM:
                    // PWM modulation (0.1 to 0.9 duty cycle)
                    modulatedValues[destIndex] = juce::jlimit(0.1f, 0.9f, modulatedValues[destIndex]);
                    break;
                    
                case OscMix:
                case AmpLevel:
                case LFO1Amount:
                case LFO2Amount:
                    // 0.0 to 1.0 range
                    modulatedValues[destIndex] = juce::jlimit(0.0f, 1.0f, modulatedValues[destIndex]);
                    break;
                    
                default:
                    break;
            }
        }
    }
}