/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Pulse24SyncAudioProcessor::Pulse24SyncAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
      , parameters(*this, nullptr, "Parameters", createParameterLayout())
{
    // Get parameter pointers
    volumeParameter = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("volume"));
    enabledParameter = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("enabled"));
    widthParameter = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("width"));
}

Pulse24SyncAudioProcessor::~Pulse24SyncAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout Pulse24SyncAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterBool>("enabled", "Enabled", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("volume", "Volume", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("width", "Pulse Width", 0.01f, 0.5f, 0.1f));

    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String Pulse24SyncAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Pulse24SyncAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Pulse24SyncAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Pulse24SyncAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Pulse24SyncAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Pulse24SyncAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Pulse24SyncAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Pulse24SyncAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Pulse24SyncAudioProcessor::getProgramName (int index)
{
    return {};
}

void Pulse24SyncAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Pulse24SyncAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    pulseCounter = 0.0;
    updateTimingInfo();
}

void Pulse24SyncAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Pulse24SyncAudioProcessor::updateTimingInfo()
{
    // Get timing information from the host
    juce::AudioPlayHead* playHead = getPlayHead();
    if (playHead != nullptr)
    {
        juce::AudioPlayHead::CurrentPositionInfo posInfo;
        if (playHead->getCurrentPosition(posInfo))
        {
            currentTempo = posInfo.bpm;
            currentPpq = posInfo.ppqPosition;

            // Calculate pulse period: 24 pulses per quarter note
            // pulsePeriod = samples per quarter note / 24
            double samplesPerQuarterNote = (60.0 / currentTempo) * sampleRate;
            pulsePeriod = samplesPerQuarterNote / 24.0;

            // Calculate pulse duration based on width parameter
            pulseDuration = pulsePeriod * pulseWidth;
        }
    }
}

void Pulse24SyncAudioProcessor::generatePulse(juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    if (!pulseEnabled || pulsePeriod <= 0.0)
        return;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        int globalSample = startSample + sample;

        // Check if we should generate a pulse at this sample
        if (pulseCounter >= pulsePeriod)
        {
            pulseCounter = 0.0;
        }

        // Generate pulse if we're within the pulse duration
        if (pulseCounter < pulseDuration)
        {
            float pulseValue = pulseVolume;

            // Apply to all output channels
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                float* channelData = buffer.getWritePointer(channel);
                channelData[globalSample] += pulseValue;
            }
        }

        pulseCounter += 1.0;
    }
}

//==============================================================================
// Pulse generation methods
void Pulse24SyncAudioProcessor::setPulseEnabled(bool enabled)
{
    pulseEnabled = enabled;
    if (enabledParameter != nullptr)
        enabledParameter->setValueNotifyingHost(enabled ? 1.0f : 0.0f);
}

bool Pulse24SyncAudioProcessor::isPulseEnabled() const
{
    return pulseEnabled;
}

void Pulse24SyncAudioProcessor::setPulseVolume(float volume)
{
    pulseVolume = juce::jlimit(0.0f, 1.0f, volume);
    if (volumeParameter != nullptr)
        volumeParameter->setValueNotifyingHost(pulseVolume);
}

float Pulse24SyncAudioProcessor::getPulseVolume() const
{
    return pulseVolume;
}

void Pulse24SyncAudioProcessor::setPulseWidth(float width)
{
    pulseWidth = juce::jlimit(0.01f, 0.5f, width);
    if (widthParameter != nullptr)
        widthParameter->setValueNotifyingHost(pulseWidth);
    updateTimingInfo();
}

float Pulse24SyncAudioProcessor::getPulseWidth() const
{
    return pulseWidth;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Pulse24SyncAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Pulse24SyncAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Update timing information
    updateTimingInfo();

    // Update parameters from UI
    if (enabledParameter != nullptr)
        pulseEnabled = enabledParameter->get();
    if (volumeParameter != nullptr)
        pulseVolume = volumeParameter->get();
    if (widthParameter != nullptr)
        pulseWidth = widthParameter->get();

    // Generate pulses
    generatePulse(buffer, 0, buffer.getNumSamples());
}

//==============================================================================
bool Pulse24SyncAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Pulse24SyncAudioProcessor::createEditor()
{
    return new Pulse24SyncAudioProcessorEditor (*this);
}

//==============================================================================
void Pulse24SyncAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Pulse24SyncAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pulse24SyncAudioProcessor();
}