// CustomLookAndFeel.cpp
#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    // Set default colors
    setColour(juce::Slider::rotarySliderFillColourId, accentColor);
    setColour(juce::Slider::thumbColourId, accentColor);
    setColour(juce::Slider::textBoxTextColourId, textColor);
    setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::Slider::textBoxHighlightColourId, accentColor);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x6634495e));

    setColour(juce::ComboBox::backgroundColourId, panelColor);
    setColour(juce::ComboBox::textColourId, textColor);
    setColour(juce::ComboBox::arrowColourId, accentColor);
    setColour(juce::ComboBox::outlineColourId, juce::Colour(0x6634495e));

    setColour(juce::Label::textColourId, textColor);
    setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);

    setColour(juce::TextButton::buttonColourId, panelColor);
    setColour(juce::TextButton::buttonOnColourId, accentColor);
    setColour(juce::TextButton::textColourOnId, textColor);
    setColour(juce::TextButton::textColourOffId, textColor);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

bool CustomLookAndFeel::loadKnobGraphics(const void* data, size_t size)
{
    knobStrip = juce::ImageCache::getFromMemory(data, size);
    if (knobStrip.isValid()) {
        frameWidth = knobStrip.getWidth() / numFrames;
        frameHeight = knobStrip.getHeight();
        return true;
    }
    return false;
}

bool CustomLookAndFeel::loadBackgroundGraphics(const void* data, size_t size)
{
    backgroundImage = juce::ImageCache::getFromMemory(data, size);
    return backgroundImage.isValid();
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                        juce::Slider& slider)
{
    if (knobStrip.isValid() && frameWidth > 0) {
        // Calculate frame index based on slider position (0-127)
        const bool isMouseOver = slider.isMouseOverOrDragging();
        const int baseFrame = isMouseOver ? 64 : 0; // Hover states start at frame 64
        const int frameIndex = baseFrame + juce::jlimit(0, 63, static_cast<int>(sliderPos * 63));

        // Ensure we don't go out of bounds
        const int safeFrameIndex = juce::jlimit(0, numFrames - 1, frameIndex);

        // Draw the appropriate frame from the sprite sheet
        g.drawImage(knobStrip, x, y, width, height,
                   safeFrameIndex * frameWidth, 0, frameWidth, frameHeight);
    } else {
        // Fallback: draw custom rotary slider
        drawFallbackRotarySlider(g, x, y, width, height, sliderPos, slider);
    }
}

void CustomLookAndFeel::drawFallbackRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                                float sliderPos, juce::Slider& slider)
{
    const float diameter = juce::jmin(width, height) * 0.8f;
    const float radius = diameter * 0.5f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = juce::MathConstants<float>::pi * 1.2f + sliderPos * (juce::MathConstants<float>::pi * 1.6f);

    // Draw knob background
    g.setColour(panelColor.brighter(0.1f));
    g.fillEllipse(rx, ry, rw, rw);

    // Draw knob edge
    g.setColour(panelColor.darker(0.3f));
    g.drawEllipse(rx, ry, rw, rw, 2.0f);

    // Draw value arc
    juce::Path valueArc;
    valueArc.addArc(rx, ry, rw, rw, juce::MathConstants<float>::pi * 1.2f, angle, true);
    g.setColour(accentColor);
    g.strokePath(valueArc, juce::PathStrokeType(3.0f));

    // Draw indicator
    const float indicatorLength = radius * 0.7f;
    const float indicatorWidth = 2.0f;
    juce::Line<float> indicator(centreX, centreY,
                               centreX + indicatorLength * std::cos(angle - juce::MathConstants<float>::halfPi),
                               centreY + indicatorLength * std::sin(angle - juce::MathConstants<float>::halfPi));
    g.setColour(textColor);
    g.drawLine(indicator, indicatorWidth);

    // Draw center dot
    g.setColour(accentColor);
    g.fillEllipse(centreX - 2.0f, centreY - 2.0f, 4.0f, 4.0f);
}

void CustomLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                                    int buttonX, int buttonY, int buttonW, int buttonH,
                                    juce::ComboBox& box)
{
    // Draw custom combo box background
    g.setColour(findColour(juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle(0, 0, width, height, 4.0f);

    g.setColour(findColour(juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle(0.5f, 0.5f, width - 1.0f, height - 1.0f, 4.0f, 1.0f);

    // Draw arrow
    const float arrowSize = 0.3f;
    const float arrowX = width - 15.0f;
    const float arrowY = height * 0.5f;

    juce::Path arrow;
    arrow.addTriangle(arrowX - arrowSize, arrowY - arrowSize,
                     arrowX + arrowSize, arrowY - arrowSize,
                     arrowX, arrowY + arrowSize);

    g.setColour(findColour(juce::ComboBox::arrowColourId));
    g.fillPath(arrow);
}

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(findColour(juce::Label::textColourId));
    g.setFont(getLabelFont(label));

    if (label.isBeingEdited()) {
        g.drawRect(0, 0, label.getWidth(), label.getHeight());
    } else {
        g.drawText(label.getText(), 0, 0, label.getWidth(), label.getHeight(),
                  label.getJustificationType(), true);
    }
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                           const juce::Colour& backgroundColour,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
    auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
                                    .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted) {
        baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);
    }

    g.setColour(baseColour);
    g.fillRoundedRectangle(bounds, 4.0f);

    g.setColour(button.findColour(juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
}

juce::Font CustomLookAndFeel::getTextButtonFont(juce::TextButton&, int buttonHeight)
{
    return juce::Font("Arial Narrow", buttonHeight * 0.5f, juce::Font::bold);
}

juce::Font CustomLookAndFeel::getLabelFont(juce::Label&)
{
    return juce::Font("Bank Gothic", 12.0f, juce::Font::plain);
}

juce::Font CustomLookAndFeel::getComboBoxFont(juce::ComboBox&)
{
    return juce::Font("Arial Narrow", 14.0f, juce::Font::plain);
}