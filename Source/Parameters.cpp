/*
  ==============================================================================

    Parameters.cpp
    Created: 17 Dec 2017 1:22:01pm
    Author:  franc

  ==============================================================================
*/

#include "Parameters.h"

Parameters::Parameters(AudioProcessor &processor) : valueTree(processor, nullptr)
{
	valueTree.createAndAddParameter("fm",
		"Modulation frequency",
		"Hz",
		NormalisableRange<float>(0, 10.0e3),
		100.0,
		nullptr,
		nullptr);

	valueTree.createAndAddParameter("am",
		"Modulation amplitude",
		"",
		NormalisableRange<float>(0.0, 0.99),
		0.1,
		nullptr,
		nullptr);

	valueTree.createAndAddParameter("nStages",
		"Number of stages",
		"",
		NormalisableRange<float>(1.0, 1000.0),
		1.0,
		nullptr,
		nullptr);

	float sampleRate = processor.getSampleRate();
}

void Parameters::update()
{
	fm.setValue(*valueTree.getRawParameterValue("fm"));
	am.setValue(*valueTree.getRawParameterValue("am"));
	nStages.setValue(*valueTree.getRawParameterValue("nStages"));
}