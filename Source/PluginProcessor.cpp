/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RingModAudioProcessor::RingModAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

RingModAudioProcessor::~RingModAudioProcessor()
{
}

//==============================================================================
const juce::String RingModAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RingModAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RingModAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RingModAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RingModAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RingModAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RingModAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RingModAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RingModAudioProcessor::getProgramName (int index)
{
    return {};
}

void RingModAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RingModAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    frequency = 440;
    waveTableSize = 1024;
    phase = 0;
    increment = frequency * waveTableSize / sampleRate;
    amp = 0.5f;
    
    for (int i = 0; i < waveTableSize; i++)
    {
        waveTable.insert(i, sin(2.0 * juce::MathConstants<double>::pi * i / waveTableSize));
    }
}

void RingModAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RingModAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RingModAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* left = buffer.getWritePointer(0, 0);
    auto* right = buffer.getWritePointer(1, 0);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        left[sample] *= waveTable[(int)phase] * amp;
        right[sample] *= waveTable[(int)phase] * amp;
        phase = fmod ((phase + increment), waveTableSize);
    }
}

//==============================================================================
bool RingModAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RingModAudioProcessor::createEditor()
{
    return new RingModAudioProcessorEditor (*this);
}

//==============================================================================
void RingModAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RingModAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RingModAudioProcessor();
}
