cmake_minimum_required(VERSION 3.15)

# Test executable
add_executable(Pulse24SyncTests
    TestMain.cpp
    PulseGeneratorTests.cpp
    PluginProcessorTests.cpp
    TestUtils.cpp
)

# Link against the main plugin target and JUCE
target_link_libraries(Pulse24SyncTests
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_audio_basics
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
    PUBLIC
        juce::juce_recommended_config_flags
)

# Include source directory for testing
target_include_directories(Pulse24SyncTests 
    PRIVATE 
        ../Source
)

# Add source files that we want to test
target_sources(Pulse24SyncTests
    PRIVATE
        ../Source/PulseGenerator.cpp
        # Note: We don't include PluginProcessor.cpp directly as it has JUCE plugin-specific code
)

# Test-specific compile definitions
target_compile_definitions(Pulse24SyncTests
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_UNIT_TESTS=1
)

# Generate JuceHeader for tests
juce_generate_juce_header(Pulse24SyncTests)

# Enable testing
enable_testing()

# Add test to CTest
add_test(NAME Pulse24SyncUnitTests COMMAND Pulse24SyncTests)

# Set test properties
set_tests_properties(Pulse24SyncUnitTests PROPERTIES 
    TIMEOUT 300
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Windows-specific configuration
if(WIN32)
    # Copy any necessary DLLs for Windows testing
    # This ensures tests can find dependencies at runtime
    set_target_properties(Pulse24SyncTests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )
endif()