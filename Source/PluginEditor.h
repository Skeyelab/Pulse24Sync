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
    juce::Slider pulseWidthSlider;  // Replaced channelSlider with pulseWidthSlider
    juce::ToggleButton syncToHostButton;
    juce::Slider manualBPMSlider;

    // Labels
    juce::Label enabledLabel;
    juce::Label velocityLabel;
    juce::Label pulseWidthLabel;  // Replaced channelLabel with pulseWidthLabel
    juce::Label syncToHostLabel;
    juce::Label manualBPMLabel;
    juce::Label titleLabel;
    juce::Label statusLabel;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pulseWidthAttachment;  // Replaced channelAttachment with pulseWidthAttachment
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncToHostAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> manualBPMAttachment;

    void setupUI();
    void updateStatus();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pulse24SyncAudioProcessorEditor)
};
