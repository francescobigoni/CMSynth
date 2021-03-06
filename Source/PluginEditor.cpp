/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CmsynthAudioProcessorEditor::CmsynthAudioProcessorEditor (CmsynthAudioProcessor& p, Parameters& params)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(params.valueTree)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

	fmAttachment = std::make_unique<SliderAttachment>(valueTreeState, "fm", fmSlider);
	amAttachment = std::make_unique<SliderAttachment>(valueTreeState, "am", amSlider);
	nStagesAttachment = std::make_unique<SliderAttachment>(valueTreeState, "nStages", nStagesSlider);

	// Set slider and label parameters
	fmSlider.setSliderStyle(Slider::Rotary);
	fmSlider.setRange(0.0, 100.0);
	fmSlider.setTextValueSuffix(" Hz");
	fmSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 150, fmSlider.getTextBoxHeight());
	fmLabel.setText("Modulation Frequency", dontSendNotification);
	fmLabel.setJustificationType(Justification::centred);
	fmLabel.attachToComponent(&fmSlider, false);

	amSlider.setSliderStyle(Slider::Rotary);
	amSlider.setRange(0.0, 0.99);
	amSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 150, amSlider.getTextBoxHeight());
	amLabel.setText("Modulation Index", dontSendNotification);
	amLabel.setJustificationType(Justification::centred);
	amLabel.attachToComponent(&amSlider, false);

	nStagesSlider.setSliderStyle(Slider::Rotary);
	nStagesSlider.setRange(1.0, 200.0);
	nStagesSlider.setVelocityBasedMode(true);
	nStagesSlider.setVelocityModeParameters(10.0, 50, 0.0, false);
	nStagesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 150, nStagesSlider.getTextBoxHeight());
	nStagesLabel.setText("Number of Stages", dontSendNotification);
	nStagesLabel.setJustificationType(Justification::centred);
	nStagesLabel.attachToComponent(&nStagesSlider, false);

	// Display sliders and labels
	addAndMakeVisible(&fmSlider);
	addAndMakeVisible(&fmLabel);
	addAndMakeVisible(&amSlider);
	addAndMakeVisible(&amLabel);
	addAndMakeVisible(&nStagesSlider);
	addAndMakeVisible(&nStagesLabel);
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
	int marginX = 20;
	int marginY = 30;
	int x = marginX;
	int y = marginY;
	int size = 150;

	fmSlider.setBounds(x, y, size, size);
	x += fmSlider.getWidth() + marginX;
	amSlider.setBounds(x, y, size, size);
	x += amSlider.getWidth() + marginX;
	nStagesSlider.setBounds(x, y, size, size);

	int numX = 3;
	int numY = 1;
	setSize(size * numX + marginX * (numX + 1), size * numY + marginY * (numY + 1));
}
