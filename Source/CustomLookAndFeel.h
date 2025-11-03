// CustomLookAndFeel.h
#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() override;

    // Rotary slider drawing
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override;

    // ComboBox styling
    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override;

    void drawLabel (juce::Graphics& g, juce::Label& label) override;

    // Button styling
    void drawButtonBackground (juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

    // Text styling
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override;
    juce::Font getLabelFont (juce::Label& label) override;
    juce::Font getComboBoxFont (juce::ComboBox&) override;

    // Load custom graphics (optional)
    bool loadKnobGraphics(const void* data, size_t size);
    bool loadBackgroundGraphics(const void* data, size_t size);

    // Get graphics
    juce::Image getKnobStrip() const { return knobStrip; }
    juce::Image getBackgroundImage() const { return backgroundImage; }

private:
    juce::Image knobStrip;
    juce::Image backgroundImage;

    int numFrames = 128; // 64 positions + 64 hover states
    int frameWidth = 0;
    int frameHeight = 0;

    juce::Colour accentColor = juce::Colour(0xfff39c12); // Moog orange
    juce::Colour panelColor = juce::Colour(0xff34495e);
    juce::Colour textColor = juce::Colour(0xffecf0f1);

    void drawFallbackRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                 float sliderPos, juce::Slider& slider);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};
