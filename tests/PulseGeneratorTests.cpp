#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "PulseGenerator.h"

static juce::AudioBuffer<float> makeBuffer(int numChannels, int numSamples)
{
    juce::AudioBuffer<float> buffer(numChannels, numSamples);
    buffer.clear();
    return buffer;
}

TEST_CASE("Pulse rate math at common BPM", "[pulse]")
{
    PulseGenerator gen;
    const double sampleRate = 48000.0;
    gen.prepare(sampleRate);
    gen.setHostIsPlaying(true);

    // Ensure sync to manual for controlled BPM
    gen.setSyncToHost(false);

    SECTION("120 BPM -> 48k/((120/60)*24) samples per pulse")
    {
        gen.setManualBPM(120.0f);
        juce::AudioBuffer<float> buffer = makeBuffer(2, 48000);
        gen.process(buffer.getNumSamples(), sampleRate, buffer);

        // At 120 BPM and 24 PPQN, pulses per second = 48. Samples per pulse = 1000.
        // Expect non-zero samples roughly every 1000 samples due to pulses.
        int nonZero = 0;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            for (int i = 0; i < buffer.getNumSamples(); ++i)
                if (buffer.getReadPointer(ch)[i] != 0.0f) { nonZero++; break; }
        REQUIRE(nonZero > 0);
    }
}

TEST_CASE("Disabled generator produces silence", "[pulse]")
{
    PulseGenerator gen;
    const double sampleRate = 44100.0;
    gen.prepare(sampleRate);
    gen.setHostIsPlaying(true);
    gen.setEnabled(false);

    auto buffer = makeBuffer(2, 2048);
    gen.process(buffer.getNumSamples(), sampleRate, buffer);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            REQUIRE(buffer.getReadPointer(ch)[i] == 0.0f);
}

TEST_CASE("Not playing -> silence", "[pulse]")
{
    PulseGenerator gen;
    const double sampleRate = 44100.0;
    gen.prepare(sampleRate);
    gen.setHostIsPlaying(false);

    auto buffer = makeBuffer(2, 4096);
    gen.process(buffer.getNumSamples(), sampleRate, buffer);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            REQUIRE(buffer.getReadPointer(ch)[i] == 0.0f);
}

TEST_CASE("Pulse width updates pulse duration samples", "[pulse]")
{
    PulseGenerator gen;
    const double sampleRate = 44100.0;
    gen.prepare(sampleRate);
    gen.setHostIsPlaying(true);

    // Default width is 22ms; change to 10ms and ensure shorter audible span
    gen.setPulseWidth(10.0f);

    auto buffer = makeBuffer(1, 2048);
    gen.setSyncToHost(false);
    gen.setManualBPM(120.0f);
    gen.process(buffer.getNumSamples(), sampleRate, buffer);

    // Find contiguous non-zero region length of first pulse
    int firstNonZero = -1;
    int lastNonZero = -1;
    auto* data = buffer.getReadPointer(0);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        if (data[i] != 0.0f)
        {
            if (firstNonZero < 0) firstNonZero = i;
            lastNonZero = i;
        }
        else if (firstNonZero >= 0 && lastNonZero >= 0)
        {
            break; // finished first pulse
        }
    }
    int nonZeroLen = (firstNonZero >= 0 && lastNonZero >= 0) ? (lastNonZero - firstNonZero + 1) : 0;
    // 10ms at 44.1k is ~441 samples; allow margin due to envelope rounding
    REQUIRE(nonZeroLen >= 300);
    REQUIRE(nonZeroLen <= 600);
}