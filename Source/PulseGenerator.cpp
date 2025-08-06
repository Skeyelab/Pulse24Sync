#include "PulseGenerator.h"

PulseGenerator::PulseGenerator()
{
}

void PulseGenerator::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    linkWrapper.prepare(newSampleRate);
    reset();
    updatePulseRate();
}

void PulseGenerator::reset()
{
    currentPosition = 0.0;
    nextPulseTime = 0.0;
    linkWrapper.reset();
    updatePulseRate();
}

void PulseGenerator::process(int numSamples, double currentSampleRate, juce::MidiBuffer& midiMessages)
{
    if (!isEnabled || !shouldGeneratePulse())
        return;

    // Update sample rate if it changed
    if (std::abs(currentSampleRate - sampleRate) > 1e-6)
    {
        sampleRate = currentSampleRate;
        linkWrapper.prepare(currentSampleRate);
        updatePulseRate();
    }

    // Update pulse rate if tempo changed
    updatePulseRate();

    // Process Link timing
    linkWrapper.processAudioBuffer(numSamples);

    // Handle Link synchronization for timing
    if (syncMode == SyncToLink && linkWrapper.isLinkEnabled())
    {
        // Use Link timing for precise synchronization
        for (int sample = 0; sample < numSamples; ++sample)
        {
            auto hostTime = linkWrapper.getOutputTimeAtSample(sample, numSamples);
            auto phase = linkWrapper.getPhaseAtTime(hostTime, 4.0); // 4 beats per bar
            
            // Check if we're at the start of a quarter note (phase = 0, 1, 2, or 3)
            auto pulsesPerBeat = PULSES_PER_QUARTER_NOTE;
            auto pulsePhase = std::fmod(phase * pulsesPerBeat, 1.0);
            
            // Generate pulse at the start of each pulse subdivision
            if (pulsePhase < 0.01) // Small threshold for pulse detection
            {
                generatePulse(midiMessages, sample);
            }
        }
    }
    else
    {
        // Original timing logic for host sync and manual mode
        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Check if it's time for a pulse
            if (currentPosition >= nextPulseTime)
            {
                generatePulse(midiMessages, sample);
                nextPulseTime += pulseInterval;
            }

            currentPosition += 1.0;
        }
    }
}

void PulseGenerator::setLinkEnabled(bool enabled)
{
    linkWrapper.setLinkEnabled(enabled);
}

bool PulseGenerator::isLinkEnabled() const
{
    return linkWrapper.isLinkEnabled();
}

std::size_t PulseGenerator::getLinkPeerCount() const
{
    return linkWrapper.getNumPeers();
}

double PulseGenerator::getCurrentBPM() const
{
    switch (syncMode)
    {
        case SyncToHost:
            return hostBPM;
        case SyncToManual:
            return manualBPM;
        case SyncToLink:
            return linkWrapper.getTempo();
        default:
            return hostBPM;
    }
}

bool PulseGenerator::shouldGeneratePulse() const
{
    switch (syncMode)
    {
        case SyncToHost:
            return hostIsPlaying;
        case SyncToManual:
            return true; // Always play in manual mode
        case SyncToLink:
            return linkWrapper.isLinkEnabled() && linkWrapper.isPlaying();
        default:
            return hostIsPlaying;
    }
}

void PulseGenerator::updatePulseRate()
{
    double currentBPM = getCurrentBPM();

    // Calculate pulses per second: (BPM / 60) * 4 * 24
    // BPM/60 = beats per second
    // * 4 = quarter notes per second (assuming 4/4 time)
    // * 24 = pulses per quarter note
    pulseRate = (currentBPM / SECONDS_PER_MINUTE) * 4.0 * PULSES_PER_QUARTER_NOTE;

    // Calculate samples between pulses
    pulseInterval = sampleRate / pulseRate;

    // Ensure we don't have negative or zero intervals
    if (pulseInterval <= 0.0)
        pulseInterval = sampleRate; // Fallback to 1 pulse per second
        
    // Update Link tempo if we're in Link mode and Link is enabled
    if (syncMode == SyncToLink && linkWrapper.isLinkEnabled())
    {
        linkWrapper.setTempo(currentBPM);
    }
}

void PulseGenerator::generatePulse(juce::MidiBuffer& midiMessages, int samplePosition)
{
    // Create MIDI note on message
    juce::MidiMessage noteOn = juce::MidiMessage::noteOn(pulseChannel, 60, pulseVelocity);
    noteOn.setTimeStamp(samplePosition);
    midiMessages.addEvent(noteOn, samplePosition);

    // Create MIDI note off message (very short note)
    juce::MidiMessage noteOff = juce::MidiMessage::noteOff(pulseChannel, 60);
    noteOff.setTimeStamp(samplePosition + 1);
    midiMessages.addEvent(noteOff, samplePosition + 1);
}
