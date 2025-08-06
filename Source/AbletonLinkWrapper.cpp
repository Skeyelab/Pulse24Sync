#include "AbletonLinkWrapper.h"

AbletonLinkWrapper::AbletonLinkWrapper()
{
    // Note: This is a simplified implementation for standalone builds
    // In a full implementation, this would connect to Ableton Link
}

void AbletonLinkWrapper::prepare(double sampleRate)
{
    mSampleRate = sampleRate;
    mSampleTime = 0;
    mPhase = 0.0;
}

void AbletonLinkWrapper::reset()
{
    mSampleTime = 0;
    mPhase = 0.0;
}

void AbletonLinkWrapper::setLinkEnabled(bool enabled)
{
    mLinkEnabled = enabled;
    if (enabled)
    {
        DBG("Ableton Link enabled (simplified mode for Projucer builds)");
    }
    else
    {
        DBG("Ableton Link disabled");
    }
}

bool AbletonLinkWrapper::isLinkEnabled() const
{
    return mLinkEnabled;
}

void AbletonLinkWrapper::setTempo(double bpm)
{
    mTempo = bpm;
}

double AbletonLinkWrapper::getTempo() const
{
    return mTempo;
}

std::size_t AbletonLinkWrapper::getNumPeers() const
{
    // In a real implementation, this would return the actual peer count
    return mLinkEnabled ? 1 : 0; // Simulate having at least one peer when enabled
}

bool AbletonLinkWrapper::isPlaying() const
{
    return mIsPlaying;
}

void AbletonLinkWrapper::setIsPlaying(bool playing)
{
    mIsPlaying = playing;
}

void AbletonLinkWrapper::processAudioBuffer(int numSamples)
{
    if (mLinkEnabled && mIsPlaying)
    {
        // Update internal timing
        double samplesPerBeat = (mSampleRate * 60.0) / mTempo;
        double phaseIncrement = numSamples / samplesPerBeat;
        mPhase += phaseIncrement;
        
        // Keep phase in range [0, 4) for a 4-beat cycle
        while (mPhase >= 4.0)
            mPhase -= 4.0;
    }
    
    mSampleTime += numSamples;
}

std::chrono::microseconds AbletonLinkWrapper::getOutputTimeAtSample(int sampleIndex, int bufferSize) const
{
    juce::ignoreUnused(bufferSize);
    
    // Convert sample position to microseconds
    double timeInSeconds = (mSampleTime + sampleIndex) / mSampleRate;
    return std::chrono::microseconds(static_cast<int64_t>(timeInSeconds * 1000000.0));
}

double AbletonLinkWrapper::getPhaseAtTime(std::chrono::microseconds hostTime, double quantum) const
{
    juce::ignoreUnused(hostTime);
    
    if (!mLinkEnabled || !mIsPlaying)
        return 0.0;
    
    // Return the current phase scaled by quantum
    return std::fmod(mPhase * (quantum / 4.0), quantum);
}