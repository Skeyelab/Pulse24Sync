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
    EDITOR_REQUIRES_KEYBOARD_FOCUS FALSE
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
)

# Set include directories
target_include_directories(Pulse24Sync
    PRIVATE
        Source
)

# Platform-specific settings
if(UNIX AND NOT APPLE)
    # Linux-specific settings
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALSA REQUIRED alsa)
    pkg_check_modules(JACK REQUIRED jack)

    target_link_libraries(Pulse24Sync
        PRIVATE
            ${ALSA_LIBRARIES}
            ${JACK_LIBRARIES}
    )

    target_include_directories(Pulse24Sync
        PRIVATE
            ${ALSA_INCLUDE_DIRS}
            ${JACK_INCLUDE_DIRS}
    )

    target_compile_options(Pulse24Sync
        PRIVATE
            ${ALSA_CFLAGS_OTHER}
            ${JACK_CFLAGS_OTHER}
    )
endif()
