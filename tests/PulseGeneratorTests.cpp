#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "TestUtils.h"
#include "PulseGenerator.h"

class PulseGeneratorTests : public juce::UnitTest
{
public:
    PulseGeneratorTests() : juce::UnitTest("PulseGenerator Tests", "Pulse24Sync") {}

    void runTest() override
    {
        beginTest("Constructor and Default Values");
        testConstructorAndDefaults();
        
        beginTest("Parameter Setting and Getting");
        testParameterSettersAndGetters();
        
        beginTest("Pulse Rate Calculation");
        testPulseRateCalculation();
        
        beginTest("MIDI Pulse Generation");
        testMidiPulseGeneration();
        
        beginTest("Host Synchronization");
        testHostSynchronization();
        
        beginTest("Manual BPM Mode");
        testManualBPMMode();
        
        beginTest("Enable/Disable Functionality");
        testEnableDisable();
        
        beginTest("Sample Rate Changes");
        testSampleRateChanges();
        
        beginTest("Edge Cases and Error Handling");
        testEdgeCases();
        
        beginTest("Performance and Timing Accuracy");
        testTimingAccuracy();
    }

private:
    void testConstructorAndDefaults()
    {
        PulseGenerator generator;
        
        // Test default values
        expect(generator.getEnabled() == true, "Default enabled state should be true");
        expectWithinAbsoluteError(generator.getPulseVelocity(), 100.0f, 0.1f, "Default velocity should be 100");
        expectEquals(generator.getPulseChannel(), 1, "Default channel should be 1");
        expect(generator.getSyncToHost() == true, "Default sync to host should be true");
        expectWithinAbsoluteError(generator.getManualBPM(), 120.0f, 0.1f, "Default manual BPM should be 120");
        expectWithinAbsoluteError(generator.getCurrentBPM(), 120.0, 0.1, "Default current BPM should be 120");
    }
    
    void testParameterSettersAndGetters()
    {
        PulseGenerator generator;
        
        // Test enabled setting
        generator.setEnabled(false);
        expect(generator.getEnabled() == false, "Enabled state should be false after setting");
        generator.setEnabled(true);
        expect(generator.getEnabled() == true, "Enabled state should be true after setting");
        
        // Test velocity setting
        generator.setPulseVelocity(64.0f);
        expectWithinAbsoluteError(generator.getPulseVelocity(), 64.0f, 0.1f, "Velocity should be 64");
        
        // Test channel setting
        generator.setPulseChannel(8);
        expectEquals(generator.getPulseChannel(), 8, "Channel should be 8");
        
        // Test sync to host setting
        generator.setSyncToHost(false);
        expect(generator.getSyncToHost() == false, "Sync to host should be false");
        
        // Test manual BPM setting
        generator.setManualBPM(140.0f);
        expectWithinAbsoluteError(generator.getManualBPM(), 140.0f, 0.1f, "Manual BPM should be 140");
        expectWithinAbsoluteError(generator.getCurrentBPM(), 140.0, 0.1, "Current BPM should match manual BPM when not synced to host");
    }
    
    void testPulseRateCalculation()
    {
        PulseGenerator generator;
        generator.prepare(44100.0);
        
        // Test various BPM values
        auto testBPMs = TestUtils::TestDataGenerator::generateBPMTestValues();
        
        for (auto bpm : testBPMs)
        {
            generator.setSyncToHost(false);
            generator.setManualBPM(static_cast<float>(bpm));
            
            // Expected pulse rate: (BPM / 60) * 4 * 24
            double expectedPulseRate = TestUtils::TimingUtils::bpmToPulseRate(bpm);
            double actualPulseRate = generator.getPulseRate();
            
            expectWithinAbsoluteError(actualPulseRate, expectedPulseRate, 0.001, 
                                    "Pulse rate should be calculated correctly for BPM " + juce::String(bpm));
        }
    }
    
    void testMidiPulseGeneration()
    {
        PulseGenerator generator;
        generator.prepare(44100.0);
        generator.setEnabled(true);
        generator.setSyncToHost(false);
        generator.setManualBPM(120.0f);
        generator.setPulseChannel(5);
        generator.setPulseVelocity(100.0f);
        generator.setHostIsPlaying(true);
        
        juce::MidiBuffer midiBuffer;
        
        // Process enough samples to generate at least one pulse
        int numSamples = 2000; // About 45ms at 44.1kHz
        generator.process(numSamples, 44100.0, midiBuffer);
        
        // Check if MIDI events were generated
        bool foundNoteOn = false;
        bool foundNoteOff = false;
        
        for (const auto& event : midiBuffer)
        {
            auto message = event.getMessage();
            
            if (message.isNoteOn() && message.getChannel() == 5 && message.getNoteNumber() == 60)
            {
                foundNoteOn = true;
                expectWithinAbsoluteError(static_cast<float>(message.getVelocity()), 100.0f, 0.1f, 
                                        "Note on velocity should match set velocity");
            }
            else if (message.isNoteOff() && message.getChannel() == 5 && message.getNoteNumber() == 60)
            {
                foundNoteOff = true;
            }
        }
        
        expect(foundNoteOn, "Should generate note on events");
        expect(foundNoteOff, "Should generate note off events");
    }
    
    void testHostSynchronization()
    {
        PulseGenerator generator;
        generator.prepare(44100.0);
        generator.setSyncToHost(true);
        generator.setEnabled(true);
        generator.setHostIsPlaying(true);
        
        // Test host tempo setting
        generator.setHostTempo(140.0);
        expectWithinAbsoluteError(generator.getCurrentBPM(), 140.0, 0.1, 
                                "Current BPM should match host tempo when synced");
        
        // Test host position setting
        generator.setHostPosition(10.5);
        // Position setting doesn't have a direct getter, but we can verify it doesn't crash
        
        // Test host playing state
        generator.setHostIsPlaying(false);
        juce::MidiBuffer midiBuffer;
        generator.process(1000, 44100.0, midiBuffer);
        
        expect(midiBuffer.isEmpty(), "Should not generate pulses when host is not playing");
    }
    
    void testManualBPMMode()
    {
        PulseGenerator generator;
        generator.prepare(44100.0);
        generator.setSyncToHost(false);
        generator.setEnabled(true);
        generator.setHostIsPlaying(true);
        
        auto testBPMs = TestUtils::TestDataGenerator::generateBPMTestValues();
        
        for (auto bpm : testBPMs)
        {
            generator.setManualBPM(static_cast<float>(bpm));
            
            expectWithinAbsoluteError(generator.getCurrentBPM(), bpm, 0.1, 
                                    "Current BPM should match manual BPM when not synced to host");
            
            // Verify pulse rate is calculated correctly
            double expectedPulseRate = TestUtils::TimingUtils::bpmToPulseRate(bpm);
            expectWithinAbsoluteError(generator.getPulseRate(), expectedPulseRate, 0.001, 
                                    "Pulse rate should match calculated rate for manual BPM " + juce::String(bpm));
        }
    }
    
    void testEnableDisable()
    {
        PulseGenerator generator;
        generator.prepare(44100.0);
        generator.setSyncToHost(false);
        generator.setManualBPM(120.0f);
        generator.setHostIsPlaying(true);
        
        // Test disabled state
        generator.setEnabled(false);
        juce::MidiBuffer midiBuffer;
        generator.process(2000, 44100.0, midiBuffer);
        
        expect(midiBuffer.isEmpty(), "Should not generate pulses when disabled");
        
        // Test enabled state
        generator.setEnabled(true);
        midiBuffer.clear();
        generator.process(2000, 44100.0, midiBuffer);
        
        expect(!midiBuffer.isEmpty(), "Should generate pulses when enabled");
    }
    
    void testSampleRateChanges()
    {
        PulseGenerator generator;
        
        // Test different sample rates
        std::vector<double> sampleRates = {22050.0, 44100.0, 48000.0, 96000.0};
        
        for (auto sampleRate : sampleRates)
        {
            generator.prepare(sampleRate);
            generator.setSyncToHost(false);
            generator.setManualBPM(120.0f);
            
            double expectedPulseRate = TestUtils::TimingUtils::bpmToPulseRate(120.0);
            expectWithinAbsoluteError(generator.getPulseRate(), expectedPulseRate, 0.001, 
                                    "Pulse rate should be consistent across sample rates");
            
            // Test runtime sample rate change
            juce::MidiBuffer midiBuffer;
            generator.setEnabled(true);
            generator.setHostIsPlaying(true);
            generator.process(1000, sampleRate, midiBuffer);
            
            // Should handle sample rate changes gracefully
        }
    }
    
    void testEdgeCases()
    {
        PulseGenerator generator;
        generator.prepare(44100.0);
        
        // Test extreme BPM values
        generator.setSyncToHost(false);
        generator.setManualBPM(1.0f); // Very slow
        expect(generator.getPulseRate() > 0.0, "Should handle very slow BPM");
        
        generator.setManualBPM(300.0f); // Very fast
        expect(generator.getPulseRate() > 0.0, "Should handle very fast BPM");
        
        // Test extreme channel values
        generator.setPulseChannel(1);
        expectEquals(generator.getPulseChannel(), 1, "Should handle channel 1");
        
        generator.setPulseChannel(16);
        expectEquals(generator.getPulseChannel(), 16, "Should handle channel 16");
        
        // Test extreme velocity values
        generator.setPulseVelocity(0.0f);
        expectWithinAbsoluteError(generator.getPulseVelocity(), 0.0f, 0.1f, "Should handle velocity 0");
        
        generator.setPulseVelocity(127.0f);
        expectWithinAbsoluteError(generator.getPulseVelocity(), 127.0f, 0.1f, "Should handle velocity 127");
        
        // Test zero samples processing
        juce::MidiBuffer midiBuffer;
        generator.process(0, 44100.0, midiBuffer);
        expect(midiBuffer.isEmpty(), "Should handle zero samples gracefully");
    }
    
    void testTimingAccuracy()
    {
        PulseGenerator generator;
        generator.prepare(44100.0);
        generator.setSyncToHost(false);
        generator.setManualBPM(120.0f);
        generator.setEnabled(true);
        generator.setHostIsPlaying(true);
        generator.setPulseChannel(1);
        
        juce::MidiBuffer midiBuffer;
        
        // Process several blocks and verify timing consistency
        std::vector<int> pulseTimes;
        int totalSamples = 0;
        const int blockSize = 512;
        const int numBlocks = 20;
        
        for (int block = 0; block < numBlocks; ++block)
        {
            midiBuffer.clear();
            generator.process(blockSize, 44100.0, midiBuffer);
            
            for (const auto& event : midiBuffer)
            {
                if (event.getMessage().isNoteOn())
                {
                    pulseTimes.push_back(totalSamples + event.samplePosition);
                }
            }
            
            totalSamples += blockSize;
        }
        
        // Verify we got some pulses
        expect(pulseTimes.size() > 0, "Should generate pulses over multiple blocks");
        
        if (pulseTimes.size() > 1)
        {
            // Calculate intervals between pulses
            std::vector<int> intervals;
            for (size_t i = 1; i < pulseTimes.size(); ++i)
            {
                intervals.push_back(pulseTimes[i] - pulseTimes[i-1]);
            }
            
            // Check interval consistency (should be relatively stable)
            if (!intervals.empty())
            {
                double expectedInterval = TestUtils::TimingUtils::pulseRateToInterval(
                    TestUtils::TimingUtils::bpmToPulseRate(120.0), 44100.0);
                
                for (auto interval : intervals)
                {
                    expectWithinAbsoluteError(static_cast<double>(interval), expectedInterval, 
                                            expectedInterval * 0.1, // 10% tolerance
                                            "Pulse intervals should be consistent");
                }
            }
        }
    }
};

// Register the test
static PulseGeneratorTests pulseGeneratorTests;