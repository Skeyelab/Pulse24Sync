#include "PluginProcessor.h"
#include "PluginEditor.h"

Pulse24SyncAudioProcessorEditor::Pulse24SyncAudioProcessorEditor(Pulse24SyncAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
#ifdef ABLETON_LINK_ENABLED
    setSize(400, 600); // Increased height for Link controls
#else
    setSize(400, 550); // Slightly increased for sync mode combo box
#endif
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

    // Channel slider
    channelLabel.setBounds(bounds.removeFromTop(20));
    channelSlider.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(10);

    // Sync mode combo box
    syncModeLabel.setBounds(bounds.removeFromTop(20));
    syncModeComboBox.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);

    // Manual BPM slider
    manualBPMLabel.setBounds(bounds.removeFromTop(20));
    manualBPMSlider.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(10);

#ifdef ABLETON_LINK_ENABLED
    // Link enabled button
    linkEnabledButton.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);

    // Link peer count
    auto peerCountBounds = bounds.removeFromTop(25);
    linkPeerCountDescLabel.setBounds(peerCountBounds.removeFromLeft(80));
    linkPeerCountLabel.setBounds(peerCountBounds);
    bounds.removeFromTop(10);
#endif
}

void Pulse24SyncAudioProcessorEditor::setupUI()
{
    // Title label
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Pulse24Sync", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);

    // Status label
    addAndMakeVisible(statusLabel);
    statusLabel.setText("Status: Ready", juce::dontSendNotification);
    statusLabel.setFont(juce::Font(12.0f));
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
    statusLabel.setJustificationType(juce::Justification::centred);

    // Enabled button
    addAndMakeVisible(enabledButton);
    enabledButton.setButtonText("Enabled");
    enabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "enabled", enabledButton);

    // Velocity slider
    addAndMakeVisible(velocityLabel);
    velocityLabel.setText("Pulse Velocity", juce::dontSendNotification);
    velocityLabel.setFont(juce::Font(12.0f));
    velocityLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    velocityLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(velocitySlider);
    velocitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    velocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    velocityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "pulseVelocity", velocitySlider);

    // Channel slider
    addAndMakeVisible(channelLabel);
    channelLabel.setText("MIDI Channel", juce::dontSendNotification);
    channelLabel.setFont(juce::Font(12.0f));
    channelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    channelLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(channelSlider);
    channelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    channelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    channelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "pulseChannel", channelSlider);

    // Sync mode combo box
    addAndMakeVisible(syncModeLabel);
    syncModeLabel.setText("Sync Mode", juce::dontSendNotification);
    syncModeLabel.setFont(juce::Font(12.0f));
    syncModeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    syncModeLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(syncModeComboBox);
    syncModeComboBox.addItem("Host", 1);
    syncModeComboBox.addItem("Manual", 2);
    syncModeComboBox.addItem("Link", 3);
    syncModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.parameters, "syncMode", syncModeComboBox);

    // Manual BPM slider
    addAndMakeVisible(manualBPMLabel);
    manualBPMLabel.setText("Manual BPM", juce::dontSendNotification);
    manualBPMLabel.setFont(juce::Font(12.0f));
    manualBPMLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    manualBPMLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(manualBPMSlider);
    manualBPMSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    manualBPMSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    manualBPMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "manualBPM", manualBPMSlider);

#ifdef ABLETON_LINK_ENABLED
    // Link enabled button
    addAndMakeVisible(linkEnabledButton);
    linkEnabledButton.setButtonText("Link Enabled");
    linkEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "linkEnabled", linkEnabledButton);

    // Link peer count labels
    addAndMakeVisible(linkPeerCountDescLabel);
    linkPeerCountDescLabel.setText("Link Peers:", juce::dontSendNotification);
    linkPeerCountDescLabel.setFont(juce::Font(12.0f));
    linkPeerCountDescLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    linkPeerCountDescLabel.setJustificationType(juce::Justification::centredLeft);

    addAndMakeVisible(linkPeerCountLabel);
    linkPeerCountLabel.setText("0", juce::dontSendNotification);
    linkPeerCountLabel.setFont(juce::Font(12.0f));
    linkPeerCountLabel.setColour(juce::Label::textColourId, juce::Colours::lightblue);
    linkPeerCountLabel.setJustificationType(juce::Justification::centredLeft);
#endif
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
    else
    {
        auto syncMode = pulseGen.getSyncMode();
        switch (syncMode)
        {
            case PulseGenerator::SyncToHost:
                statusText += "Host Sync - " + juce::String(pulseGen.getCurrentBPM(), 1) + " BPM";
                statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
                break;
            case PulseGenerator::SyncToManual:
                statusText += "Manual Mode - " + juce::String(pulseGen.getCurrentBPM(), 1) + " BPM";
                statusLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
                break;
            case PulseGenerator::SyncToLink:
#ifdef ABLETON_LINK_ENABLED
                if (pulseGen.isLinkEnabled())
                {
                    statusText += "Link Sync - " + juce::String(pulseGen.getCurrentBPM(), 1) + " BPM";
                    statusLabel.setColour(juce::Label::textColourId, juce::Colours::lightblue);
                }
                else
                {
                    statusText += "Link Mode (Disabled) - " + juce::String(pulseGen.getCurrentBPM(), 1) + " BPM";
                    statusLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
                }
                
                // Update peer count
                linkPeerCountLabel.setText(juce::String(pulseGen.getLinkPeerCount()), juce::dontSendNotification);
#else
                statusText += "Link Not Available";
                statusLabel.setColour(juce::Label::textColourId, juce::Colours::red);
#endif
                break;
        }
    }

    statusText += " | Rate: " + juce::String(pulseGen.getPulseRate(), 1) + " Hz";

    statusLabel.setText(statusText, juce::dontSendNotification);
}
