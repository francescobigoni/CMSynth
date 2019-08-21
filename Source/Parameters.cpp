/*
  ==============================================================================

    Parameters.cpp
    Created: 21 Aug 2019 7:47:07pm
    Author:  franc

  ==============================================================================
*/

#include "Parameters.h"

Parameters::Parameters(AudioProcessor& processor) : valueTree(processor, nullptr, "Parameters",
	{
		std::make_unique<AudioParameterFloat>("fm",
		"Modulation Frequency",
		NormalisableRange<float>(0.0, 100.0, 0.001, 0.199),
		1.0),

		std::make_unique<AudioParameterFloat>("am",
		"Modulation Index",
		NormalisableRange<float>(0.0, 0.99),
		0.1),

		std::make_unique<AudioParameterFloat>("nStages",
		"Number of Stages",
		NormalisableRange<float>(1.0, 200.0, 0, 0.5),
		1.0)
	})
{

}

void Parameters::update()
{
	fm.setTargetValue(*valueTree.getRawParameterValue("fm"));
	am.setTargetValue(*valueTree.getRawParameterValue("am"));
	nStages.setTargetValue(*valueTree.getRawParameterValue("nStages"));
}