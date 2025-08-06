#pragma once

#include <JuceHeader.h>

// Simplified Ableton Link wrapper for Projucer builds
// This provides the interface without requiring external Link dependencies
class AbletonLinkWrapper
{
public:
    AbletonLinkWrapper();
    ~AbletonLinkWrapper() = default;

    void prepare(double sampleRate);
    void reset();

    // Link control
    void setLinkEnabled(bool enabled);
    bool isLinkEnabled() const;
    
    void setTempo(double bpm);
    double getTempo() const;
    
    // Session info
    std::size_t getNumPeers() const;
    bool isPlaying() const;
    void setIsPlaying(bool playing);

    // Audio processing
    void processAudioBuffer(int numSamples);
    
    // Timing calculations for audio callback
    std::chrono::microseconds getOutputTimeAtSample(int sampleIndex, int bufferSize) const;
    double getPhaseAtTime(std::chrono::microseconds hostTime, double quantum = 4.0) const;

private:
    double mSampleRate = 44100.0;
    double mTempo = 120.0;
    bool mLinkEnabled = false;
    bool mIsPlaying = false;
    int64_t mSampleTime = 0;
    
    // Internal timing
    double mPhase = 0.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbletonLinkWrapper)
};