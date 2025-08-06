#include "PluginProcessor.h"
#include "PluginEditor.h"

Pulse24SyncAudioProcessorEditor::Pulse24SyncAudioProcessorEditor(Pulse24SyncAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(500, 800); // Increased size for new controls
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
    g.setFont(22.0f);
    g.drawText("Enhanced MIDI Sync", getLocalBounds().removeFromTop(40), juce::Justification::centred);

    // Draw subtitle
    g.setFont(12.0f);
    g.setColour(juce::Colours::lightgrey);
    g.drawText("Professional MIDI Clock & Transport Control", getLocalBounds().removeFromTop(60).removeFromTop(20), juce::Justification::centred);

    // Draw section separators
    g.setColour(juce::Colours::darkgrey);
    g.drawLine(20, 180, getWidth() - 20, 180, 1.0f);  // After basic controls
    g.drawLine(20, 420, getWidth() - 20, 420, 1.0f);  // After sync controls
    g.drawLine(20, 580, getWidth() - 20, 580, 1.0f);  // After transport controls
}

void Pulse24SyncAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    bounds.removeFromTop(80); // Space for title

    // Status label at top
    statusLabel.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);

    // === BASIC CONTROLS SECTION ===
    auto basicSection = bounds.removeFromTop(180);
    
    // Enabled button
    enabledButton.setBounds(basicSection.removeFromTop(30));
    basicSection.removeFromTop(5);

    // Velocity slider
    velocityLabel.setBounds(basicSection.removeFromTop(20));
    velocitySlider.setBounds(basicSection.removeFromTop(40));
    basicSection.removeFromTop(5);

    // Channel slider
    channelLabel.setBounds(basicSection.removeFromTop(20));
    channelSlider.setBounds(basicSection.removeFromTop(40));
    basicSection.removeFromTop(5);

    bounds.removeFromTop(15); // Section separator space

    // === TIMING & SYNC SECTION ===
    auto syncSection = bounds.removeFromTop(230);

    // Sync to host button
    syncToHostButton.setBounds(syncSection.removeFromTop(30));
    syncSection.removeFromTop(5);

    // Manual BPM slider
    manualBPMLabel.setBounds(syncSection.removeFromTop(20));
    manualBPMSlider.setBounds(syncSection.removeFromTop(40));
    syncSection.removeFromTop(10);

    // PPQN Resolution
    ppqnLabel.setBounds(syncSection.removeFromTop(20));
    ppqnComboBox.setBounds(syncSection.removeFromTop(30));
    syncSection.removeFromTop(10);

    // Advanced sync options
    sendClockWhenStoppedButton.setBounds(syncSection.removeFromTop(25));
    sendSPPButton.setBounds(syncSection.removeFromTop(25));
    useMidiClockButton.setBounds(syncSection.removeFromTop(25));

    bounds.removeFromTop(15); // Section separator space

    // === MMC CONTROLS SECTION ===
    auto mmcSection = bounds.removeFromTop(150);

    mmcEnabledButton.setBounds(mmcSection.removeFromTop(30));
    mmcSection.removeFromTop(5);

    sendMMCCommandsButton.setBounds(mmcSection.removeFromTop(25));
    mmcSection.removeFromTop(5);

    mmcDeviceLabel.setBounds(mmcSection.removeFromTop(20));
    mmcDeviceSlider.setBounds(mmcSection.removeFromTop(40));

    bounds.removeFromTop(15); // Section separator space

    // === TRANSPORT CONTROLS SECTION ===
    auto transportSection = bounds.removeFromTop(80);
    auto buttonBounds = transportSection.removeFromTop(35);
    
    auto resetBounds = buttonBounds.removeFromLeft(buttonBounds.getWidth() / 2);
    resetBounds.reduce(5, 0);
    resetButton.setBounds(resetBounds);
    
    buttonBounds.reduce(5, 0);
    resyncButton.setBounds(buttonBounds);

    transportSection.removeFromTop(10);

    // === STATUS INFO SECTION ===
    transportStateLabel.setBounds(bounds.removeFromTop(25));
    songPositionLabel.setBounds(bounds.removeFromTop(25));
    ppqnInfoLabel.setBounds(bounds.removeFromTop(25));
}

void Pulse24SyncAudioProcessorEditor::setupUI()
{
    // Set up colors and styles
    auto primaryColour = juce::Colour(0xff4a9eff);
    auto secondaryColour = juce::Colour(0xff2d2d2d);

    // === BASIC CONTROLS ===
    // Status label
    addAndMakeVisible(statusLabel);
    statusLabel.setFont(14.0f);
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
    statusLabel.setJustificationType(juce::Justification::centred);

    // Enabled button
    addAndMakeVisible(enabledButton);
    enabledButton.setButtonText("Plugin Enabled");
    enabledButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    enabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "enabled", enabledButton);

    // Velocity slider
    addAndMakeVisible(velocityLabel);
    velocityLabel.setText("MIDI Velocity", juce::dontSendNotification);
    velocityLabel.setFont(12.0f);
    velocityLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(velocitySlider);
    velocitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    velocitySlider.setColour(juce::Slider::thumbColourId, primaryColour);
    velocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "pulseVelocity", velocitySlider);

    // Channel slider
    addAndMakeVisible(channelLabel);
    channelLabel.setText("MIDI Channel", juce::dontSendNotification);
    channelLabel.setFont(12.0f);
    channelLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(channelSlider);
    channelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    channelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    channelSlider.setColour(juce::Slider::thumbColourId, primaryColour);
    channelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "pulseChannel", channelSlider);

    // === TIMING & SYNC CONTROLS ===
    // Sync to host button
    addAndMakeVisible(syncToHostButton);
    syncToHostButton.setButtonText("Sync to Host Tempo");
    syncToHostButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    syncToHostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "syncToHost", syncToHostButton);

    // Manual BPM slider
    addAndMakeVisible(manualBPMLabel);
    manualBPMLabel.setText("Manual BPM", juce::dontSendNotification);
    manualBPMLabel.setFont(12.0f);
    manualBPMLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(manualBPMSlider);
    manualBPMSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    manualBPMSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    manualBPMSlider.setColour(juce::Slider::thumbColourId, primaryColour);
    manualBPMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "manualBPM", manualBPMSlider);

    // PPQN Resolution combo box
    addAndMakeVisible(ppqnLabel);
    ppqnLabel.setText("Clock Resolution", juce::dontSendNotification);
    ppqnLabel.setFont(12.0f);
    ppqnLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(ppqnComboBox);
    ppqnComboBox.addItemList(juce::StringArray{"24 PPQN (Standard)", "48 PPQN (Korg)", "96 PPQN (Oberheim)", "192 PPQN (Modern)", "480 PPQN (High-Res)"}, 1);
    ppqnComboBox.setColour(juce::ComboBox::backgroundColourId, secondaryColour);
    ppqnComboBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
    ppqnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.parameters, "ppqnResolution", ppqnComboBox);

    // Advanced sync options
    addAndMakeVisible(sendClockWhenStoppedButton);
    sendClockWhenStoppedButton.setButtonText("Send Clock When Stopped");
    sendClockWhenStoppedButton.setColour(juce::ToggleButton::textColourId, juce::Colours::lightyellow);
    sendClockWhenStoppedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "sendClockWhenStopped", sendClockWhenStoppedButton);

    addAndMakeVisible(sendSPPButton);
    sendSPPButton.setButtonText("Send Song Position Pointer");
    sendSPPButton.setColour(juce::ToggleButton::textColourId, juce::Colours::lightyellow);
    sendSPPAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "sendSPP", sendSPPButton);

    addAndMakeVisible(useMidiClockButton);
    useMidiClockButton.setButtonText("Use MIDI Clock (vs Note Pulses)");
    useMidiClockButton.setColour(juce::ToggleButton::textColourId, juce::Colours::lightyellow);
    useMidiClockAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "useMidiClock", useMidiClockButton);

    // === MMC CONTROLS ===
    addAndMakeVisible(mmcEnabledButton);
    mmcEnabledButton.setButtonText("MIDI Machine Control Enabled");
    mmcEnabledButton.setColour(juce::ToggleButton::textColourId, juce::Colours::orange);
    mmcEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "mmcEnabled", mmcEnabledButton);

    addAndMakeVisible(sendMMCCommandsButton);
    sendMMCCommandsButton.setButtonText("Send MMC Commands");
    sendMMCCommandsButton.setColour(juce::ToggleButton::textColourId, juce::Colours::orange);
    sendMMCCommandsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "sendMMCCommands", sendMMCCommandsButton);

    addAndMakeVisible(mmcDeviceLabel);
    mmcDeviceLabel.setText("MMC Device ID (127 = All)", juce::dontSendNotification);
    mmcDeviceLabel.setFont(12.0f);
    mmcDeviceLabel.setColour(juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible(mmcDeviceSlider);
    mmcDeviceSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mmcDeviceSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    mmcDeviceSlider.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    mmcDeviceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "mmcDeviceID", mmcDeviceSlider);

    // === TRANSPORT CONTROLS ===
    addAndMakeVisible(resetButton);
    resetButton.setButtonText("Reset");
    resetButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    resetButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    resetButton.onClick = [this]() { audioProcessor.pulseGenerator.forceReset(); };

    addAndMakeVisible(resyncButton);
    resyncButton.setButtonText("Resync");
    resyncButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgreen);
    resyncButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    resyncButton.onClick = [this]() { audioProcessor.pulseGenerator.requestResync(); };

    // === STATUS DISPLAY ===
    addAndMakeVisible(transportStateLabel);
    transportStateLabel.setFont(12.0f);
    transportStateLabel.setColour(juce::Label::textColourId, juce::Colours::lightblue);

    addAndMakeVisible(songPositionLabel);
    songPositionLabel.setFont(12.0f);
    songPositionLabel.setColour(juce::Label::textColourId, juce::Colours::lightblue);

    addAndMakeVisible(ppqnInfoLabel);
    ppqnInfoLabel.setFont(12.0f);
    ppqnInfoLabel.setColour(juce::Label::textColourId, juce::Colours::lightblue);
}

void Pulse24SyncAudioProcessorEditor::timerCallback()
{
    updateStatus();
}

void Pulse24SyncAudioProcessorEditor::updateStatus()
{
    auto& pulseGen = audioProcessor.pulseGenerator;

    // Main status
    juce::String statusText;
    if (!pulseGen.getEnabled())
    {
        statusText = "DISABLED";
    }
    else if (!pulseGen.getSyncToHost())
    {
        statusText = "Manual Mode - " + juce::String(pulseGen.getManualBPM(), 1) + " BPM";
    }
    else
    {
        statusText = "Host Sync - " + juce::String(pulseGen.getCurrentBPM(), 1) + " BPM";
    }

    statusText += " | " + juce::String(pulseGen.getPulseRate(), 1) + " pulses/sec";
    statusLabel.setText(statusText, juce::dontSendNotification);

    // Transport state
    juce::String transportText = "Transport: ";
    switch (pulseGen.getTransportState())
    {
        case TransportState::Stopped: transportText += "Stopped"; break;
        case TransportState::Playing: transportText += "Playing"; break;
        case TransportState::Paused: transportText += "Paused"; break;
        case TransportState::Recording: transportText += "Recording"; break;
    }
    transportStateLabel.setText(transportText, juce::dontSendNotification);

    // Song position
    songPositionLabel.setText("Song Position: " + juce::String(pulseGen.getCurrentSongPosition()) + " MIDI beats", 
                             juce::dontSendNotification);

    // PPQN info
    int ppqnValue = static_cast<int>(pulseGen.getPPQNResolution());
    ppqnInfoLabel.setText("Current Resolution: " + juce::String(ppqnValue) + " PPQN", 
                         juce::dontSendNotification);
}
