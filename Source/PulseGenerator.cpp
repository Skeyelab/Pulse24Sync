#include "PulseGenerator.h"

PulseGenerator::PulseGenerator()
{
}

void PulseGenerator::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    // Update pulse duration based on sample rate (keep it around 22ms)
    pulseDurationSamples = static_cast<int>(sampleRate * 0.022); // 22ms pulse duration
    reset();
    updatePulseRate();
}

void PulseGenerator::reset()
{
    currentPosition = 0.0;
    nextPulseTime = 0.0;
    currentPulsePosition = 0;
    pulseActive = false;
    updatePulseRate();
}

void PulseGenerator::process(int numSamples, double currentSampleRate, juce::AudioBuffer<float>& audioBuffer)
{
    if (!isEnabled || !hostIsPlaying)
        return;

    // Update sample rate if it changed
    if (currentSampleRate != sampleRate)
    {
        sampleRate = currentSampleRate;
        pulseDurationSamples = static_cast<int>(sampleRate * 0.022); // Update pulse duration
        updatePulseRate();
    }

    // Update pulse rate if tempo changed
    updatePulseRate();

    // Process each sample
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Check if it's time for a new pulse
        if (currentPosition >= nextPulseTime && !pulseActive)
        {
            pulseActive = true;
            currentPulsePosition = 0;
            nextPulseTime += pulseInterval;
        }

        // Generate audio for active pulse
        if (pulseActive)
        {
            float pulseSample = generatePulseSample(currentPulsePosition);
            
            // Add to all output channels
            for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel)
            {
                audioBuffer.addSample(channel, sample, pulseSample);
            }

            currentPulsePosition++;
            
            // Check if pulse is finished
            if (currentPulsePosition >= pulseDurationSamples)
            {
                pulseActive = false;
                currentPulsePosition = 0;
            }
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

void PulseGenerator::generateAudioPulse(juce::AudioBuffer<float>& audioBuffer, int startSample, int numSamples)
{
    // This method is kept for potential future use but not currently called
    juce::ignoreUnused(audioBuffer, startSample, numSamples);
}

float PulseGenerator::generatePulseSample(int sampleIndex)
{
    if (sampleIndex >= pulseDurationSamples)
        return 0.0f;

    // Generate sine wave
    float phase = (2.0f * juce::MathConstants<float>::pi * PULSE_FREQUENCY * sampleIndex) / static_cast<float>(sampleRate);
    float sineWave = std::sin(phase);

    // Apply envelope (quick attack, exponential decay)
    float envelope = 1.0f;
    if (sampleIndex < pulseDurationSamples * 0.1f) // 10% attack
    {
        envelope = static_cast<float>(sampleIndex) / (pulseDurationSamples * 0.1f);
    }
    else // 90% decay
    {
        float decayPosition = (sampleIndex - pulseDurationSamples * 0.1f) / (pulseDurationSamples * 0.9f);
        envelope = std::exp(-5.0f * decayPosition); // Exponential decay
    }

    return sineWave * envelope * pulseVelocity * 0.1f; // Scale down to prevent clipping
}
