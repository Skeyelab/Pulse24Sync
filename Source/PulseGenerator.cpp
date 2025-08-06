#include "PulseGenerator.h"

PulseGenerator::PulseGenerator()
{
}

void PulseGenerator::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    reset();
    updatePulseRate();
}

void PulseGenerator::reset()
{
    currentPosition = 0.0;
    nextPulseTime = 0.0;
    lastPPQPosition = 0.0;
    currentSongPosition = 0;
    transportState = TransportState::Stopped;
    previousTransportState = TransportState::Stopped;
    resyncRequested = false;
    updatePulseRate();
}

void PulseGenerator::setHostIsPlaying(bool playing)
{
    bool wasPlaying = hostIsPlaying;
    hostIsPlaying = playing;
    
    // Detect transport state changes for MMC messaging
    if (playing && !wasPlaying)
    {
        transportState = TransportState::Playing;
    }
    else if (!playing && wasPlaying)
    {
        transportState = TransportState::Stopped;
    }
}

void PulseGenerator::process(int numSamples, double currentSampleRate, juce::MidiBuffer& midiMessages)
{
    // Update sample rate if it changed
    if (currentSampleRate != sampleRate)
    {
        sampleRate = currentSampleRate;
        updatePulseRate();
    }

    // Update transport state and send MMC commands if needed
    updateTransportState();

    // Handle resync requests
    if (resyncRequested)
    {
        reset();
        resyncRequested = false;
        if (sendSPP)
        {
            generateSPPMessage(midiMessages, currentSongPosition, 0);
        }
    }

    // Update pulse rate if tempo changed
    updatePulseRate();

    // Calculate current song position
    currentSongPosition = calculateCurrentSongPosition();

    // Send clock pulses based on configuration
    bool shouldSendClock = isEnabled && (hostIsPlaying || sendClockWhenStopped);
    
    if (shouldSendClock)
    {
        // Process each sample
        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Check if it's time for a pulse
            if (currentPosition >= nextPulseTime)
            {
                // Generate clock pulse (note or MIDI clock)
                generatePulse(midiMessages, sample);
                
                // Schedule next pulse
                nextPulseTime += pulseInterval;
            }

            currentPosition += 1.0;
        }
    }
}

void PulseGenerator::updatePulseRate()
{
    double currentBPM = syncToHost ? hostBPM : manualBPM;
    int resolution = static_cast<int>(ppqnResolution);

    // Calculate pulses per second based on PPQN resolution
    // BPM/60 = beats per second (quarter notes per second)
    // * resolution = pulses per quarter note
    pulseRate = (currentBPM / SECONDS_PER_MINUTE) * resolution;

    // Calculate samples between pulses
    pulseInterval = sampleRate / pulseRate;

    // Ensure we don't have negative or zero intervals
    if (pulseInterval <= 0.0)
        pulseInterval = sampleRate; // Fallback to 1 pulse per second
}

void PulseGenerator::updateTransportState()
{
    if (transportState != previousTransportState)
    {
        previousTransportState = transportState;
    }
}

void PulseGenerator::generatePulse(juce::MidiBuffer& midiMessages, int samplePosition)
{
    // Generate MIDI clock pulse
    generateMIDIClockMessage(midiMessages, samplePosition);
}

void PulseGenerator::generateMIDIClockMessage(juce::MidiBuffer& midiMessages, int samplePosition)
{
    // MIDI Clock message (0xF8)
    juce::MidiMessage clockMsg = juce::MidiMessage(0xF8);
    clockMsg.setTimeStamp(samplePosition);
    midiMessages.addEvent(clockMsg, samplePosition);
}

void PulseGenerator::generateMIDIStartMessage(juce::MidiBuffer& midiMessages, int samplePosition)
{
    // MIDI Start message (0xFA)
    juce::MidiMessage startMsg = juce::MidiMessage(0xFA);
    startMsg.setTimeStamp(samplePosition);
    midiMessages.addEvent(startMsg, samplePosition);
}

void PulseGenerator::generateMIDIStopMessage(juce::MidiBuffer& midiMessages, int samplePosition)
{
    // MIDI Stop message (0xFC)
    juce::MidiMessage stopMsg = juce::MidiMessage(0xFC);
    stopMsg.setTimeStamp(samplePosition);
    midiMessages.addEvent(stopMsg, samplePosition);
}

void PulseGenerator::generateMIDIContinueMessage(juce::MidiBuffer& midiMessages, int samplePosition)
{
    // MIDI Continue message (0xFB)
    juce::MidiMessage continueMsg = juce::MidiMessage(0xFB);
    continueMsg.setTimeStamp(samplePosition);
    midiMessages.addEvent(continueMsg, samplePosition);
}

void PulseGenerator::sendMMCCommand(MMCCommand command)
{
    // This would typically queue the command to be sent in the next process block
    // For now, we'll implement a simple approach
}

void PulseGenerator::generateMMCMessage(juce::MidiBuffer& midiMessages, MMCCommand command, int samplePosition)
{
    if (!mmcEnabled) return;

    // MMC message format: F0 7F <DeviceID> 06 <Command> F7
    juce::uint8 mmcData[] = {
        0xF0, // SysEx start
        0x7F, // Universal Real Time SysEx
        static_cast<juce::uint8>(mmcDeviceID), // Device ID
        0x06, // MMC command
        static_cast<juce::uint8>(command), // Specific command
        0xF7  // SysEx end
    };

    juce::MidiMessage mmcMsg = juce::MidiMessage::createSysExMessage(mmcData, sizeof(mmcData));
    mmcMsg.setTimeStamp(samplePosition);
    midiMessages.addEvent(mmcMsg, samplePosition);
}

void PulseGenerator::sendMMCLocate(int hours, int minutes, int seconds, int frames, int subFrames)
{
    // MMC Locate message format: F0 7F <DeviceID> 06 44 06 01 <hr> <mn> <sc> <fr> <ff> F7
    // This would be implemented with proper parameter validation and queuing
}

void PulseGenerator::sendSongPositionPointer(int midiBeats)
{
    currentSongPosition = midiBeats;
}

void PulseGenerator::generateSPPMessage(juce::MidiBuffer& midiMessages, int midiBeats, int samplePosition)
{
    // Song Position Pointer message (0xF2)
    // Format: F2 <LSB> <MSB> where position = LSB + (MSB * 128)
    juce::uint8 lsb = midiBeats & 0x7F;
    juce::uint8 msb = (midiBeats >> 7) & 0x7F;
    
    juce::uint8 sppData[] = { 0xF2, lsb, msb };
    juce::MidiMessage sppMsg = juce::MidiMessage(sppData[0], sppData[1], sppData[2]);
    sppMsg.setTimeStamp(samplePosition);
    midiMessages.addEvent(sppMsg, samplePosition);
}

void PulseGenerator::forceReset()
{
    reset();
}

int PulseGenerator::calculateCurrentSongPosition()
{
    if (!syncToHost) return 0;
    
    // Convert PPQ position to MIDI beats (16th notes)
    // 1 quarter note = 4 MIDI beats (16th notes)
    return static_cast<int>(hostPPQPosition * MIDI_BEATS_PER_QUARTER_NOTE);
}
