/*
  ==============================================================================

    Parameters.h
    Created: 17 Dec 2017 1:22:07pm
    Author:  franc

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

struct Parameters
{
	Parameters(AudioProcessor &processor);
	void update();

	LinearSmoothedValue<float> fm;
	LinearSmoothedValue<float> am;
	LinearSmoothedValue<float> nStages;

	AudioProcessorValueTreeState valueTree;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)
};