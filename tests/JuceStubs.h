#pragma once

#include <vector>
#include <cmath>

namespace juce {

template <typename T>
struct MathConstants {
    static constexpr T pi = static_cast<T>(3.14159265358979323846264338327950288L);
};

// Variadic template version to avoid C-style varargs issues with non-POD types
template <typename... Ts>
inline void ignoreUnused(const Ts&...) noexcept {}

template <typename SampleType>
class AudioBuffer {
public:
    AudioBuffer(int numChannels, int numSamples)
        : channels(numChannels), samples(numSamples), data(numChannels, std::vector<SampleType>(numSamples, SampleType{})) {}

    void clear() {
        for (auto& ch : data) std::fill(ch.begin(), ch.end(), SampleType{});
    }

    int getNumChannels() const { return channels; }
    int getNumSamples() const { return samples; }

    const SampleType* getReadPointer(int channel) const { return data[channel].data(); }

    void addSample(int channel, int sampleIndex, SampleType value) {
        data[channel][sampleIndex] += value;
    }

private:
    int channels;
    int samples;
    std::vector<std::vector<SampleType>> data;
};

inline float jlimit(float lower, float upper, float value) {
    return std::max(lower, std::min(upper, value));
}

} // namespace juce