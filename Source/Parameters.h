/*
  ==============================================================================

    Parameters.h
    Created: 21 Aug 2019 7:47:07pm
    Author:  franc

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

struct Parameters
{
	Parameters(AudioProcessor& processor);

	void update();

	LinearSmoothedValue<float> fm;
	LinearSmoothedValue<float> am;
	LinearSmoothedValue<float> nStages;

	AudioProcessorValueTreeState valueTree;
};