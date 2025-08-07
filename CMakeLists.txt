cmake_minimum_required(VERSION 3.15)
project(Pulse24Sync VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set macOS deployment target to avoid deprecated API issues
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0")
endif()

# Download and setup JUCE
include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
)

# Disable JUCE extras to avoid build issues with macOS 15.0 and Linux
set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(JUCE)

# Create the VST plugin
juce_add_plugin(Pulse24Sync
    COMPANY_NAME "Pulse24Sync"
    PLUGIN_NAME "Pulse24Sync"
    COMPANY_WEBSITE "https://github.com/yourusername/Pulse24Sync"
    COMPANY_EMAIL "your.email@example.com"
    PLUGIN_MANUFACTURER_CODE Puls
    PLUGIN_CODE P24s
    FORMATS VST3 Standalone
    PRODUCT_NAME "Pulse24Sync"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    VST3_CATEGORIES "Fx"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    COPY_PLUGIN_AFTER_BUILD TRUE
)

# Add source files
target_sources(Pulse24Sync
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/PulseGenerator.cpp
)

# Add JUCE modules
target_compile_definitions(Pulse24Sync
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# Generate JuceHeader.h
juce_generate_juce_header(Pulse24Sync)

# Link JUCE modules
target_link_libraries(Pulse24Sync
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Linux-specific configurations
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)

    # Find audio systems
    pkg_check_modules(ALSA REQUIRED alsa)
    pkg_check_modules(JACK REQUIRED jack)

    # Find GUI libraries
    pkg_check_modules(FREETYPE REQUIRED freetype2)
    pkg_check_modules(GL REQUIRED gl)
    pkg_check_modules(CURL REQUIRED libcurl)

    # Add include directories
    target_include_directories(Pulse24Sync PRIVATE
        ${ALSA_INCLUDE_DIRS}
        ${JACK_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
        ${GL_INCLUDE_DIRS}
        ${CURL_INCLUDE_DIRS}
    )

    # Add link libraries
    target_link_libraries(Pulse24Sync PRIVATE
        ${ALSA_LIBRARIES}
        ${JACK_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${GL_LIBRARIES}
        ${CURL_LIBRARIES}
    )

    # Add compile flags
    target_compile_options(Pulse24Sync PRIVATE
        ${ALSA_CFLAGS_OTHER}
        ${JACK_CFLAGS_OTHER}
        ${FREETYPE_CFLAGS_OTHER}
        ${GL_CFLAGS_OTHER}
        ${CURL_CFLAGS_OTHER}
    )
endif()

# Testing configuration
option(PULSE24SYNC_BUILD_TESTS "Build tests for Pulse24Sync" ON)

if(PULSE24SYNC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
