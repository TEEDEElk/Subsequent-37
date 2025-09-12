// ModulationMatrix.h
#pragma once

#include <JuceHeader.h>
#include <vector>
#include <functional>

// Forward declarations
class LFO;
class ADSREnvelope;

class ModulationMatrix {
public:
    // Modulation sources
    enum Source {
        None = 0,
        LFO1,
        LFO2,
        FilterEnv,
        AmpEnv,
        Velocity,
        ModWheel,
        Aftertouch,
        PitchBend,
        NumSources
    };
    
    // Modulation destinations
    enum Destination {
        // Oscillator
        Osc1Pitch,
        Osc2Pitch,
        Osc1PWM,
        Osc2PWM,
        OscMix,
        
        // Filter
        FilterCutoff,
        FilterResonance,
        FilterEnvAmount,
        
        // Amplitude
        AmpLevel,
        
        // LFOs
        LFO1Amount,
        LFO2Amount,
        
        NumDestinations
    };
    
    // Modulation connection
    struct Connection {
        Source source;
        Destination destination;
        float amount; // -1.0 to +1.0 for bipolar, 0.0 to +1.0 for unipolar
        
        Connection(Source s, Destination d, float a)
            : source(s), destination(d), amount(a) {}
    };
    
    ModulationMatrix();
    
    // Connection management
    void addConnection(Source source, Destination destination, float amount);
    void removeConnection(Source source, Destination destination);
    void clearConnections();
    bool hasConnection(Source source, Destination destination) const;
    
    // Source registration
    void setLFO1(LFO* lfo) { lfo1 = lfo; }
    void setLFO2(LFO* lfo) { lfo2 = lfo; }
    void setFilterEnv(ADSREnvelope* env) { filterEnv = env; }
    void setAmpEnv(ADSREnvelope* env) { ampEnv = env; }
    
    // Modulation values
    void setVelocity(float vel) { velocity = vel; }
    void setModWheel(float wheel) { modWheel = wheel; }
    void setAftertouch(float touch) { aftertouch = touch; }
    void setPitchBend(float bend) { pitchBend = bend; }
    
    // Processing
    void processBlock(int numSamples);
    
    // Destination value access
    float getDestinationValue(Destination dest) const;
    void setBaseValue(Destination dest, float value);
    
    // Connection query
    const std::vector<Connection>& getConnections() const { return connections; }
    
private:
    // Source pointers
    LFO* lfo1 = nullptr;
    LFO* lfo2 = nullptr;
    ADSREnvelope* filterEnv = nullptr;
    ADSREnvelope* ampEnv = nullptr;
    
    // Continuous modulation values
    float velocity = 0.0f;
    float modWheel = 0.0f;
    float aftertouch = 0.0f;
    float pitchBend = 0.0f;
    
    // Connections
    std::vector<Connection> connections;
    
    // Destination values (base + modulation)
    std::array<float, NumDestinations> baseValues;
    std::array<float, NumDestinations> modulatedValues;
    
    // Helper functions
    float getSourceValue(Source source, int sampleIndex) const;
    void applyModulation(int sampleIndex);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationMatrix)
};