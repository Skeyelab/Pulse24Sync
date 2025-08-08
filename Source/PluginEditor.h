#pragma once

// Pulse24SyncAudioProcessorEditor
// - Minimal UI that binds controls to parameters (see Parameters.h)
// - Displays a status line (enabled, sync mode, BPM, pulse rate)
// - Uses a timer to periodically pull state from the engine for UX feedback

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"

class Pulse24SyncAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    Pulse24SyncAudioProcessorEditor(Pulse24SyncAudioProcessor&);
    ~Pulse24SyncAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override; // Periodically updates status label from engine state

private:
    Pulse24SyncAudioProcessor& audioProcessor;

    // UI Components
    juce::ToggleButton enabledButton;
    juce::Slider velocitySlider;
    juce::Slider pulseWidthSlider;  // Pulse width in ms
    juce::ToggleButton syncToHostButton;
    juce::Slider manualBPMSlider;

    // Labels
    juce::Label enabledLabel;
    juce::Label velocityLabel;
    juce::Label pulseWidthLabel;
    juce::Label syncToHostLabel;
    juce::Label manualBPMLabel;
    juce::Label titleLabel;
    juce::Label statusLabel;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pulseWidthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncToHostAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> manualBPMAttachment;

    void setupUI();   // Creates and binds UI controls to parameters
    void updateStatus(); // Renders a concise status line for users

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pulse24SyncAudioProcessorEditor)
};
