/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
CmsynthAudioProcessorEditor::CmsynthAudioProcessorEditor (CmsynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize(730, 400);

	// Set slider and label parameters
	fmSlider.setSliderStyle(Slider::Rotary);
	fmSlider.setRange(0.0, 10.0e3);
	fmSlider.setTextValueSuffix(" Hz");
	fmSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 150, fmSlider.getTextBoxHeight());
	fmSlider.setSkewFactorFromMidPoint(316.0);
	fmSlider.setValue(100.0);
	fmLabel.setText("Modulation frequency", dontSendNotification);
	fmLabel.setJustificationType(Justification::centred);
	fmLabel.attachToComponent(&fmSlider, false);

	amSlider.setSliderStyle(Slider::Rotary);
	amSlider.setRange(0.0, 0.99);
	amSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 150, amSlider.getTextBoxHeight());
	amSlider.setValue(0.1);
	amLabel.setText("Modulation index", dontSendNotification);
	amLabel.setJustificationType(Justification::centred);
	amLabel.attachToComponent(&amSlider, false);

	NSlider.setSliderStyle(Slider::Rotary);
	NSlider.setRange(1.0, 1000.0, 1.0);
	NSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 150, NSlider.getTextBoxHeight());
	NSlider.setValue(1.0);
	NSlider.setVelocityBasedMode(true);
	NSlider.setVelocityModeParameters(10.0, 50, 0.0, false);
	NLabel.setText("Number of stages", dontSendNotification);
	NLabel.setJustificationType(Justification::centred);
	NLabel.attachToComponent(&NSlider, false);

	// Display sliders and labels
	addAndMakeVisible(&fmSlider);
	addAndMakeVisible(&fmLabel);
	addAndMakeVisible(&amSlider);
	addAndMakeVisible(&amLabel);
	addAndMakeVisible(&NSlider);
	addAndMakeVisible(&NLabel);

	// Add listeners to sliders
	fmSlider.addListener(this);
	amSlider.addListener(this);
	NSlider.addListener(this);
}

CmsynthAudioProcessorEditor::~CmsynthAudioProcessorEditor()
{
}

//==============================================================================
void CmsynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void CmsynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	fmSlider.setBounds(20, 100, 200, 200);
	amSlider.setBounds(250, 100, 200, 200);
	NSlider.setBounds(480, 100, 200, 200);
}

void CmsynthAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &fmSlider)
		processor.fm.setValue((float)fmSlider.getValue());
	else if (slider == &amSlider)
		processor.am.setValue((float)amSlider.getValue());
}

void CmsynthAudioProcessorEditor::sliderDragEnded(Slider * slider)
{
	if (slider == &NSlider)
		processor.nStages.setValue((float)NSlider.getValue());
}
