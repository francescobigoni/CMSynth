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
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
	, parameters(*this)
{
	currentPhaseM = 0.0;
	deltaPhaseM = 0.0;
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

void CmsynthAudioProcessor::setCurrentProgram (int index)
{
}

const String CmsynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void CmsynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CmsynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	parameters.fm.reset(sampleRate, 0.01);
	parameters.am.reset(sampleRate, 0.01);
	parameters.nStages.reset(sampleRate, 0.01);
	updateDeltaPhase(parameters.fm.getNextValue(), sampleRate);
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

void CmsynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

	for (int n = 0; n < buffer.getNumSamples(); n++)
	{
		float in;
		float modulator;

		parameters.update();
		updateDeltaPhase(parameters.fm.getNextValue(), getSampleRate());
		modulator = parameters.am.getNextValue() * std::sin(currentPhaseM);
		currentPhaseM += deltaPhaseM;
		
		float currentNStages = parameters.nStages.getNextValue();
		long currentNStagesInt = (long)(currentNStages);
		float currentNStagesFrac = currentNStages - currentNStagesInt;

		for (int channel = 0; channel < totalNumInputChannels; ++channel)
		{
			auto* carrier = buffer.getReadPointer(channel);
			auto* channelData = buffer.getWritePointer(channel);
			in = carrier[n];
			
			for (long j = 0; j < currentNStagesInt; j++)
			{
				out[channel][j] = delayBufferIn[channel][j] + modulator * (in - delayBufferOut[channel][j]);
				delayBufferIn[channel][j] = in;
				delayBufferOut[channel][j] = out[channel][j];
				in = out[channel][j];
			}

			if (currentNStagesFrac != 0)
			{
				long j = currentNStagesInt;
				out[channel][j] = delayBufferIn[channel][j] + modulator * (in - delayBufferOut[channel][j]);
				delayBufferIn[channel][j] = in;
				delayBufferOut[channel][j] = out[channel][j];

				out[channel][j] = (1 - currentNStagesFrac) * delayBufferIn[channel][j - 1] + currentNStagesFrac * delayBufferIn[channel][j]
					+ modulator * (in - ((1 - currentNStagesFrac) * delayBufferOut[channel][j - 1] + currentNStagesFrac * delayBufferOut[channel][j]));

				channelData[n] = out[channel][j];
			}

			else
				channelData[n] = out[channel][currentNStagesInt - 1];
		}
	}

	if (currentPhaseM > MathConstants<double>::twoPi)
		currentPhaseM -= MathConstants<double>::twoPi;
}

void CmsynthAudioProcessor::updateDeltaPhase(float newFrequency, double sampleRate)
{
	deltaPhaseM = MathConstants<double>::twoPi * (double)newFrequency / sampleRate;
}

//==============================================================================
bool CmsynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CmsynthAudioProcessor::createEditor()
{
    return new CmsynthAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void CmsynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CmsynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
