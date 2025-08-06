#pragma once

#include <JuceHeader.h>

// MIDI Machine Control Commands
enum class MMCCommand : juce::uint8
{
    Stop = 0x01,
    Play = 0x02,
    DeferredPlay = 0x03,
    FastForward = 0x04,
    Rewind = 0x05,
    RecordStrobe = 0x06,    // Punch In
    RecordExit = 0x07,      // Punch Out
    RecordPause = 0x08,
    Pause = 0x09,
    Eject = 0x0A,
    Chase = 0x0B,
    Reset = 0x0D,
    Locate = 0x44,          // Goto/Song Position
    Shuttle = 0x47
};

// PPQN Resolution Options
enum class PPQNResolution : int
{
    PPQN_24 = 24,   // Standard MIDI clock
    PPQN_48 = 48,   // Korg standard
    PPQN_96 = 96,   // Oberheim/Linn standard
    PPQN_192 = 192, // Modern sequencers
    PPQN_480 = 480  // High-resolution DAW timing
};

// Transport States
enum class TransportState
{
    Stopped,
    Playing,
    Paused,
    Recording
};

class PulseGenerator
{
public:
    PulseGenerator();
    ~PulseGenerator() = default;

    void prepare(double sampleRate);
    void reset();

    void process(int numSamples, double sampleRate, juce::MidiBuffer& midiMessages);

    // Basic parameter setters
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setPulseVelocity(float velocity) { pulseVelocity = static_cast<juce::uint8>(velocity); }
    void setPulseChannel(int channel) { pulseChannel = channel; }
    void setSyncToHost(bool sync) { syncToHost = sync; }
    void setManualBPM(float bpm) { manualBPM = bpm; }

    // Enhanced sync parameters
    void setPPQNResolution(PPQNResolution resolution) { ppqnResolution = resolution; }
    void setMMCDeviceID(int deviceId) { mmcDeviceID = deviceId; }
    void setMMCEnabled(bool enabled) { mmcEnabled = enabled; }
    void setSendClockWhenStopped(bool send) { sendClockWhenStopped = send; }
    void setSendMMCCommands(bool send) { sendMMCCommands = send; }
    void setSendSongPositionPointer(bool send) { sendSPP = send; }

    // Host tempo synchronization
    void setHostTempo(double bpm) { hostBPM = bpm; }
    void setHostIsPlaying(bool playing);
    void setHostPosition(double timeInSeconds) { hostPosition = timeInSeconds; }
    void setHostPPQPosition(double ppqPos) { hostPPQPosition = ppqPos; }

    // Transport control
    void sendMMCCommand(MMCCommand command);
    void sendMMCLocate(int hours, int minutes, int seconds, int frames, int subFrames);
    void sendSongPositionPointer(int midiBeats);
    void requestResync() { resyncRequested = true; }
    void forceReset();

    // Getters for UI
    bool getEnabled() const { return isEnabled; }
    float getPulseVelocity() const { return static_cast<float>(pulseVelocity); }
    int getPulseChannel() const { return pulseChannel; }
    bool getSyncToHost() const { return syncToHost; }
    float getManualBPM() const { return manualBPM; }
    PPQNResolution getPPQNResolution() const { return ppqnResolution; }
    int getMMCDeviceID() const { return mmcDeviceID; }
    bool getMMCEnabled() const { return mmcEnabled; }
    bool getSendClockWhenStopped() const { return sendClockWhenStopped; }
    bool getSendMMCCommands() const { return sendMMCCommands; }
    bool getSendSongPositionPointer() const { return sendSPP; }
    double getCurrentBPM() const { return syncToHost ? hostBPM : manualBPM; }
    double getPulseRate() const { return pulseRate; }
    TransportState getTransportState() const { return transportState; }
    int getCurrentSongPosition() const { return currentSongPosition; }

private:
    // Basic parameters
    bool isEnabled = true;
    juce::uint8 pulseVelocity = 100;
    int pulseChannel = 1;
    bool syncToHost = true;
    float manualBPM = 120.0f;

    // Enhanced sync parameters
    PPQNResolution ppqnResolution = PPQNResolution::PPQN_24;
    int mmcDeviceID = 0x7F; // All-call by default
    bool mmcEnabled = false;
    bool sendClockWhenStopped = false;
    bool sendMMCCommands = false;
    bool sendSPP = false;

    // Host tempo info
    double hostBPM = 120.0;
    bool hostIsPlaying = false;
    double hostPosition = 0.0;
    double hostPPQPosition = 0.0;

    // Transport state
    TransportState transportState = TransportState::Stopped;
    TransportState previousTransportState = TransportState::Stopped;
    bool resyncRequested = false;
    int currentSongPosition = 0; // In MIDI beats (16th notes)

    // Timing
    double sampleRate = 44100.0;
    double pulseRate = 0.0;  // pulses per second
    double pulseInterval = 0.0;  // samples between pulses
    double currentPosition = 0.0;  // current position in samples
    double nextPulseTime = 0.0;  // time of next pulse in samples
    double lastPPQPosition = 0.0;

    // Constants
    static constexpr double SECONDS_PER_MINUTE = 60.0;
    static constexpr int MIDI_BEATS_PER_QUARTER_NOTE = 4; // MIDI beat = 16th note

    // Helper methods
    void updatePulseRate();
    void updateTransportState();
    void generatePulse(juce::MidiBuffer& midiMessages, int samplePosition);
    void generateMMCMessage(juce::MidiBuffer& midiMessages, MMCCommand command, int samplePosition);
    void generateSPPMessage(juce::MidiBuffer& midiMessages, int midiBeats, int samplePosition);
    void generateMIDIClockMessage(juce::MidiBuffer& midiMessages, int samplePosition);
    void generateMIDIStartMessage(juce::MidiBuffer& midiMessages, int samplePosition);
    void generateMIDIStopMessage(juce::MidiBuffer& midiMessages, int samplePosition);
    void generateMIDIContinueMessage(juce::MidiBuffer& midiMessages, int samplePosition);
    int calculateCurrentSongPosition();
};
