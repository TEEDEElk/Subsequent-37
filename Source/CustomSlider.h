// CustomSlider.h
#pragma once

#include <JuceHeader.h>

class CustomSlider : public juce::Slider
{
public:
    CustomSlider();
    ~CustomSlider() override;
    
    void setKnobImage(const juce::Image& image, int numFrames);
    void paint(juce::Graphics& g) override;
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    
private:
    juce::Image knobImage;
    int frameCount = 128;
    int frameWidth = 0;
    int frameHeight = 0;
    bool isHovered = false;
    
    void drawCustomKnob(juce::Graphics& g);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomSlider)
};