#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_plugin_client/juce_audio_plugin_client.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
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
    juce::ToggleButton syncToHostButton;
    juce::Slider manualBPMSlider;

    // Labels
    juce::Label enabledLabel;
    juce::Label velocityLabel;
    juce::Label channelLabel;
    juce::Label syncToHostLabel;
    juce::Label manualBPMLabel;
    juce::Label titleLabel;
    juce::Label statusLabel;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> channelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncToHostAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> manualBPMAttachment;

    void setupUI();
    void updateStatus();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pulse24SyncAudioProcessorEditor)
};
