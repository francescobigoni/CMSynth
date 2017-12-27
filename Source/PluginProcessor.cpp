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
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#elif
	:
#endif
parameters(*this)
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
	parameters.fm.reset(sampleRate, 0.01);
	parameters.am.reset(sampleRate, 0.01);
	parameters.nStages.reset(sampleRate, 0.01);
	phaseM = 0.0;
	in = 0.0;
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

	float modulator;

	for (int i = 0; i < buffer.getNumSamples(); i++)
	{
		parameters.update();
		updateDeltaPhase();
		modulator = parameters.am.getNextValue() * std::sin(phaseM);
		in = carrier[i];
		
		float currentNStages = parameters.nStages.getNextValue();
		int currentNStagesInt = (int)std::floor(currentNStages);
		float currentNStagesFrac = currentNStages - currentNStagesInt;

		for (int j = 0; j < currentNStagesInt; j++)
		{
			out[j] = delayBufferIn[j] + modulator * (in - delayBufferOut[j]);
			delayBufferIn[j] = in;
			delayBufferOut[j] = out[j];
			in = out[j];
		}
		
		if (currentNStagesFrac != 0)
		{
			int j = currentNStagesInt;
			out[j] = delayBufferIn[j] + modulator * (in - delayBufferOut[j]);
			delayBufferIn[j] = in;
			delayBufferOut[j] = out[j];

			out[j] = (1 - currentNStagesFrac) * delayBufferIn[j - 1] + currentNStagesFrac *  delayBufferIn[j]
				+ modulator * (in - ((1 - currentNStagesFrac) * delayBufferOut[j - 1] + currentNStagesFrac * delayBufferOut[j]));
			
			for (int channel = 0; channel < buffer.getNumChannels(); channel++)
			{
				float* channelData = buffer.getWritePointer(channel);
				channelData[i] = out[j];
			}
		}

		else
		{
			for (int channel = 0; channel < buffer.getNumChannels(); channel++)
			{
				float* channelData = buffer.getWritePointer(channel);
				channelData[i] = out[currentNStagesInt - 1];
			}
		}
		
		phaseM += deltaPhaseM;
	}

	if (phaseM > 2 * double_Pi)
		phaseM -= 2 * double_Pi;
}

void CmsynthAudioProcessor::updateDeltaPhase()
{
	deltaPhaseM = 2 * float_Pi * (float)parameters.fm.getNextValue() / (float)getSampleRate();
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
