#pragma once

#include <JuceHeader.h>

class PulseGenerator
{
public:
    PulseGenerator();
    ~PulseGenerator() = default;

    void prepare(double sampleRate);
    void reset();

    void process(int numSamples, double sampleRate, juce::AudioBuffer<float>& audioBuffer);

    // Parameter setters
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setPulseVelocity(float velocity) { pulseVelocity = juce::jlimit(0.0f, 1.0f, velocity / 127.0f); } // Convert MIDI velocity to gain
    void setPulseChannel(int channel) { pulseChannel = channel; } // Keep for compatibility but not used for audio
    void setSyncToHost(bool sync) { syncToHost = sync; }
    void setManualBPM(float bpm) { manualBPM = bpm; }

    // Host tempo synchronization
    void setHostTempo(double bpm) { hostBPM = bpm; }
    void setHostIsPlaying(bool playing) { hostIsPlaying = playing; }
    void setHostPosition(double timeInSeconds) { hostPosition = timeInSeconds; }
    void setHostPPQPosition(double ppq) { hostPPQPosition = ppq; }  // Add PPQ position setter

    // Getters for UI
    bool getEnabled() const { return isEnabled; }
    float getPulseVelocity() const { return pulseVelocity * 127.0f; } // Convert back to MIDI scale for UI
    int getPulseChannel() const { return pulseChannel; }
    bool getSyncToHost() const { return syncToHost; }
    float getManualBPM() const { return manualBPM; }
    double getCurrentBPM() const { return syncToHost ? hostBPM : manualBPM; }
    double getPulseRate() const { return pulseRate; }

private:
    // Parameters
    bool isEnabled = true;
    float pulseVelocity = 100.0f / 127.0f; // Store as gain (0.0 to 1.0)
    int pulseChannel = 1; // Keep for compatibility
    bool syncToHost = true;
    float manualBPM = 120.0f;

    // Host tempo info
    double hostBPM = 120.0;
    bool hostIsPlaying = false;
    double hostPosition = 0.0;
    double hostPPQPosition = 0.0;  // PPQ position from DAW
    double lastHostBPM = 120.0;    // Track tempo changes
    double lastPPQPosition = 0.0;  // Track PPQ position for sync

    // Timing
    double sampleRate = 44100.0;
    double pulseRate = 0.0;  // pulses per second
    double pulseInterval = 0.0;  // samples between pulses
    double currentPosition = 0.0;  // current position in samples
    double nextPulseTime = 0.0;  // time of next pulse in samples
    double pulsesPerQuarterNote = 24.0;  // Standard MIDI clock rate
    double samplesPerPulse = 0.0;  // Samples per pulse at current tempo

    // Audio generation
    int pulseDurationSamples = 1000; // Duration of each pulse in samples (about 22ms at 44.1kHz)
    int currentPulsePosition = 0; // Current position within a pulse
    bool pulseActive = false; // Whether we're currently generating a pulse

    // Constants
    static constexpr int PULSES_PER_QUARTER_NOTE = 24;
    static constexpr double SECONDS_PER_MINUTE = 60.0;
    static constexpr float PULSE_FREQUENCY = 1000.0f; // 1kHz sine wave for pulses
    static constexpr double TEMPO_CHANGE_THRESHOLD = 0.1; // Detect tempo changes > 0.1 BPM

    // Helper methods
    void updatePulseRate();
    void generateAudioPulse(juce::AudioBuffer<float>& audioBuffer, int startSample, int numSamples);
    float generatePulseSample(int sampleIndex);
    bool detectTempoChange();  // Detect if tempo has changed
    void resyncTiming();       // Resynchronize timing when tempo changes
};
