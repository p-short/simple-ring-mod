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
    void drawButtonBackground (juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour, bool isMouseOverButton, bool isButtonDown) override
    {
        juce::Rectangle <float> btnArea (button.getLocalBounds().toFloat());
        juce::Rectangle <float> reducedBtnArea (btnArea.reduced(4));
        juce::Point <float> buttonCentre(btnArea.getCentre().toFloat());
        
        g.setColour (juce::Colours::red);
        g.drawRect (btnArea);
        g.drawRect (reducedBtnArea);
        
        float radius = reducedBtnArea.getWidth() / 2;
        double nutStartAngle = juce::MathConstants <double>::pi;
        const int nutNumSides = 6;
        
        juce::Path nut;
        nut.startNewSubPath(0 + sin (nutStartAngle) * radius, 0 + cos (nutStartAngle) * radius);
        for (int i = 0; i < nutNumSides; i++)
        {
            
        }
        
    }
};

class RingModAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public juce::Slider::Listener
{
public:
    RingModAudioProcessorEditor (RingModAudioProcessor&);
    ~RingModAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *slider) override;

private:
    RateDialLookAndFeel rateDialLookAndFeel;
    BypassBtnLookAndFeel bypassBtnLookAndFeel;
    
    juce::Slider rateDial;
    juce::TextButton bypassButton;
    
    RingModAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingModAudioProcessorEditor)
};
