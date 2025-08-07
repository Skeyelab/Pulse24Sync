cmake_minimum_required(VERSION 3.15)

# Create a console application for tests using JUCE
juce_add_console_app(Pulse24SyncTests
    PRODUCT_NAME "Pulse24Sync Tests"
)

# Add test source files
target_sources(Pulse24SyncTests
    PRIVATE
        TestMain.cpp
        PulseGeneratorTests.cpp
        PluginProcessorTests.cpp
        TestUtils.cpp
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
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Pulse24SyncTests,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Pulse24SyncTests,JUCE_VERSION>"
)

# Include source directory for testing
target_include_directories(Pulse24SyncTests 
    PRIVATE 
        ../Source
)

# Link JUCE modules
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
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

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