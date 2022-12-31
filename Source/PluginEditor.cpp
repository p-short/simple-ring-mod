/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModAudioProcessorEditor::RingModAudioProcessorEditor (RingModAudioProcessor& p)
    : AudioProcessorEditor (&p), getColourInterpVal(p.ap_ColourInterpVal), audioProcessor (p)
{
    juce::Typeface::Ptr myTypeface = juce::Typeface::createSystemTypefaceFor (BinaryData::ImpactLabellVYZ_ttf, BinaryData::ImpactLabellVYZ_ttfSize);
    
    juce::Font myFont(myTypeface);
    myFont.setHeight(30);
    
    addAndMakeVisible(rmLabel);
    rmLabel.setFont(myFont);
    rmLabel.setText ("RING_MOD", juce::dontSendNotification);
    rmLabel.setColour (juce::Label::backgroundColourId, juce::Colours::lightgrey);
    rmLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    rmLabel.setJustificationType (juce::Justification::centred);
    
    addAndMakeVisible(rateDial);
    rateDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rateDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    rateDial.setRange(20, 4000, 0.01);
    rateDial.setValue(20);
    rateDial.addListener(this);
    
    rateDial.setLookAndFeel(&rateDialLookAndFeel);
    
    addAndMakeVisible(bypassButton);
    bypassButton.setLookAndFeel(&bypassBtnLookAndFeel);
    bypassButton.addListener(this);
    
    Timer::startTimerHz(60);
    
    setSize (400, 300);
}

RingModAudioProcessorEditor::~RingModAudioProcessorEditor()
{
    Timer::stopTimer();
}

//==============================================================================
void RingModAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::darkgrey.darker());
    
    juce::Point<float> centrePoint = getLocalBounds().getCentre().toFloat();
    juce::Point<float> ledCentrePoint = centrePoint.translated(0, -getHeight() / 3);
    juce::Rectangle<float> windowArea = getLocalBounds().toFloat();
    juce::Rectangle <float> led (10, 10, 10, 10);
    
    led.setCentre(ledCentrePoint);
    
    windowArea.setWidth(windowArea.getWidth() / 2.75f);
    windowArea.setHeight(windowArea.getHeight() / 1.15f);
    windowArea.setCentre(centrePoint);
    
    g.setColour(juce::Colours::lightgrey);
    g.fillRoundedRectangle(windowArea, 20.f);
   
    g.setColour(btnState ? juce::Colours::black.interpolatedWith(juce::Colours::red, interpolationValue) : juce::Colours::black);
    g.fillEllipse(led);
}

void RingModAudioProcessorEditor::resized()
{
    juce::Point<int> rsCentrePoint = getLocalBounds().getCentre();
    juce::Point<int> rsLabelCentrePoint = rsCentrePoint.translated(0, -getHeight() / 4.25);
    
    juce::Rectangle<int> rsWindowArea = getLocalBounds();
    juce::Rectangle<int> rsLabelPos = rsWindowArea;
    juce::Rectangle<int> rsNut = getLocalBounds();
    
    rsLabelPos.setWidth(rsLabelPos.getWidth() / 2.75f);
    rsLabelPos.setHeight(getHeight() / 6);
    rsLabelPos.setCentre(rsLabelCentrePoint);
    rsLabelPos.reduce(10.f, 10.f);
    
    rsWindowArea.setWidth(getWidth() / 4);
    rsWindowArea.setHeight(getWidth() / 4);
    rsWindowArea.setCentre(rsCentrePoint);
    
    rsNut.setWidth(getWidth() / 8.5);
    rsNut.setHeight(getWidth() / 8.5);
    rsNut.setCentre(rsCentrePoint);
    rsNut.translate(0, rsCentrePoint.y / 2);
    
    rmLabel.setBounds(rsLabelPos);
    rateDial.setBounds(rsWindowArea);
    bypassButton.setBounds(rsNut);
}

void RingModAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &rateDial)
    {
        audioProcessor.frequency = rateDial.getValue();
    }
}

void RingModAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    if (button == &bypassButton)
    {
        if (btnState)
        {
            btnState = false;
            audioProcessor.on = false;
        }
        
        else
        {
            btnState = true;
            audioProcessor.on = true;
        }
    }
}

void RingModAudioProcessorEditor::timerCallback()
{
    repaint();
    interpolationValue = getColourInterpVal.retrieveValue();
}
