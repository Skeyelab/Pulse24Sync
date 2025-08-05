cmake_minimum_required(VERSION 3.15)
project(Pulse24Sync VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and setup JUCE
include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 6.1.6
)

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
    JUCE_MODULES
        juce_audio_utils
        juce_audio_processors
        juce_core
        juce_data_structures
        juce_events
        juce_graphics
        juce_gui_basics
        juce_gui_extra
        juce_audio_basics
        juce_audio_devices
        juce_audio_formats
        juce_audio_plugin_client
        juce_audio_plugin_host
        juce_dsp
)

# Add source files
target_sources(Pulse24Sync
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/PulseGenerator.cpp
)

# Set compiler flags
if(MSVC)
    target_compile_options(Pulse24Sync PRIVATE /W4)
else()
    target_compile_options(Pulse24Sync PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Disable JUCE extras to avoid build issues
set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
