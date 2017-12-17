/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Parameters.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class CmsynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    CmsynthAudioProcessorEditor (CmsynthAudioProcessor&, Parameters&);
    ~CmsynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CmsynthAudioProcessor& processor;
	AudioProcessorValueTreeState& valueTreeState;

	ScopedPointer<SliderAttachment> fmAttachment;
	ScopedPointer<SliderAttachment> amAttachment;
	ScopedPointer<SliderAttachment> nStagesAttachment;

	// Create sliders and labels for parameters

	Slider fmSlider;
	Label fmLabel;
	Slider amSlider;
	Label amLabel;
	Slider nStagesSlider;
	Label nStagesLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CmsynthAudioProcessorEditor)
};
