#pragma once

// Pulse24SyncAudioProcessor
// - Owns parameters via AudioProcessorValueTreeState (see Parameters.h for IDs)
// - Bridges host state (tempo/transport) to the PulseGenerator engine
// - Generates an audible 1kHz pulse train at 24 PPQN for sync testing
// - UI binds directly to parameters; processBlock reads and applies every block

#include <JuceHeader.h>
#include "PulseGenerator.h"
#include "Parameters.h"

class Pulse24SyncAudioProcessor : public juce::AudioProcessor
{
public:
    Pulse24SyncAudioProcessor();
    ~Pulse24SyncAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& busesLayout) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Plugin parameters
    juce::AudioProcessorValueTreeState parameters;

    // Pulse generator
    PulseGenerator pulseGenerator;

private:
    void syncParametersToEngine();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pulse24SyncAudioProcessor)
};
