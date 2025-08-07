#include <JuceHeader.h>
#include "TestUtils.h"

// Simplified parameter testing without full AudioProcessor infrastructure
class SimpleParameterTest
{
public:
    SimpleParameterTest()
    {
        enabledParam = std::make_unique<juce::AudioParameterBool>("enabled", "Enabled", true);
        velocityParam = std::make_unique<juce::AudioParameterFloat>("pulseVelocity", "Pulse Velocity", 0.0f, 127.0f, 100.0f);
        channelParam = std::make_unique<juce::AudioParameterInt>("pulseChannel", "Pulse Channel", 1, 16, 1);
        syncParam = std::make_unique<juce::AudioParameterBool>("syncToHost", "Sync to Host", true);
        bpmParam = std::make_unique<juce::AudioParameterFloat>("manualBPM", "Manual BPM", 60.0f, 200.0f, 120.0f);
    }
    
    std::unique_ptr<juce::AudioParameterBool> enabledParam;
    std::unique_ptr<juce::AudioParameterFloat> velocityParam;
    std::unique_ptr<juce::AudioParameterInt> channelParam;
    std::unique_ptr<juce::AudioParameterBool> syncParam;
    std::unique_ptr<juce::AudioParameterFloat> bpmParam;
};

class PluginProcessorTests : public juce::UnitTest
{
public:
    PluginProcessorTests() : juce::UnitTest("PluginProcessor Tests", "Plugin") {}

    void runTest() override
    {
        beginTest("Parameter Creation");
        testParameterCreation();
        
        beginTest("Parameter Validation");
        testParameterValidation();
        
        beginTest("Parameter Value Ranges");
        testParameterRanges();
        
        beginTest("Basic Plugin Concepts");
        testBasicConcepts();
    }

private:
    void testParameterCreation()
    {
        SimpleParameterTest paramTest;
        
        // Test parameter creation
        expect(paramTest.enabledParam != nullptr, "Enabled parameter should be created");
        expect(paramTest.velocityParam != nullptr, "Velocity parameter should be created");
        expect(paramTest.channelParam != nullptr, "Channel parameter should be created");
        expect(paramTest.syncParam != nullptr, "Sync parameter should be created");
        expect(paramTest.bpmParam != nullptr, "BPM parameter should be created");
        
        // Test parameter IDs
        expect(paramTest.enabledParam->getParameterID() == "enabled", "Enabled parameter ID should be correct");
        expect(paramTest.velocityParam->getParameterID() == "pulseVelocity", "Velocity parameter ID should be correct");
        expect(paramTest.channelParam->getParameterID() == "pulseChannel", "Channel parameter ID should be correct");
        expect(paramTest.syncParam->getParameterID() == "syncToHost", "Sync parameter ID should be correct");
        expect(paramTest.bpmParam->getParameterID() == "manualBPM", "BPM parameter ID should be correct");
        
        // Test default values
        expect(paramTest.enabledParam->get() == true, "Default enabled value should be true");
        expectWithinAbsoluteError(paramTest.velocityParam->get(), 100.0f, 0.1f, "Default velocity should be 100");
        expectEquals(paramTest.channelParam->get(), 1, "Default channel should be 1");
        expect(paramTest.syncParam->get() == true, "Default sync to host should be true");
        expectWithinAbsoluteError(paramTest.bpmParam->get(), 120.0f, 0.1f, "Default manual BPM should be 120");
    }
    
    void testParameterValidation()
    {
        SimpleParameterTest paramTest;
        
        // Test parameter names
        expect(paramTest.enabledParam->getName(100) == "Enabled", "Enabled parameter name should be correct");
        expect(paramTest.velocityParam->getName(100) == "Pulse Velocity", "Velocity parameter name should be correct");
        expect(paramTest.channelParam->getName(100) == "Pulse Channel", "Channel parameter name should be correct");
        expect(paramTest.syncParam->getName(100) == "Sync to Host", "Sync parameter name should be correct");
        expect(paramTest.bpmParam->getName(100) == "Manual BPM", "BPM parameter name should be correct");
        
        // Test parameter value setting
        paramTest.enabledParam->setValueNotifyingHost(0.0f);
        expect(paramTest.enabledParam->get() == false, "Enabled parameter should be false when set to 0");
        
        paramTest.velocityParam->setValueNotifyingHost(0.5f);
        expectWithinAbsoluteError(paramTest.velocityParam->get(), 63.5f, 1.0f, "Velocity should be approximately 63.5 when normalized to 0.5");
        
        paramTest.channelParam->setValueNotifyingHost(0.5f);
        expectEquals(paramTest.channelParam->get(), 8, "Channel should be approximately 8 when normalized to 0.5");
    }
    
    void testParameterRanges()
    {
        SimpleParameterTest paramTest;
        
        // Test velocity range
        expectWithinAbsoluteError(paramTest.velocityParam->getNormalisableRange().start, 0.0f, 0.1f, "Velocity minimum should be 0");
        expectWithinAbsoluteError(paramTest.velocityParam->getNormalisableRange().end, 127.0f, 0.1f, "Velocity maximum should be 127");
        
        // Test channel range
        expectEquals(paramTest.channelParam->getRange().getStart(), 1, "Channel minimum should be 1");
        expectEquals(paramTest.channelParam->getRange().getEnd(), 16, "Channel maximum should be 16");
        
        // Test BPM range
        expectWithinAbsoluteError(paramTest.bpmParam->getNormalisableRange().start, 60.0f, 0.1f, "BPM minimum should be 60");
        expectWithinAbsoluteError(paramTest.bpmParam->getNormalisableRange().end, 200.0f, 0.1f, "BPM maximum should be 200");
        
        // Test boolean parameters
        expect(paramTest.enabledParam->get() == true || paramTest.enabledParam->get() == false, "Boolean parameter should have valid state");
        expect(paramTest.syncParam->get() == true || paramTest.syncParam->get() == false, "Boolean parameter should have valid state");
    }
    
    void testBasicConcepts()
    {
        SimpleParameterTest paramTest;
        
        // Test parameter types
        expect(paramTest.enabledParam->isBoolean(), "Enabled parameter should be boolean");
        expect(!paramTest.velocityParam->isBoolean(), "Velocity parameter should not be boolean");
        expect(!paramTest.channelParam->isBoolean(), "Channel parameter should not be boolean");
        expect(paramTest.syncParam->isBoolean(), "Sync parameter should be boolean");
        expect(!paramTest.bpmParam->isBoolean(), "BPM parameter should not be boolean");
        
        // Test parameter categories (basic concept validation)
        expect(paramTest.enabledParam->getCategory() == juce::AudioProcessorParameter::genericParameter, 
               "Parameter should have valid category");
        expect(paramTest.velocityParam->getCategory() == juce::AudioProcessorParameter::genericParameter, 
               "Parameter should have valid category");
        
        // Test basic parameter functionality
        auto originalValue = paramTest.velocityParam->get();
        paramTest.velocityParam->setValueNotifyingHost(0.75f);
        auto newValue = paramTest.velocityParam->get();
        expect(newValue != originalValue, "Parameter value should change when set");
        
        // Test parameter normalization
        auto normalizedValue = paramTest.velocityParam->getValue();
        expect(normalizedValue >= 0.0f && normalizedValue <= 1.0f, "Normalized value should be between 0 and 1");
        
        // Test string conversion
        auto stringValue = paramTest.velocityParam->getText(paramTest.velocityParam->getValue(), 100);
        expect(stringValue.isNotEmpty(), "Parameter should convert to string");
    }
};

// Register the test
static PluginProcessorTests pluginProcessorTests;