#include "PluginProcessor.h"
#include "PluginEditor.h"

Pulse24SyncAudioProcessor::Pulse24SyncAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters(*this, nullptr, juce::Identifier("Pulse24Sync"),
        {
            // Basic parameters
            std::make_unique<juce::AudioParameterBool>("enabled", "Enabled", true),
            std::make_unique<juce::AudioParameterFloat>("pulseVelocity", "Pulse Velocity", 0.0f, 127.0f, 100.0f),
            std::make_unique<juce::AudioParameterInt>("pulseChannel", "Pulse Channel", 1, 16, 1),
            std::make_unique<juce::AudioParameterBool>("syncToHost", "Sync to Host", true),
            std::make_unique<juce::AudioParameterFloat>("manualBPM", "Manual BPM", 60.0f, 200.0f, 120.0f),
            
            // Enhanced sync parameters
            std::make_unique<juce::AudioParameterChoice>("ppqnResolution", "PPQN Resolution", 
                juce::StringArray{"24 PPQN", "48 PPQN", "96 PPQN", "192 PPQN", "480 PPQN"}, 0),
            std::make_unique<juce::AudioParameterInt>("mmcDeviceID", "MMC Device ID", 0, 127, 127),
            std::make_unique<juce::AudioParameterBool>("mmcEnabled", "MMC Enabled", false),
            std::make_unique<juce::AudioParameterBool>("sendClockWhenStopped", "Send Clock When Stopped", false),
            std::make_unique<juce::AudioParameterBool>("sendMMCCommands", "Send MMC Commands", false),
            std::make_unique<juce::AudioParameterBool>("sendSPP", "Send Song Position Pointer", false),
            std::make_unique<juce::AudioParameterBool>("useMidiClock", "Use MIDI Clock", true),
            std::make_unique<juce::AudioParameterBool>("useNotePulses", "Use Note Pulses", false)
        })
{
}

Pulse24SyncAudioProcessor::~Pulse24SyncAudioProcessor()
{
}

const juce::String Pulse24SyncAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Pulse24SyncAudioProcessor::acceptsMidi() const
{
    return true;
}

bool Pulse24SyncAudioProcessor::producesMidi() const
{
    return true;
}

bool Pulse24SyncAudioProcessor::isMidiEffect() const
{
    return false;
}

double Pulse24SyncAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Pulse24SyncAudioProcessor::getNumPrograms()
{
    return 1;
}

int Pulse24SyncAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Pulse24SyncAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String Pulse24SyncAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void Pulse24SyncAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void Pulse24SyncAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);

    // Initialize pulse generator
    pulseGenerator.prepare(sampleRate);

    // Set initial parameters
    updatePulseGeneratorParameters();
}

void Pulse24SyncAudioProcessor::releaseResources()
{
    pulseGenerator.reset();
}

bool Pulse24SyncAudioProcessor::isBusesLayoutSupported(const BusesLayout& busesLayout) const
{
    // Support any layout
    return true;
}

void Pulse24SyncAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't have input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Update pulse generator parameters
    updatePulseGeneratorParameters();

    // Get host tempo information
    juce::AudioPlayHead* playHead = getPlayHead();
    if (playHead != nullptr)
    {
        auto posInfo = playHead->getPosition();
        if (posInfo.hasValue())
        {
            pulseGenerator.setHostTempo(posInfo->getBpm().orFallback(120.0));
            pulseGenerator.setHostIsPlaying(posInfo->getIsPlaying());
            pulseGenerator.setHostPosition(posInfo->getTimeInSeconds().orFallback(0.0));
            pulseGenerator.setHostPPQPosition(posInfo->getPpqPosition().orFallback(0.0));
        }
    }

    // Process pulses and generate MIDI
    pulseGenerator.process(buffer.getNumSamples(), getSampleRate(), midiMessages);
}

void Pulse24SyncAudioProcessor::updatePulseGeneratorParameters()
{
    // Basic parameters
    pulseGenerator.setEnabled(*parameters.getRawParameterValue("enabled"));
    pulseGenerator.setPulseVelocity(*parameters.getRawParameterValue("pulseVelocity"));
    pulseGenerator.setPulseChannel(static_cast<int>(*parameters.getRawParameterValue("pulseChannel")));
    pulseGenerator.setSyncToHost(*parameters.getRawParameterValue("syncToHost"));
    pulseGenerator.setManualBPM(*parameters.getRawParameterValue("manualBPM"));

    // Enhanced sync parameters
    int ppqnIndex = static_cast<int>(*parameters.getRawParameterValue("ppqnResolution"));
    PPQNResolution resolution = static_cast<PPQNResolution>(getPPQNValueFromIndex(ppqnIndex));
    pulseGenerator.setPPQNResolution(resolution);
    
    pulseGenerator.setMMCDeviceID(static_cast<int>(*parameters.getRawParameterValue("mmcDeviceID")));
    pulseGenerator.setMMCEnabled(*parameters.getRawParameterValue("mmcEnabled"));
    pulseGenerator.setSendClockWhenStopped(*parameters.getRawParameterValue("sendClockWhenStopped"));
    pulseGenerator.setSendMMCCommands(*parameters.getRawParameterValue("sendMMCCommands"));
    pulseGenerator.setSendSongPositionPointer(*parameters.getRawParameterValue("sendSPP"));
}

int Pulse24SyncAudioProcessor::getPPQNValueFromIndex(int index)
{
    switch (index)
    {
        case 0: return 24;   // 24 PPQN
        case 1: return 48;   // 48 PPQN
        case 2: return 96;   // 96 PPQN
        case 3: return 192;  // 192 PPQN
        case 4: return 480;  // 480 PPQN
        default: return 24;
    }
}

bool Pulse24SyncAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Pulse24SyncAudioProcessor::createEditor()
{
    return new Pulse24SyncAudioProcessorEditor(*this);
}

void Pulse24SyncAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Pulse24SyncAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pulse24SyncAudioProcessor();
}
