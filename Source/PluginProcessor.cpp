#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

Pulse24SyncAudioProcessor::Pulse24SyncAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters(*this, nullptr, juce::Identifier("Pulse24Sync"),
        {
            std::make_unique<juce::AudioParameterBool>(PluginParams::enabled, PluginParams::name_enabled, true),
            std::make_unique<juce::AudioParameterFloat>(PluginParams::pulseVelocity, PluginParams::name_pulseVelocity, 0.0f, 127.0f, 100.0f),
            std::make_unique<juce::AudioParameterFloat>(PluginParams::pulseWidth, PluginParams::name_pulseWidth, 1.0f, 50.0f, 22.0f),
            std::make_unique<juce::AudioParameterBool>(PluginParams::syncToHost, PluginParams::name_syncToHost, true),
            std::make_unique<juce::AudioParameterFloat>(PluginParams::manualBPM, PluginParams::name_manualBPM, 60.0f, 200.0f, 120.0f)
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
    syncParametersToEngine();
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

    // Clear the output buffer first (we want to generate audio, not pass through input)
    buffer.clear();

    // Update pulse generator parameters
    syncParametersToEngine();

    // Get host tempo information
    juce::AudioPlayHead* playHead = getPlayHead();
    if (playHead != nullptr)
    {
        auto posInfo = playHead->getPosition();
        if (posInfo.hasValue())
        {
            // Get BPM and update it
            double currentBPM = posInfo->getBpm().orFallback(120.0);
            pulseGenerator.setHostTempo(currentBPM);
            
            // Get playing state
            pulseGenerator.setHostIsPlaying(posInfo->getIsPlaying());
            
            // Get time position
            pulseGenerator.setHostPosition(posInfo->getTimeInSeconds().orFallback(0.0));
            
            // Get PPQ position for accurate sync
            double ppqPosition = posInfo->getPpqPosition().orFallback(0.0);
            pulseGenerator.setHostPPQPosition(ppqPosition);
        }
        else
        {
            // Fallback if position info not available
            pulseGenerator.setHostTempo(120.0);
            pulseGenerator.setHostIsPlaying(false);
            pulseGenerator.setHostPosition(0.0);
            pulseGenerator.setHostPPQPosition(0.0);
        }
    }
    else
    {
        // Fallback if playhead not available
        pulseGenerator.setHostTempo(120.0);
        pulseGenerator.setHostIsPlaying(false);
        pulseGenerator.setHostPosition(0.0);
        pulseGenerator.setHostPPQPosition(0.0);
    }

    // Process pulses and generate audio
    pulseGenerator.process(buffer.getNumSamples(), getSampleRate(), buffer);
    
    // Clear MIDI messages as we're not using them anymore
    midiMessages.clear();
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

void Pulse24SyncAudioProcessor::syncParametersToEngine()
{
    pulseGenerator.setEnabled(*parameters.getRawParameterValue(PluginParams::enabled));
    pulseGenerator.setPulseVelocity(*parameters.getRawParameterValue(PluginParams::pulseVelocity));
    pulseGenerator.setPulseWidth(*parameters.getRawParameterValue(PluginParams::pulseWidth));
    pulseGenerator.setSyncToHost(*parameters.getRawParameterValue(PluginParams::syncToHost));
    pulseGenerator.setManualBPM(*parameters.getRawParameterValue(PluginParams::manualBPM));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pulse24SyncAudioProcessor();
}
