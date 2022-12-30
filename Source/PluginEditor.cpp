/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModAudioProcessorEditor::RingModAudioProcessorEditor (RingModAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(rateDial);
    rateDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    rateDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    rateDial.setRange(20, 4000, 0.01);
    rateDial.setValue(20);
    rateDial.addListener(this);
    
    rateDial.setLookAndFeel(&rateDialLookAndFeel);
    
    addAndMakeVisible(bypassButton);
    bypassButton.setLookAndFeel(&bypassBtnLookAndFeel);
    
    setSize (400, 300);
}

RingModAudioProcessorEditor::~RingModAudioProcessorEditor()
{
}

//==============================================================================
void RingModAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    
    juce::Point<float> centrePoint = getLocalBounds().getCentre().toFloat();
    juce::Rectangle<float> windowArea = getLocalBounds().toFloat();
    
    windowArea.setWidth(windowArea.getWidth() / 2.75f);
    windowArea.setHeight(windowArea.getHeight() / 1.15f);
    windowArea.setCentre(centrePoint);
    
    g.setColour(juce::Colours::lightgrey);
    g.fillRoundedRectangle(windowArea, 20.f);
}

void RingModAudioProcessorEditor::resized()
{
    juce::Point<int> rsCentrePoint = getLocalBounds().getCentre();
    juce::Rectangle<int> rsWindowArea = getLocalBounds();
    
    rsWindowArea.setWidth(getWidth() / 4);
    rsWindowArea.setHeight(getWidth() / 4);
    rsWindowArea.setCentre(rsCentrePoint);
    
    rateDial.setBounds(rsWindowArea);
    bypassButton.setBounds(20, 20, 100, 100);
}

void RingModAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &rateDial)
    {
        audioProcessor.frequency = rateDial.getValue();
    }
}
