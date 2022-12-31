/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

struct RateDialLookAndFeel : public juce::LookAndFeel_V4
{
    void drawRotarySlider (juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override
    {
        juce::Rectangle <float> dialArea (x, y, width, height);
        juce::Rectangle <float> ellipseArea (dialArea.reduced(10));
        juce::Point <float> centrePoint = ellipseArea.getCentre();
        
        float radius = ellipseArea.getWidth() / 2;
        //start angle = 3.76991 end angle = 8.79646 in radians
        float dialAngle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        g.setColour(juce::Colours::black);
        dialArea.reduce (2.f, 2.f);
        g.fillEllipse (dialArea);
        //g.drawEllipse (dialArea, 1.f);
    
        const int topLayerNumSides = 7;
        float topLayerSideLength = 2.0 * juce::MathConstants<double>::pi / topLayerNumSides;
        float topLayerStartAng = juce::MathConstants<double>::pi;
        
        juce::Path topLayer;
        topLayer.startNewSubPath ( 0 + sin (topLayerStartAng) * radius, 0 + cos (topLayerStartAng) * radius);
        
        for (int i = 0; i < topLayerNumSides + 1; i++)
        {
            float tlStep = topLayerStartAng + i * topLayerSideLength;
            juce::Point <float> plot (0 + sin (tlStep) * radius, 0 + cos (tlStep) * radius);
            topLayer.lineTo (plot);
        }

        g.fillPath (topLayer, juce::AffineTransform::rotation(dialAngle).translated(centrePoint));
        
        g.setColour(juce::Colours::white.darker());
        
        g.strokePath (topLayer, juce::PathStrokeType(1.f), juce::AffineTransform::rotation(dialAngle).translated(centrePoint));
        
        g.setColour(juce::Colours::white);
        
        juce::Path dialTick;
        dialTick.addRectangle(0 - 2.5f, -radius + radius / 8, 5.0f, radius * 0.5);
        g.fillPath(dialTick, juce::AffineTransform::rotation(dialAngle).translated(centrePoint));
    }
};

struct BypassBtnLookAndFeel : public juce::LookAndFeel_V4
{
    BypassBtnLookAndFeel()
    {
        randNum = juce::Random::getSystemRandom().nextFloat() * juce::MathConstants<double>::twoPi;
    }
    void drawButtonBackground (juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour, bool isMouseOverButton, bool isButtonDown) override
    {
        juce::Rectangle <float> btnArea (button.getLocalBounds().toFloat());
        float reduction = 0.1 * button.getWidth();
        juce::Rectangle <float> reducedBtnArea (btnArea.reduced(reduction));
        juce::Rectangle <float> nutInner (reducedBtnArea.reduced(reduction));
        juce::Rectangle <float> btnRaisedStageOne (nutInner.reduced(reduction / 3.25));
        juce::Rectangle <float> btnRaisedStageTwo (btnRaisedStageOne.reduced(reduction / 3.25));
        juce::Point <float> buttonCentre(btnArea.getCentre().toFloat());
        
        g.fillAll (juce::Colours::lightgrey);
        g.setColour (juce::Colours::black);

        g.drawEllipse(nutInner, 1.f);
        
        float radius = reducedBtnArea.getWidth() / 2;
        double nutStartAngle = juce::MathConstants <double>::pi - randNum;
        const int nutNumSides = 6;
        const float nutStep = 2.0 * juce::MathConstants <double>::pi / nutNumSides;
        
        juce::Path nut;
        nut.startNewSubPath(0 + sin (nutStartAngle) * radius, 0 + cos (nutStartAngle) * radius);
        for (int i = 0; i < nutNumSides + 1; i++)
        {
            juce::Point <float> plot(0 + sin (nutStartAngle + i * nutStep) * radius,
                                     0 + cos (nutStartAngle + i * nutStep) * radius);
            nut.lineTo(plot);
        }
        g.setColour(juce::Colours::lightgrey);
        g.fillPath (nut, juce::AffineTransform::translation(buttonCentre));
        
        g.setColour (juce::Colours::black);
        g.strokePath (nut, juce::PathStrokeType(1.f), juce::AffineTransform::translation(buttonCentre));
        
        g.setColour (juce::Colour (0xffafafaf));
        g.fillEllipse (nutInner);
        
        g.setColour (juce::Colours::black);
        g.drawEllipse(nutInner, 1.f);
        g.drawEllipse(btnRaisedStageOne, 0.5f);
        
        if (isMouseOverButton)
        {
            g.setColour (juce::Colour (0xff919191));
        }
        else if (!isMouseOverButton)
        {
            g.setColour (juce::Colour (0xffa0a0a0));
        }
        
        if (isButtonDown)
        {
            g.setColour (juce::Colour (0xff919191));
        }
        else if (!isButtonDown)
        {
            g.fillEllipse(btnRaisedStageTwo);
        }
        
        g.setColour (juce::Colours::black);
        g.drawEllipse(btnRaisedStageTwo, 0.25f);
    }
private:
    float randNum;
};

struct GetColourInterpVal
{
    GetColourInterpVal (std::atomic <float> &valueToUse) : value(valueToUse) {}
    
    float retrieveValue()
    {
        return value.load();
    }
    std::atomic <float> &value;
};

class RingModAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public juce::Slider::Listener,
                                     public juce::Button::Listener,
                                     public juce::Timer
{
public:
    RingModAudioProcessorEditor (RingModAudioProcessor&);
    ~RingModAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged (juce::Slider *slider) override;
    void buttonClicked (juce::Button *button) override;
    void timerCallback() override;
    
    GetColourInterpVal getColourInterpVal;

private:
    RateDialLookAndFeel rateDialLookAndFeel;
    BypassBtnLookAndFeel bypassBtnLookAndFeel;
    
    juce::Slider rateDial;
    juce::TextButton bypassButton;
    juce::Label rmLabel;
    
    bool btnState { true };
    float interpolationValue { 0 };
    
    RingModAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModAudioProcessorEditor)
};
