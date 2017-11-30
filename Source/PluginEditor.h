/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class CmsynthAudioProcessorEditor  : public AudioProcessorEditor,
									 private Slider::Listener
{
public:
    CmsynthAudioProcessorEditor (CmsynthAudioProcessor&);
    ~CmsynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CmsynthAudioProcessor& processor;

	void sliderValueChanged(Slider* slider) override;
	void sliderDragEnded(Slider * slider) override;

	// Create sliders and labels for parameters

	Slider fmSlider;
	Label fmLabel;
	Slider amSlider;
	Label amLabel;
	Slider NSlider;
	Label NLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CmsynthAudioProcessorEditor)
};
