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
    updatePulseRate();
}

void PulseGenerator::process(int numSamples, double currentSampleRate, juce::MidiBuffer& midiMessages)
{
    if (!isEnabled || !hostIsPlaying)
        return;

    // Update sample rate if it changed
    if (currentSampleRate != sampleRate)
    {
        sampleRate = currentSampleRate;
        updatePulseRate();
    }

    // Update pulse rate if tempo changed
    updatePulseRate();

    // Process each sample
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

void PulseGenerator::updatePulseRate()
{
    double currentBPM = syncToHost ? hostBPM : manualBPM;

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
