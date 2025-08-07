#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Test utilities and helper classes for Pulse24Sync testing
 */
namespace TestUtils
{
    /**
     * Mock MIDI buffer for testing MIDI generation
     */
    class MockMidiBuffer
    {
    public:
        MockMidiBuffer() = default;
        
        void clear() { messages.clear(); }
        
        void addEvent(const juce::MidiMessage& message, int samplePosition)
        {
            messages.push_back({message, samplePosition});
        }
        
        size_t getNumEvents() const { return messages.size(); }
        
        juce::MidiMessage getMessage(size_t index) const 
        { 
            return index < messages.size() ? messages[index].message : juce::MidiMessage();
        }
        
        int getSamplePosition(size_t index) const
        {
            return index < messages.size() ? messages[index].samplePosition : -1;
        }
        
        bool hasNoteOn(int channel, int noteNumber) const
        {
            for (const auto& event : messages)
            {
                if (event.message.isNoteOn() && 
                    event.message.getChannel() == channel &&
                    event.message.getNoteNumber() == noteNumber)
                {
                    return true;
                }
            }
            return false;
        }
        
        bool hasNoteOff(int channel, int noteNumber) const
        {
            for (const auto& event : messages)
            {
                if (event.message.isNoteOff() && 
                    event.message.getChannel() == channel &&
                    event.message.getNoteNumber() == noteNumber)
                {
                    return true;
                }
            }
            return false;
        }
        
    private:
        struct MidiEvent
        {
            juce::MidiMessage message;
            int samplePosition;
        };
        
        std::vector<MidiEvent> messages;
    };
    
    /**
     * Test fixture for consistent test setup
     */
    class TestFixture
    {
    public:
        TestFixture(double sampleRate = 44100.0) 
            : testSampleRate(sampleRate)
        {
            setup();
        }
        
        virtual ~TestFixture() = default;
        
        double getSampleRate() const { return testSampleRate; }
        
    protected:
        virtual void setup() {}
        virtual void teardown() {}
        
    private:
        double testSampleRate;
    };
    
    /**
     * Precision comparison helpers
     */
    inline bool isApproximatelyEqual(double a, double b, double tolerance = 1e-6)
    {
        return std::abs(a - b) < tolerance;
    }
    
    inline bool isApproximatelyEqual(float a, float b, float tolerance = 1e-5f)
    {
        return std::abs(a - b) < tolerance;
    }
    
    /**
     * Timing utilities for audio tests
     */
    class TimingUtils
    {
    public:
        static double bpmToPulseRate(double bpm, int pulsesPerQuarterNote = 24)
        {
            // BPM/60 = beats per second
            // * 4 = quarter notes per second (assuming 4/4 time)
            // * pulsesPerQuarterNote = pulses per second
            return (bpm / 60.0) * 4.0 * pulsesPerQuarterNote;
        }
        
        static double pulseRateToInterval(double pulseRate, double sampleRate)
        {
            return sampleRate / pulseRate;
        }
        
        static int samplesToMs(int samples, double sampleRate)
        {
            return static_cast<int>((samples * 1000.0) / sampleRate);
        }
        
        static int msToSamples(int milliseconds, double sampleRate)
        {
            return static_cast<int>((milliseconds * sampleRate) / 1000.0);
        }
    };
    
    /**
     * Test data generators
     */
    class TestDataGenerator
    {
    public:
        static std::vector<double> generateBPMTestValues()
        {
            return {60.0, 120.0, 140.0, 180.0, 200.0};
        }
        
        static std::vector<int> generateChannelTestValues()
        {
            return {1, 8, 16};
        }
        
        static std::vector<float> generateVelocityTestValues()
        {
            return {0.0f, 64.0f, 100.0f, 127.0f};
        }
    };
}