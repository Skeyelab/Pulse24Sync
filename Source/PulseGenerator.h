#pragma once

#include <JuceHeader.h>

class PulseGenerator
{
public:
    PulseGenerator();
    ~PulseGenerator() = default;

    void prepare(double sampleRate);
    void reset();

    void process(int numSamples, double sampleRate, juce::MidiBuffer& midiMessages);

    // Parameter setters
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setPulseVelocity(float velocity) { pulseVelocity = static_cast<uint8>(velocity); }
    void setPulseChannel(int channel) { pulseChannel = channel; }
    void setSyncToHost(bool sync) { syncToHost = sync; }
    void setManualBPM(float bpm) { manualBPM = bpm; }

    // Host tempo synchronization
    void setHostTempo(double bpm) { hostBPM = bpm; }
    void setHostIsPlaying(bool playing) { hostIsPlaying = playing; }
    void setHostPosition(double timeInSeconds) { hostPosition = timeInSeconds; }

    // Getters for UI
    bool getEnabled() const { return isEnabled; }
    float getPulseVelocity() const { return static_cast<float>(pulseVelocity); }
    int getPulseChannel() const { return pulseChannel; }
    bool getSyncToHost() const { return syncToHost; }
    float getManualBPM() const { return manualBPM; }
    double getCurrentBPM() const { return syncToHost ? hostBPM : manualBPM; }
    double getPulseRate() const { return pulseRate; }

private:
    // Parameters
    bool isEnabled = true;
    uint8 pulseVelocity = 100;
    int pulseChannel = 1;
    bool syncToHost = true;
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

    // Constants
    static constexpr int PULSES_PER_QUARTER_NOTE = 24;
    static constexpr double SECONDS_PER_MINUTE = 60.0;

    // Helper methods
    void updatePulseRate();
    void generatePulse(juce::MidiBuffer& midiMessages, int samplePosition);
};
