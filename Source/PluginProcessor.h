/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Pulse24SyncAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Pulse24SyncAudioProcessor();
    ~Pulse24SyncAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Pulse generation methods
    void setPulseEnabled(bool enabled);
    bool isPulseEnabled() const;
    void setPulseVolume(float volume);
    float getPulseVolume() const;
    void setPulseWidth(float width);
    float getPulseWidth() const;

private:
    //==============================================================================
    // Pulse generation variables
    bool pulseEnabled = true;
    float pulseVolume = 0.5f;
    float pulseWidth = 0.1f; // 10% of pulse period

    // Timing variables
    double sampleRate = 44100.0;
    double currentTempo = 120.0;
    double currentPpq = 0.0;
    double lastPpq = 0.0;

    // Pulse generation variables
    double pulsePeriod = 0.0; // in samples
    double pulseCounter = 0.0;
    double pulseDuration = 0.0; // in samples

    // Audio processing variables
    juce::AudioParameterFloat* volumeParameter = nullptr;
    juce::AudioParameterBool* enabledParameter = nullptr;
    juce::AudioParameterFloat* widthParameter = nullptr;

    // Parameter handling
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Make parameters accessible to editor
public:
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }

    // Pulse generation method
    void generatePulse(juce::AudioBuffer<float>& buffer, int startSample, int numSamples);
    void updateTimingInfo();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pulse24SyncAudioProcessor)
};
