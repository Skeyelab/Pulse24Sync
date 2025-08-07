# Testing Guide for Pulse24Sync

This document provides comprehensive information about the testing framework for Pulse24Sync, including how to run tests, write new tests, and understand the test architecture.

## Table of Contents

- [Overview](#overview)
- [Running Tests](#running-tests)
- [Test Structure](#test-structure)
- [Writing New Tests](#writing-new-tests)
- [Test Categories](#test-categories)
- [Continuous Integration](#continuous-integration)
- [Troubleshooting](#troubleshooting)

## Overview

Pulse24Sync uses JUCE's built-in unit testing framework to ensure code quality and prevent regressions. The test suite covers:

- **Core Logic**: PulseGenerator functionality, timing accuracy, MIDI generation
- **Plugin Interface**: Parameter management, state persistence, host integration
- **Edge Cases**: Boundary conditions, error handling, performance validation

### Test Framework Features

- **JUCE UnitTest Framework**: Native JUCE testing with familiar APIs
- **Automated Test Discovery**: Tests are automatically registered and discovered
- **Categorized Testing**: Tests organized by functionality (Core, Plugin, UI, etc.)
- **Performance Validation**: Timing accuracy and audio-rate processing tests
- **Mock Objects**: Simplified test doubles for complex dependencies
- **Comprehensive Assertions**: Floating-point precision, timing validation, MIDI event checking

## Running Tests

### Quick Start

```bash
# Run all tests
./run-tests.sh

# Run with verbose output
./run-tests.sh --verbose

# Run only core functionality tests
./run-tests.sh --category=Core

# Clean build and run tests in debug mode
./run-tests.sh --clean --debug
```

### Manual Build and Run

```bash
# Create test build
mkdir build-tests && cd build-tests

# Configure with tests enabled
cmake .. -DCMAKE_BUILD_TYPE=Release -DPULSE24SYNC_BUILD_TESTS=ON

# Build
make -j$(nproc)  # Linux
make -j$(sysctl -n hw.ncpu)  # macOS

# Run tests
cd tests
./Pulse24SyncTests

# Or use CTest
cd ..
ctest --output-on-failure
```

### Command Line Options

The test runner supports several options:

- `--verbose, -v`: Detailed test output including pass/fail for each assertion
- `--debug`: Build tests in Debug mode for better debugging information
- `--clean`: Remove previous build artifacts before building
- `--category=NAME`: Run only tests in the specified category
- `--help, -h`: Show usage information

### Test Output

Successful test run example:
```
🧪 Running Pulse24Sync Test Suite
====================================
📋 CMake version: 3.22.1
⚙️  Configuring tests with CMake...
🔨 Building tests...
✅ Tests built successfully!

🏃 Running tests...
====================

=== Test Summary ===
Total tests: 2
Failures: 0
Result: PASSED

🎉 All tests passed successfully!
```

## Test Structure

### Directory Layout

```
tests/
├── CMakeLists.txt              # Test build configuration
├── TestMain.cpp                # Test runner and main entry point
├── TestUtils.h                 # Testing utilities and helpers
├── TestUtils.cpp               # Testing utility implementations
├── PulseGeneratorTests.cpp     # Core pulse generation tests
└── PluginProcessorTests.cpp    # Plugin interface tests
```

### Test Organization

Tests are organized into categories:

- **Core**: Fundamental functionality (PulseGenerator, timing, MIDI)
- **Plugin**: Plugin-specific features (parameters, state, host integration)
- **UI**: User interface components and interactions
- **Performance**: Timing accuracy and performance validation

### Test Utilities

The `TestUtils` namespace provides helpful utilities:

```cpp
// Precision comparisons
TestUtils::isApproximatelyEqual(actual, expected, tolerance)

// Timing calculations
TestUtils::TimingUtils::bpmToPulseRate(bpm)
TestUtils::TimingUtils::pulseRateToInterval(rate, sampleRate)

// Test data generation
TestUtils::TestDataGenerator::generateBPMTestValues()
TestUtils::TestDataGenerator::generateChannelTestValues()

// Mock MIDI buffer for testing
TestUtils::MockMidiBuffer buffer;
buffer.hasNoteOn(channel, noteNumber);
```

## Writing New Tests

### Creating a New Test Class

```cpp
#include <JuceHeader.h>
#include "TestUtils.h"
#include "YourClassToTest.h"

class YourClassTests : public juce::UnitTest
{
public:
    YourClassTests() : juce::UnitTest("YourClass Tests", "Core") {}

    void runTest() override
    {
        beginTest("Test Description");
        testSomeFeature();
        
        beginTest("Another Test");
        testAnotherFeature();
    }

private:
    void testSomeFeature()
    {
        YourClass instance;
        
        // Setup
        instance.setSomeValue(42);
        
        // Test
        auto result = instance.getSomeValue();
        
        // Verify
        expectEquals(result, 42, "Value should be set correctly");
    }
    
    void testAnotherFeature()
    {
        // Test implementation
    }
};

// Register the test (important!)
static YourClassTests yourClassTests;
```

### Best Practices

1. **Descriptive Test Names**: Use clear, descriptive names for test methods and assertions
2. **Single Responsibility**: Each test method should focus on one specific behavior
3. **Setup and Teardown**: Use constructor/destructor or setup/teardown methods for common initialization
4. **Edge Cases**: Test boundary conditions, invalid inputs, and error scenarios
5. **Performance**: Include timing validation for audio-rate processing code

### Common Assertions

```cpp
// Basic assertions
expect(condition, "Error message");
expectEquals(actual, expected, "Values should match");

// Floating-point comparisons
expectWithinAbsoluteError(actual, expected, tolerance, "Should be approximately equal");

// MIDI event validation
expect(midiBuffer.hasNoteOn(channel, noteNumber), "Should generate note on");
expectEquals(message.getVelocity(), expectedVelocity, "Velocity should match");

// Timing validation
expectWithinAbsoluteError(actualInterval, expectedInterval, tolerance, "Timing should be accurate");
```

### Testing Audio Components

When testing audio processing components:

```cpp
void testAudioProcessing()
{
    const double sampleRate = 44100.0;
    const int blockSize = 512;
    
    AudioComponent component;
    component.prepare(sampleRate);
    
    juce::AudioBuffer<float> audioBuffer(2, blockSize);
    juce::MidiBuffer midiBuffer;
    
    // Fill with test audio data
    fillBufferWithTestSignal(audioBuffer);
    
    // Process
    component.process(audioBuffer, midiBuffer);
    
    // Validate results
    validateAudioOutput(audioBuffer);
    validateMidiOutput(midiBuffer);
}
```

### Testing Parameter Changes

```cpp
void testParameterChanges()
{
    MockAudioProcessor processor;
    
    // Test parameter existence
    auto* param = processor.parameters.getParameter("someParam");
    expect(param != nullptr, "Parameter should exist");
    
    // Test value changes
    param->setValueNotifyingHost(0.5f);
    float value = *processor.parameters.getRawParameterValue("someParam");
    expectWithinAbsoluteError(value, expectedValue, 0.1f, "Parameter should update");
}
```

## Test Categories

### Core Tests

Test fundamental functionality that doesn't depend on plugin infrastructure:

- Pulse generation algorithms
- Timing calculations
- MIDI message creation
- Mathematical operations
- Data structure manipulation

### Plugin Tests

Test plugin-specific functionality:

- Parameter management
- State serialization/deserialization
- Host integration
- Plugin metadata
- Audio processor interface compliance

### UI Tests (Future)

Test user interface components:

- Component layout and sizing
- Parameter binding
- User interaction handling
- Visual feedback
- Accessibility features

### Performance Tests

Test timing-critical functionality:

- Audio processing latency
- Pulse timing accuracy
- Memory allocation patterns
- CPU usage optimization
- Real-time safety

## Continuous Integration

### Local Pre-commit Testing

Before committing code:

```bash
# Run full test suite
./run-tests.sh

# Run specific categories
./run-tests.sh --category=Core
./run-tests.sh --category=Plugin

# Test both debug and release builds
./run-tests.sh --debug
./run-tests.sh  # (release is default)
```

### Build Script Integration

The main build scripts can optionally run tests:

```bash
# Build with testing
cmake .. -DPULSE24SYNC_BUILD_TESTS=ON
make
ctest

# Build without testing (default for releases)
cmake .. -DPULSE24SYNC_BUILD_TESTS=OFF
make
```

### GitHub Actions

The project includes comprehensive GitHub Actions workflows for automated testing:

#### **Main Test Workflow** (`.github/workflows/test.yml`)
Runs on every push and pull request:

- **Multi-platform testing**: Ubuntu, macOS, Windows
- **Multiple build configurations**: Release, Debug, RelWithDebInfo
- **Sanitizer testing**: AddressSanitizer, UndefinedBehaviorSanitizer  
- **Code coverage**: Automatic coverage reporting with Codecov
- **Dependency caching**: JUCE downloads are cached for faster builds

#### **PR Check Workflow** (`.github/workflows/pr-check.yml`)
Fast validation for pull requests:

- **Quick test run**: Ubuntu-only for fast feedback
- **Build verification**: Ensures plugin builds on all platforms
- **Automatic comments**: Test results posted directly to PR

#### **Badge Status Workflow** (`.github/workflows/badge-status.yml`)
Generates status badges:

- **Daily updates**: Test status badges updated automatically
- **Real-time status**: Shows current test passing/failing status
- **Coverage tracking**: Coverage percentage badges (when enabled)

#### **Workflow Features**

- ✅ **Parallel execution**: Multiple jobs run simultaneously
- ✅ **Artifact upload**: Test logs saved on failure
- ✅ **Cache optimization**: Dependencies cached between runs
- ✅ **Matrix testing**: Multiple OS/compiler combinations
- ✅ **Failure isolation**: Individual job failures don't cancel others
- ✅ **Manual triggering**: All workflows can be triggered manually

#### **Setting Up Badges**

To enable status badges in your README:

1. **Create a secret gist** for badge data storage
2. **Add repository secrets**:
   - `BADGE_GIST_ID`: Your gist ID for badge storage
3. **Add badges to README**:
   ```markdown
   ![Tests](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/USERNAME/GIST_ID/raw/pulse24sync-tests.json)
   ![Coverage](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/USERNAME/GIST_ID/raw/pulse24sync-coverage.json)
   ```

## Troubleshooting

### Common Issues

**Test executable not found**
```bash
Error: Test executable was not built successfully.
```
- Ensure CMake configuration includes `-DPULSE24SYNC_BUILD_TESTS=ON`
- Check that all test source files are listed in `tests/CMakeLists.txt`
- Verify JUCE dependencies are properly configured

**Tests fail to compile**
```
error: 'SomeClass' was not declared in this scope
```
- Add missing includes to test files
- Ensure test target includes necessary source directories
- Check that dependencies are properly linked

**Tests pass individually but fail together**
- Check for static initialization order issues
- Ensure tests don't have interdependencies
- Look for global state that isn't properly reset between tests

**Timing tests are flaky**
- Increase tolerance for timing comparisons
- Use relative rather than absolute timing validation
- Consider system load and threading issues

### Debugging Tests

**Enable verbose output:**
```bash
./run-tests.sh --verbose
```

**Build in debug mode:**
```bash
./run-tests.sh --debug
```

**Run specific test category:**
```bash
./run-tests.sh --category=Core
```

**Use a debugger:**
```bash
cd build-tests/tests
gdb ./Pulse24SyncTests
# or
lldb ./Pulse24SyncTests
```

### Performance Debugging

**Profile test execution:**
```bash
cd build-tests/tests
valgrind --tool=callgrind ./Pulse24SyncTests
```

**Check for memory leaks:**
```bash
valgrind --leak-check=full ./Pulse24SyncTests
```

### Getting Help

- Check test logs: `build-tests/tests/test_results.log`
- Review CMake configuration: `build-tests/CMakeCache.txt`
- Examine test output for specific failure messages
- Use the `--verbose` flag for detailed assertion information

## Contributing New Tests

When adding new features to Pulse24Sync:

1. **Write tests first** (TDD approach) or alongside implementation
2. **Cover edge cases** including error conditions and boundary values
3. **Maintain test coverage** for all public APIs
4. **Update documentation** when adding new test categories or utilities
5. **Ensure tests are fast** to maintain rapid feedback cycles

### Test Review Checklist

- [ ] Tests have descriptive names and clear assertions
- [ ] Edge cases and error conditions are covered
- [ ] Tests don't depend on external resources or timing
- [ ] Performance-critical code includes timing validation
- [ ] Tests are properly categorized
- [ ] No test interdependencies or shared mutable state
- [ ] Documentation is updated for new test utilities

---

For more information about the Pulse24Sync project, see the main [README.md](../README.md).