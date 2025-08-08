#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

Pulse24SyncAudioProcessorEditor::Pulse24SyncAudioProcessorEditor(Pulse24SyncAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 500);
    setupUI();

    // Start timer for status updates
    startTimerHz(10); // Update 10 times per second
}

Pulse24SyncAudioProcessorEditor::~Pulse24SyncAudioProcessorEditor()
{
    stopTimer();
}

void Pulse24SyncAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));

    // Draw title
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Pulse24Sync", getLocalBounds().removeFromTop(40), juce::Justification::centred);

    // Draw subtitle
    g.setFont(12.0f);
    g.setColour(juce::Colours::lightgrey);
    g.drawText("24 Pulses per Quarter Note", getLocalBounds().removeFromTop(60).removeFromTop(20), juce::Justification::centred);
}

void Pulse24SyncAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    bounds.removeFromTop(80); // Space for title

    // Status label at top
    statusLabel.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);

    // Enabled button
    enabledButton.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);

    // Velocity slider
    velocityLabel.setBounds(bounds.removeFromTop(20));
    velocitySlider.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(10);

    // Pulse width slider
    pulseWidthLabel.setBounds(bounds.removeFromTop(20));
    pulseWidthSlider.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(10);

    // Sync to host button
    syncToHostButton.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);

    // Manual BPM slider
    manualBPMLabel.setBounds(bounds.removeFromTop(20));
    manualBPMSlider.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(10);
}

void Pulse24SyncAudioProcessorEditor::setupUI()
{
    auto styleLabel = [](juce::Label& label, const juce::String& text, juce::Colour colour) {
        label.setText(text, juce::dontSendNotification);
        label.setFont(juce::Font(12.0f));
        label.setColour(juce::Label::textColourId, colour);
        label.setJustificationType(juce::Justification::centred);
    };

    // Title label
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Pulse24Sync", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);

    // Status label
    addAndMakeVisible(statusLabel);
    styleLabel(statusLabel, "Status: Ready", juce::Colours::lightgreen);

    // Enabled button
    addAndMakeVisible(enabledButton);
    enabledButton.setButtonText("Enabled");
    enabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, PluginParams::enabled, enabledButton);

    // Velocity slider
    addAndMakeVisible(velocityLabel);
    styleLabel(velocityLabel, "Pulse Velocity", juce::Colours::white);

    addAndMakeVisible(velocitySlider);
    velocitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    velocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, PluginParams::pulseVelocity, velocitySlider);

    // Pulse width slider
    addAndMakeVisible(pulseWidthLabel);
    styleLabel(pulseWidthLabel, "Pulse Width", juce::Colours::white);

    addAndMakeVisible(pulseWidthSlider);
    pulseWidthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    pulseWidthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    pulseWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, PluginParams::pulseWidth, pulseWidthSlider);

    // Sync to host button
    addAndMakeVisible(syncToHostButton);
    syncToHostButton.setButtonText("Sync to Host Tempo");
    syncToHostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, PluginParams::syncToHost, syncToHostButton);

    // Manual BPM slider
    addAndMakeVisible(manualBPMLabel);
    styleLabel(manualBPMLabel, "Manual BPM", juce::Colours::white);

    addAndMakeVisible(manualBPMSlider);
    manualBPMSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    manualBPMSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    manualBPMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, PluginParams::manualBPM, manualBPMSlider);
}

void Pulse24SyncAudioProcessorEditor::timerCallback()
{
    updateStatus();
}

void Pulse24SyncAudioProcessorEditor::updateStatus()
{
    auto& pulseGen = audioProcessor.pulseGenerator;

    juce::String statusText = "Status: ";

    if (!pulseGen.getEnabled())
    {
        statusText += "Disabled";
        statusLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    }
    else if (!pulseGen.getSyncToHost())
    {
        statusText += "Manual Mode - " + juce::String(pulseGen.getManualBPM(), 1) + " BPM";
        statusLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    }
    else
    {
        statusText += "Host Sync - " + juce::String(pulseGen.getCurrentBPM(), 1) + " BPM";
        statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
    }

    statusText += " | Rate: " + juce::String(pulseGen.getPulseRate(), 1) + " Hz";

    statusLabel.setText(statusText, juce::dontSendNotification);
}
