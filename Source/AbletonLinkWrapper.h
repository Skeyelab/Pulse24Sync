#pragma once

#include <JuceHeader.h>

#ifdef ABLETON_LINK_ENABLED
#include <ableton/Link.hpp>
#include <ableton/link/HostTimeFilter.hpp>
#endif

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

    // Timing calculations for audio callback
    double getBeatAtTime(std::chrono::microseconds hostTime, double quantum = 4.0) const;
    double getPhaseAtTime(std::chrono::microseconds hostTime, double quantum = 4.0) const;
    
    // Audio callback helpers
    std::chrono::microseconds getOutputTimeAtSample(int samplePosition, int bufferSize) const;
    void processAudioBuffer(int bufferSize);

private:
#ifdef ABLETON_LINK_ENABLED
    ableton::Link mLink;
    ableton::link::HostTimeFilter<ableton::link::platform::Clock> mHostTimeFilter;
    double mSampleRate = 44100.0;
    std::uint64_t mSampleTime = 0;
    bool mLinkEnabled = false;
#endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AbletonLinkWrapper)
};