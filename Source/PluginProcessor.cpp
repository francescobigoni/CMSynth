/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
CmsynthAudioProcessor::CmsynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
}

CmsynthAudioProcessor::~CmsynthAudioProcessor()
{
}

//==============================================================================
const String CmsynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CmsynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CmsynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CmsynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CmsynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CmsynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CmsynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CmsynthAudioProcessor::setCurrentProgram (int)
{
}

const String CmsynthAudioProcessor::getProgramName (int)
{
    return {};
}

void CmsynthAudioProcessor::changeProgramName (int, const String&)
{
}

//==============================================================================
void CmsynthAudioProcessor::prepareToPlay (double sampleRate, int)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	fm.reset(sampleRate, 1e-2);
	fm.setValue((float)100.0);
	am.reset(sampleRate, 1e-2);
	am.setValue((float)0.1);
	nStages.reset(sampleRate, 1e-2);
	nStages.setValue((float)1.0);
	phaseM = 0.0;
	in = 0.0;
	out = 0.0;
	updateDeltaPhase();
}

void CmsynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CmsynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void CmsynthAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
	const float* carrier = buffer.getReadPointer(0);

	float* channelData = buffer.getWritePointer(0);

	float modulator;

	for (int i = 0; i < buffer.getNumSamples(); i++)
	{
		updateDeltaPhase();
		modulator = am.getNextValue() * std::sin(phaseM);
		in = carrier[i];

		for (int j = 0; j < std::floor(nStages.getNextValue()); j++)
		{
			out = delayBufferIn[j] + modulator * (in - delayBufferOut[j]);
			delayBufferIn[j] = in;
			delayBufferOut[j] = out;
			in = out;
		}

		channelData[i] = out;
		phaseM += deltaPhaseM;
	}

	phaseM = (float)std::fmod(phaseM, 2 * double_Pi);
}

void CmsynthAudioProcessor::updateDeltaPhase()
{
	deltaPhaseM = 2 * float_Pi * (float)fm.getNextValue() / (float)getSampleRate();
}

//==============================================================================
bool CmsynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CmsynthAudioProcessor::createEditor()
{
    return new CmsynthAudioProcessorEditor (*this);
}

//==============================================================================
void CmsynthAudioProcessor::getStateInformation (MemoryBlock&)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CmsynthAudioProcessor::setStateInformation (const void*, int)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CmsynthAudioProcessor();
}
