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
    lastHostBPM = hostBPM;
    lastPPQPosition = 0.0;
    updatePulseRate();
}

void PulseGenerator::process(int numSamples, double currentSampleRate, juce::AudioBuffer<float>& audioBuffer)
{
    if (!isEnabled)
        return;
    
    // In standalone mode or when not syncing to host, we should always generate audio when enabled
    // Only skip if we're syncing to host AND the host is not playing
    if (syncToHost && !hostIsPlaying)
        return;

    // Update sample rate if it changed
    if (currentSampleRate != sampleRate)
    {
        sampleRate = currentSampleRate;
        pulseDurationSamples = static_cast<int>(sampleRate * 0.022); // Update pulse duration
        updatePulseRate();
    }

    // Detect and handle tempo changes
    if (detectTempoChange())
    {
        resyncTiming();
        updatePulseRate();
    }

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

    // Ensure BPM is valid
    if (currentBPM <= 0.0)
        currentBPM = 120.0;

    // Calculate pulses per second correctly
    // BPM = beats per minute
    // For 24 PPQN (pulses per quarter note):
    // pulses per second = (BPM / 60) * 24
    pulseRate = (currentBPM / SECONDS_PER_MINUTE) * PULSES_PER_QUARTER_NOTE;

    // Calculate samples between pulses
    pulseInterval = sampleRate / pulseRate;
    samplesPerPulse = pulseInterval;

    // Ensure we don't have negative or zero intervals
    if (pulseInterval <= 0.0)
        pulseInterval = sampleRate; // Fallback to 1 pulse per second
}

bool PulseGenerator::detectTempoChange()
{
    // Check if we're in sync mode and tempo has changed
    if (syncToHost)
    {
        // Detect significant tempo change
        if (std::abs(hostBPM - lastHostBPM) > TEMPO_CHANGE_THRESHOLD)
        {
            return true;
        }
        
        // Also check for PPQ position jumps (e.g., when transport is repositioned)
        if (hostPPQPosition > 0.0 && lastPPQPosition > 0.0)
        {
            double expectedPPQAdvance = (hostPPQPosition - lastPPQPosition);
            // If PPQ jumped more than expected (transport repositioned)
            if (std::abs(expectedPPQAdvance) > 1.0)
            {
                return true;
            }
        }
    }
    return false;
}

void PulseGenerator::resyncTiming()
{
    // When tempo changes or transport jumps, resync our timing
    if (syncToHost && hostPPQPosition > 0.0)
    {
        // Calculate which pulse we should be on based on PPQ position
        double pulsesElapsed = hostPPQPosition * PULSES_PER_QUARTER_NOTE;
        double wholePulses = std::floor(pulsesElapsed);
        double fractionalPulse = pulsesElapsed - wholePulses;
        
        // Reset position to align with current pulse
        currentPosition = fractionalPulse * pulseInterval;
        nextPulseTime = pulseInterval;
        
        // If we're in the middle of a pulse, adjust accordingly
        if (fractionalPulse < (pulseDurationSamples / pulseInterval))
        {
            pulseActive = true;
            currentPulsePosition = static_cast<int>(fractionalPulse * pulseInterval);
        }
        else
        {
            pulseActive = false;
            currentPulsePosition = 0;
        }
    }
    else
    {
        // For manual mode or when PPQ not available, just reset timing
        currentPosition = 0.0;
        nextPulseTime = 0.0;
        pulseActive = false;
        currentPulsePosition = 0;
    }
    
    // Update last known values
    lastHostBPM = hostBPM;
    lastPPQPosition = hostPPQPosition;
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
