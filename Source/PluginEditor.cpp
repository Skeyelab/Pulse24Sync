/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Pulse24SyncAudioProcessorEditor::Pulse24SyncAudioProcessorEditor (Pulse24SyncAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set window size
    setSize (400, 300);
    
    // Setup title label
    titleLabel.setText("Pulse24Sync", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);

    // Setup enabled button
    enabledLabel.setText("Enabled", juce::dontSendNotification);
    enabledLabel.setJustificationType(juce::Justification::centred);
    enabledLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(enabledLabel);
    addAndMakeVisible(enabledButton);

    // Setup volume slider
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    volumeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(volumeLabel);

    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    addAndMakeVisible(volumeSlider);

    // Setup width slider
    widthLabel.setText("Pulse Width", juce::dontSendNotification);
    widthLabel.setJustificationType(juce::Justification::centred);
    widthLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(widthLabel);

    widthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    widthSlider.setRange(0.01, 0.5, 0.01);
    widthSlider.setValue(0.1);
    addAndMakeVisible(widthSlider);

    // Setup parameter attachments
    enabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getParameters(), "enabled", enabledButton);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "volume", volumeSlider);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "width", widthSlider);
}

Pulse24SyncAudioProcessorEditor::~Pulse24SyncAudioProcessorEditor()
{
}

//==============================================================================
void Pulse24SyncAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background gradient
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xff1e1e1e),
        0.0f, 0.0f,
        juce::Colour(0xff2d2d2d),
        getWidth(), getHeight(),
        false));
    g.fillAll();

    // Draw border
    g.setColour(juce::Colour(0xff404040));
    g.drawRect(getLocalBounds(), 2);
}

void Pulse24SyncAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);

    // Title at the top
    titleLabel.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(20);

    // Enabled section
    auto enabledSection = bounds.removeFromTop(60);
    enabledLabel.setBounds(enabledSection.removeFromTop(20));
    enabledButton.setBounds(enabledSection.reduced(0, 10));

    bounds.removeFromTop(20);

    // Volume and width sliders side by side
    auto sliderSection = bounds.removeFromTop(120);
    auto volumeSection = sliderSection.removeFromLeft(sliderSection.getWidth() / 2);
    auto widthSection = sliderSection;

    volumeLabel.setBounds(volumeSection.removeFromTop(20));
    volumeSlider.setBounds(volumeSection);

    widthLabel.setBounds(widthSection.removeFromTop(20));
    widthSlider.setBounds(widthSection);
}