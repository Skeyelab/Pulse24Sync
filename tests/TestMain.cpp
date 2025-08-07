#include <JuceHeader.h>
#include "TestUtils.h"

// Forward declarations of test classes
class PulseGeneratorTests;
class PluginProcessorTests;

class TestApplication : public juce::ConsoleApplication
{
public:
    TestApplication()
    {
        
    }

    void initialise(const juce::String& commandLine) override
    {
        auto args = getArgumentList();
        
        bool verbose = false;
        juce::String testCategory = "";
        
        for (const auto& arg : args)
        {
            if (arg == "--verbose" || arg == "-v")
                verbose = true;
            else if (arg.startsWith("--category="))
                testCategory = arg.fromFirstOccurrenceOf("=", false, false);
            else if (arg == "--help" || arg == "-h")
            {
                std::cout << "Pulse24Sync Test Runner\n"
                          << "Usage: TestApp [options]\n"
                          << "Options:\n"
                          << "  --verbose, -v     Verbose output\n"
                          << "  --category=NAME   Run only tests in specified category\n"
                          << "  --help, -h        Show this help\n";
                quit();
                return;
            }
        }
        
        runTests(verbose, testCategory);
        quit();
    }

    void shutdown() override
    {
    }

private:
    void runTests(bool verbose, const juce::String& testCategory)
    {
    
    // Create a console logger for test output
    std::unique_ptr<juce::FileLogger> logger;
    
    // Try to create a log file, fall back to console if it fails
    auto logFile = juce::File::getCurrentWorkingDirectory().getChildFile("test_results.log");
    logger = std::make_unique<juce::FileLogger>(logFile, "Pulse24Sync Test Results", 1024 * 1024);
    
    if (logger->getLogFile().exists())
    {
        juce::Logger::setCurrentLogger(logger.get());
        juce::Logger::writeToLog("=== Pulse24Sync Unit Tests Started ===");
    }
    
        // Run all unit tests
        juce::UnitTestRunner testRunner;
    if (testCategory.isNotEmpty())
    {
        testRunner.runTestsInCategory(testCategory);
    }
    else
    {
        testRunner.runAllTests();
    }
    
    // Get results
    auto numTests = testRunner.getNumResults();
    int failures = 0;
    
    for (int i = 0; i < numTests; ++i)
    {
        auto* result = testRunner.getResult(i);
        if (result->failures > 0)
            failures += result->failures;
            
        if (verbose || result->failures > 0)
        {
            std::cout << "Test: " << result->unitTestName 
                      << " - " << result->subcategoryName << std::endl;
            std::cout << "  Passes: " << result->passes 
                      << ", Failures: " << result->failures << std::endl;
                      
            if (result->failures > 0)
            {
                for (auto& message : result->messages)
                {
                    std::cout << "    " << message << std::endl;
                }
            }
        }
    }
    
    // Summary
    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Total tests: " << numTests << std::endl;
    std::cout << "Failures: " << failures << std::endl;
    std::cout << "Result: " << (failures == 0 ? "PASSED" : "FAILED") << std::endl;
    
        if (logger)
        {
            juce::Logger::writeToLog("=== Pulse24Sync Unit Tests Completed ===");
            juce::Logger::writeToLog("Total failures: " + juce::String(failures));
            juce::Logger::setCurrentLogger(nullptr);
        }
        
        // Set exit code for the application
        if (failures > 0)
            setApplicationReturnValue(1);
        else
            setApplicationReturnValue(0);
    }
};

START_JUCE_APPLICATION(TestApplication)