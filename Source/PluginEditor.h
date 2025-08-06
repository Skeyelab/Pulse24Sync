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
    void setupUI();
    void updateStatus();

    Pulse24SyncAudioProcessor& audioProcessor;

    // Basic controls
    juce::Label titleLabel;
    juce::Label statusLabel;
    juce::ToggleButton enabledButton;
    juce::Label velocityLabel;
    juce::Slider velocitySlider;
    juce::Label channelLabel;
    juce::Slider channelSlider;
    juce::ToggleButton syncToHostButton;
    juce::Label manualBPMLabel;
    juce::Slider manualBPMSlider;

    // Enhanced sync controls
    juce::Label ppqnLabel;
    juce::ComboBox ppqnComboBox;
    juce::Label mmcDeviceLabel;
    juce::Slider mmcDeviceSlider;
    juce::ToggleButton mmcEnabledButton;
    juce::ToggleButton sendClockWhenStoppedButton;
    juce::ToggleButton sendMMCCommandsButton;
    juce::ToggleButton sendSPPButton;
    juce::ToggleButton useMidiClockButton;
    juce::ToggleButton useNotePulsesButton;

    // Transport controls
    juce::TextButton resetButton;
    juce::TextButton resyncButton;

    // Advanced info display
    juce::Label transportStateLabel;
    juce::Label songPositionLabel;
    juce::Label ppqnInfoLabel;

    // Parameter attachments for basic controls
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> channelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncToHostAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> manualBPMAttachment;

    // Parameter attachments for enhanced controls
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ppqnAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mmcDeviceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mmcEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sendClockWhenStoppedAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sendMMCCommandsAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sendSPPAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> useMidiClockAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> useNotePulsesAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pulse24SyncAudioProcessorEditor)
};
