#include "AbletonLinkWrapper.h"

AbletonLinkWrapper::AbletonLinkWrapper()
#ifdef ABLETON_LINK_ENABLED
    : mLink(120.0)  // Initialize with 120 BPM
#endif
{
#ifdef ABLETON_LINK_ENABLED
    mLink.setTempoCallback([this](double bpm) {
        // Tempo change callback - could be used for UI updates
        DBG("Link tempo changed to: " << bpm);
    });
    
    mLink.setStartStopCallback([this](bool isPlaying) {
        // Start/stop callback - could be used for UI updates
        DBG("Link start/stop changed to: " << (isPlaying ? "playing" : "stopped"));
    });
#endif
}

void AbletonLinkWrapper::prepare(double sampleRate)
{
#ifdef ABLETON_LINK_ENABLED
    mSampleRate = sampleRate;
    mSampleTime = 0;
    mHostTimeFilter.reset();
#else
    juce::ignoreUnused(sampleRate);
#endif
}

void AbletonLinkWrapper::reset()
{
#ifdef ABLETON_LINK_ENABLED
    mSampleTime = 0;
    mHostTimeFilter.reset();
#endif
}

void AbletonLinkWrapper::setLinkEnabled(bool enabled)
{
#ifdef ABLETON_LINK_ENABLED
    mLinkEnabled = enabled;
    mLink.enable(enabled);
#else
    juce::ignoreUnused(enabled);
#endif
}

bool AbletonLinkWrapper::isLinkEnabled() const
{
#ifdef ABLETON_LINK_ENABLED
    return mLinkEnabled;
#else
    return false;
#endif
}

void AbletonLinkWrapper::setTempo(double bpm)
{
#ifdef ABLETON_LINK_ENABLED
    if (mLinkEnabled && bpm > 0.0)
    {
        auto sessionState = mLink.captureAudioSessionState();
        sessionState.setTempo(bpm, mHostTimeFilter.sampleTimeToHostTime(mSampleTime));
        mLink.commitAudioSessionState(sessionState);
    }
#else
    juce::ignoreUnused(bpm);
#endif
}

double AbletonLinkWrapper::getTempo() const
{
#ifdef ABLETON_LINK_ENABLED
    if (mLinkEnabled)
    {
        auto sessionState = mLink.captureAudioSessionState();
        return sessionState.tempo();
    }
    return 120.0;
#else
    return 120.0;
#endif
}

std::size_t AbletonLinkWrapper::getNumPeers() const
{
#ifdef ABLETON_LINK_ENABLED
    return mLink.numPeers();
#else
    return 0;
#endif
}

bool AbletonLinkWrapper::isPlaying() const
{
#ifdef ABLETON_LINK_ENABLED
    if (mLinkEnabled)
    {
        auto sessionState = mLink.captureAudioSessionState();
        return sessionState.isPlaying();
    }
    return false;
#else
    return false;
#endif
}

void AbletonLinkWrapper::setIsPlaying(bool playing)
{
#ifdef ABLETON_LINK_ENABLED
    if (mLinkEnabled)
    {
        auto sessionState = mLink.captureAudioSessionState();
        sessionState.setIsPlaying(playing, mHostTimeFilter.sampleTimeToHostTime(mSampleTime));
        mLink.commitAudioSessionState(sessionState);
    }
#else
    juce::ignoreUnused(playing);
#endif
}

double AbletonLinkWrapper::getBeatAtTime(std::chrono::microseconds hostTime, double quantum) const
{
#ifdef ABLETON_LINK_ENABLED
    if (mLinkEnabled)
    {
        auto sessionState = mLink.captureAudioSessionState();
        return sessionState.beatAtTime(hostTime, quantum);
    }
    return 0.0;
#else
    juce::ignoreUnused(hostTime, quantum);
    return 0.0;
#endif
}

double AbletonLinkWrapper::getPhaseAtTime(std::chrono::microseconds hostTime, double quantum) const
{
#ifdef ABLETON_LINK_ENABLED
    if (mLinkEnabled)
    {
        auto sessionState = mLink.captureAudioSessionState();
        return sessionState.phaseAtTime(hostTime, quantum);
    }
    return 0.0;
#else
    juce::ignoreUnused(hostTime, quantum);
    return 0.0;
#endif
}

std::chrono::microseconds AbletonLinkWrapper::getOutputTimeAtSample(int samplePosition, int bufferSize) const
{
#ifdef ABLETON_LINK_ENABLED
    // Calculate the host time for a specific sample position with latency compensation
    const auto outputLatency = std::chrono::microseconds(llround(1.0e6 * bufferSize / mSampleRate));
    return mHostTimeFilter.sampleTimeToHostTime(mSampleTime + samplePosition) + outputLatency;
#else
    juce::ignoreUnused(samplePosition, bufferSize);
    return std::chrono::microseconds(0);
#endif
}

void AbletonLinkWrapper::processAudioBuffer(int bufferSize)
{
#ifdef ABLETON_LINK_ENABLED
    // Update the host time filter with the current sample time
    const auto hostTime = mLink.clock().micros();
    mHostTimeFilter.sampleTimeToHostTime(mSampleTime);
    
    // Advance sample time
    mSampleTime += bufferSize;
#else
    juce::ignoreUnused(bufferSize);
#endif
}