#pragma once

#include <JuceHeader.h>
#include "AbletonLinkWrapper.h"

class PulseGenerator
{
public:
    enum SyncMode
    {
        SyncToHost = 0,
        SyncToManual,
        SyncToLink
    };

    PulseGenerator();
    ~PulseGenerator() = default;

    void prepare(double sampleRate);
    void reset();

    void process(int numSamples, double sampleRate, juce::MidiBuffer& midiMessages);

    // Parameter setters
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setPulseVelocity(float velocity) { pulseVelocity = static_cast<juce::uint8>(velocity); }
    void setPulseChannel(int channel) { pulseChannel = channel; }
    void setSyncMode(SyncMode mode) { syncMode = mode; }
    void setManualBPM(float bpm) { manualBPM = bpm; }

    // Host tempo synchronization
    void setHostTempo(double bpm) { hostBPM = bpm; }
    void setHostIsPlaying(bool playing) { hostIsPlaying = playing; }
    void setHostPosition(double timeInSeconds) { hostPosition = timeInSeconds; }

    // Link control (only available in standalone builds)
    void setLinkEnabled(bool enabled);
    bool isLinkEnabled() const;
    std::size_t getLinkPeerCount() const;

    // Getters for UI
    bool getEnabled() const { return isEnabled; }
    float getPulseVelocity() const { return static_cast<float>(pulseVelocity); }
    int getPulseChannel() const { return pulseChannel; }
    SyncMode getSyncMode() const { return syncMode; }
    float getManualBPM() const { return manualBPM; }
    double getCurrentBPM() const;
    double getPulseRate() const { return pulseRate; }

private:
    // Parameters
    bool isEnabled = true;
    juce::uint8 pulseVelocity = 100;
    int pulseChannel = 1;
    SyncMode syncMode = SyncToHost;
    float manualBPM = 120.0f;

    // Host tempo info
    double hostBPM = 120.0;
    bool hostIsPlaying = false;
    double hostPosition = 0.0;

    // Timing
    double sampleRate = 44100.0;
    double pulseRate = 0.0;  // pulses per second
    double pulseInterval = 0.0;  // samples between pulses
    double currentPosition = 0.0;  // current position in samples
    double nextPulseTime = 0.0;  // time of next pulse in samples

    // Link integration
    AbletonLinkWrapper linkWrapper;

    // Constants
    static constexpr int PULSES_PER_QUARTER_NOTE = 24;
    static constexpr double SECONDS_PER_MINUTE = 60.0;

    // Helper methods
    void updatePulseRate();
    void generatePulse(juce::MidiBuffer& midiMessages, int samplePosition);
    bool shouldGeneratePulse() const;
};
