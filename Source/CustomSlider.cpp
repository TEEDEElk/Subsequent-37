// CustomSlider.cpp
#include "CustomSlider.h"

CustomSlider::CustomSlider() : juce::Slider()
{
    setSliderStyle(RotaryHorizontalVerticalDrag);
    setTextBoxStyle(NoTextBox, false, 0, 0);
}

CustomSlider::~CustomSlider()
{
}

void CustomSlider::setKnobImage(const juce::Image& image, int numFrames)
{
    knobImage = image;
    frameCount = numFrames;
    if (knobImage.isValid()) {
        frameWidth = knobImage.getWidth() / frameCount;
        frameHeight = knobImage.getHeight();
    }
}

void CustomSlider::paint(juce::Graphics& g)
{
    if (knobImage.isValid() && frameWidth > 0) {
        drawCustomKnob(g);
    } else {
        // Fallback to default drawing
        LookAndFeel_V4::drawRotarySlider(g, 0, 0, getWidth(), getHeight(),
                                       (float)getValue() / (getMaximum() - getMinimum()),
                                       2.1f, 8.4f, *this);
    }
}

void CustomSlider::drawCustomKnob(juce::Graphics& g)
{
    const int baseFrame = isHovered ? frameCount / 2 : 0;
    const int valueFrame = juce::jlimit(0, (frameCount / 2) - 1, 
                                      static_cast<int>((getValue() - getMinimum()) / 
                                                      (getMaximum() - getMinimum()) * 
                                                      (frameCount / 2 - 1)));
    
    const int frameIndex = baseFrame + valueFrame;
    
    g.drawImage(knobImage, 0, 0, getWidth(), getHeight(),
               frameIndex * frameWidth, 0, frameWidth, frameHeight);
}

void CustomSlider::mouseEnter(const juce::MouseEvent&)
{
    isHovered = true;
    repaint();
}

void CustomSlider::mouseExit(const juce::MouseEvent&)
{
    isHovered = false;
    repaint();
}