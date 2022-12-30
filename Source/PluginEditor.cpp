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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
