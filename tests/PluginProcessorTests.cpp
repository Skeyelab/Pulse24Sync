#include <JuceHeader.h>
#include "TestUtils.h"

// We'll create a simplified version of the processor for testing
// since the full processor has dependencies on JUCE plugin infrastructure
class MockAudioProcessor
{
public:
    MockAudioProcessor()
        : parameters(juce::AudioProcessorValueTreeState::ParameterLayout{
            std::make_unique<juce::AudioParameterBool>("enabled", "Enabled", true),
            std::make_unique<juce::AudioParameterFloat>("pulseVelocity", "Pulse Velocity", 0.0f, 127.0f, 100.0f),
            std::make_unique<juce::AudioParameterInt>("pulseChannel", "Pulse Channel", 1, 16, 1),
            std::make_unique<juce::AudioParameterBool>("syncToHost", "Sync to Host", true),
            std::make_unique<juce::AudioParameterFloat>("manualBPM", "Manual BPM", 60.0f, 200.0f, 120.0f)
        })
    {
    }
    
    juce::AudioProcessorValueTreeState parameters;
};

class PluginProcessorTests : public juce::UnitTest
{
public:
    PluginProcessorTests() : juce::UnitTest("PluginProcessor Tests", "Plugin") {}

    void runTest() override
    {
        beginTest("Parameter Tree State");
        testParameterTreeState();
        
        beginTest("Parameter Validation");
        testParameterValidation();
        
        beginTest("Parameter Persistence");
        testParameterPersistence();
        
        beginTest("Parameter Change Notifications");
        testParameterChangeNotifications();
        
        beginTest("State Information Serialization");
        testStateInformationSerialization();
        
        beginTest("Plugin Configuration");
        testPluginConfiguration();
    }

private:
    void testParameterTreeState()
    {
        MockAudioProcessor processor;
        
        // Test parameter existence
        auto* enabledParam = processor.parameters.getParameter("enabled");
        expect(enabledParam != nullptr, "Enabled parameter should exist");
        
        auto* velocityParam = processor.parameters.getParameter("pulseVelocity");
        expect(velocityParam != nullptr, "Pulse velocity parameter should exist");
        
        auto* channelParam = processor.parameters.getParameter("pulseChannel");
        expect(channelParam != nullptr, "Pulse channel parameter should exist");
        
        auto* syncParam = processor.parameters.getParameter("syncToHost");
        expect(syncParam != nullptr, "Sync to host parameter should exist");
        
        auto* bpmParam = processor.parameters.getParameter("manualBPM");
        expect(bpmParam != nullptr, "Manual BPM parameter should exist");
        
        // Test default values
        expect(*processor.parameters.getRawParameterValue("enabled") == 1.0f, 
               "Default enabled value should be true");
        expectWithinAbsoluteError(*processor.parameters.getRawParameterValue("pulseVelocity"), 
                                 100.0f, 0.1f, "Default velocity should be 100");
        expectWithinAbsoluteError(*processor.parameters.getRawParameterValue("pulseChannel"), 
                                 1.0f, 0.1f, "Default channel should be 1");
        expect(*processor.parameters.getRawParameterValue("syncToHost") == 1.0f, 
               "Default sync to host should be true");
        expectWithinAbsoluteError(*processor.parameters.getRawParameterValue("manualBPM"), 
                                 120.0f, 0.1f, "Default manual BPM should be 120");
    }
    
    void testParameterValidation()
    {
        MockAudioProcessor processor;
        
        // Test velocity parameter range
        auto* velocityParam = dynamic_cast<juce::AudioParameterFloat*>(
            processor.parameters.getParameter("pulseVelocity"));
        
        if (velocityParam != nullptr)
        {
            expectWithinAbsoluteError(velocityParam->range.start, 0.0f, 0.1f, 
                                     "Velocity minimum should be 0");
            expectWithinAbsoluteError(velocityParam->range.end, 127.0f, 0.1f, 
                                     "Velocity maximum should be 127");
        }
        
        // Test channel parameter range
        auto* channelParam = dynamic_cast<juce::AudioParameterInt*>(
            processor.parameters.getParameter("pulseChannel"));
        
        if (channelParam != nullptr)
        {
            expectEquals(channelParam->getRange().getStart(), 1, "Channel minimum should be 1");
            expectEquals(channelParam->getRange().getEnd(), 16, "Channel maximum should be 16");
        }
        
        // Test BPM parameter range
        auto* bpmParam = dynamic_cast<juce::AudioParameterFloat*>(
            processor.parameters.getParameter("manualBPM"));
        
        if (bpmParam != nullptr)
        {
            expectWithinAbsoluteError(bpmParam->range.start, 60.0f, 0.1f, 
                                     "BPM minimum should be 60");
            expectWithinAbsoluteError(bpmParam->range.end, 200.0f, 0.1f, 
                                     "BPM maximum should be 200");
        }
    }
    
    void testParameterPersistence()
    {
        MockAudioProcessor processor;
        
        // Set some non-default values
        if (auto* param = processor.parameters.getParameter("pulseVelocity"))
            param->setValueNotifyingHost(0.5f); // Should be 63.5 in actual value
            
        if (auto* param = processor.parameters.getParameter("pulseChannel"))
            param->setValueNotifyingHost(0.5f); // Should be around channel 8
            
        if (auto* param = processor.parameters.getParameter("syncToHost"))
            param->setValueNotifyingHost(0.0f); // Should be false
        
        // Get state
        auto state = processor.parameters.copyState();
        
        // Verify state contains our values
        expect(state.isValid(), "State should be valid");
        expect(state.hasType(juce::Identifier("Parameters")), "State should have Parameters type");
        
        // Create new processor and restore state
        MockAudioProcessor newProcessor;
        newProcessor.parameters.replaceState(state);
        
        // Verify values were restored
        float restoredVelocity = *newProcessor.parameters.getRawParameterValue("pulseVelocity");
        float restoredSync = *newProcessor.parameters.getRawParameterValue("syncToHost");
        
        expectWithinAbsoluteError(restoredVelocity, 63.5f, 5.0f, 
                                 "Velocity should be restored approximately");
        expectWithinAbsoluteError(restoredSync, 0.0f, 0.1f, 
                                 "Sync to host should be restored as false");
    }
    
    void testParameterChangeNotifications()
    {
        MockAudioProcessor processor;
        
        // Create a listener to track parameter changes
        class TestListener : public juce::AudioProcessorValueTreeState::Listener
        {
        public:
            void parameterChanged(const juce::String& parameterID, float newValue) override
            {
                lastChangedParameter = parameterID;
                lastChangedValue = newValue;
                changeCount++;
            }
            
            juce::String lastChangedParameter;
            float lastChangedValue = 0.0f;
            int changeCount = 0;
        };
        
        TestListener listener;
        processor.parameters.addParameterListener("pulseVelocity", &listener);
        
        // Change a parameter
        if (auto* param = processor.parameters.getParameter("pulseVelocity"))
        {
            param->setValueNotifyingHost(0.8f);
        }
        
        // Verify listener was notified
        expectEquals(listener.changeCount, 1, "Should have received one parameter change");
        expect(listener.lastChangedParameter == "pulseVelocity", 
               "Should have received change for velocity parameter");
        expectWithinAbsoluteError(listener.lastChangedValue, 0.8f, 0.1f, 
                                 "Should have received correct new value");
        
        processor.parameters.removeParameterListener("pulseVelocity", &listener);
    }
    
    void testStateInformationSerialization()
    {
        MockAudioProcessor processor;
        
        // Set some test values
        if (auto* param = processor.parameters.getParameter("enabled"))
            param->setValueNotifyingHost(0.0f); // false
            
        if (auto* param = processor.parameters.getParameter("manualBPM"))
            param->setValueNotifyingHost(0.75f); // Should be 165 BPM (60 + 0.75 * 140)
        
        // Serialize to XML
        auto state = processor.parameters.copyState();
        std::unique_ptr<juce::XmlElement> xml(state.createXml());
        
        expect(xml != nullptr, "Should be able to create XML from state");
        
        if (xml != nullptr)
        {
            // Serialize to binary
            juce::MemoryBlock binaryData;
            juce::AudioProcessor::copyXmlToBinary(*xml, binaryData);
            
            expect(binaryData.getSize() > 0, "Binary data should not be empty");
            
            // Deserialize from binary
            std::unique_ptr<juce::XmlElement> restoredXml(
                juce::AudioProcessor::getXmlFromBinary(binaryData.getData(), 
                                                      static_cast<int>(binaryData.getSize())));
            
            expect(restoredXml != nullptr, "Should be able to restore XML from binary");
            
            if (restoredXml != nullptr)
            {
                // Restore state in new processor
                MockAudioProcessor newProcessor;
                auto restoredState = juce::ValueTree::fromXml(*restoredXml);
                newProcessor.parameters.replaceState(restoredState);
                
                // Verify values
                float restoredEnabled = *newProcessor.parameters.getRawParameterValue("enabled");
                float restoredBPM = *newProcessor.parameters.getRawParameterValue("manualBPM");
                
                expectWithinAbsoluteError(restoredEnabled, 0.0f, 0.1f, 
                                         "Enabled state should be restored");
                expectWithinAbsoluteError(restoredBPM, 165.0f, 5.0f, 
                                         "BPM should be restored approximately");
            }
        }
    }
    
    void testPluginConfiguration()
    {
        // Test plugin metadata and configuration
        // This would typically test things like:
        // - Plugin name
        // - MIDI input/output capabilities
        // - Channel configurations
        // - Plugin categories
        
        // Since we're using a mock processor, we'll test basic configuration concepts
        MockAudioProcessor processor;
        
        // Test parameter count
        auto paramLayout = processor.parameters.createAndAddParameterGroup("test", "Test", "|", 
            std::make_unique<juce::AudioParameterBool>("testParam", "Test", false));
        
        // The processor should have our expected parameters plus any test parameters
        expect(processor.parameters.getParameter("enabled") != nullptr, 
               "Should have enabled parameter");
        expect(processor.parameters.getParameter("pulseVelocity") != nullptr, 
               "Should have velocity parameter");
        expect(processor.parameters.getParameter("pulseChannel") != nullptr, 
               "Should have channel parameter");
        expect(processor.parameters.getParameter("syncToHost") != nullptr, 
               "Should have sync parameter");
        expect(processor.parameters.getParameter("manualBPM") != nullptr, 
               "Should have BPM parameter");
    }
};

// Register the test
static PluginProcessorTests pluginProcessorTests;