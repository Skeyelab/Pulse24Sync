cmake_minimum_required(VERSION 3.15)

# Create a standard executable
add_executable(Pulse24SyncTests
    TestMain.cpp
    PulseGeneratorTests.cpp
    PluginProcessorTests.cpp
    TestUtils.cpp
    ../Source/PulseGenerator.cpp
)

# Set C++ standard
set_property(TARGET Pulse24SyncTests PROPERTY CXX_STANDARD 17)
set_property(TARGET Pulse24SyncTests PROPERTY CXX_STANDARD_REQUIRED ON)

# Include directories
target_include_directories(Pulse24SyncTests PRIVATE
    ../Source
    ${CMAKE_BINARY_DIR}/Pulse24Sync_artefacts/JuceLibraryCode
)

# Compile definitions
target_compile_definitions(Pulse24SyncTests PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_UNIT_TESTS=1
    JUCE_STANDALONE_APPLICATION=1
)

# Link libraries
target_link_libraries(Pulse24SyncTests PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_audio_basics
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_gui_basics
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# Enable testing
enable_testing()

# Add test
add_test(NAME Pulse24SyncUnitTests COMMAND Pulse24SyncTests)

# Test properties
set_tests_properties(Pulse24SyncUnitTests PROPERTIES
    TIMEOUT 300
)