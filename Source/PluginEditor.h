#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class Pulse24SyncAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    Pulse24SyncAudioProcessorEditor(Pulse24SyncAudioProcessor&);
    ~Pulse24SyncAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    Pulse24SyncAudioProcessor& audioProcessor;

    // UI Components
    juce::ToggleButton enabledButton;
    juce::Slider velocitySlider;
    juce::Slider channelSlider;
    juce::ComboBox syncModeComboBox;
    juce::Slider manualBPMSlider;

#ifdef ABLETON_LINK_ENABLED
    juce::ToggleButton linkEnabledButton;
    juce::Label linkPeerCountLabel;
#endif

    // Labels
    juce::Label enabledLabel;
    juce::Label velocityLabel;
    juce::Label channelLabel;
    juce::Label syncModeLabel;
    juce::Label manualBPMLabel;
    juce::Label titleLabel;
    juce::Label statusLabel;

#ifdef ABLETON_LINK_ENABLED
    juce::Label linkEnabledLabel;
    juce::Label linkPeerCountDescLabel;
#endif

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> channelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> syncModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> manualBPMAttachment;

#ifdef ABLETON_LINK_ENABLED
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> linkEnabledAttachment;
#endif

    void setupUI();
    void updateStatus();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pulse24SyncAudioProcessorEditor)
};
