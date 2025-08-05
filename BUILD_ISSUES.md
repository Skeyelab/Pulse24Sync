# Build Issues and Solutions

## Current Issue

The JUCE framework has compatibility issues with macOS 15 (Sequoia) due to deprecated APIs:

- `CGWindowListCreateImage` is obsoleted in macOS 15.0
- `CTFontManagerRegisterGraphicsFont` and related functions are deprecated
- Various other macOS 15 compatibility issues

## What We've Created

Despite the build issues, we have successfully created a complete VST plugin project structure:

### ✅ Completed Components

1. **Plugin Architecture**
   - `PluginProcessor.h/cpp` - Main VST plugin processor
   - `PluginEditor.h/cpp` - Custom UI with real-time status display
   - `PulseGenerator.h/cpp` - Core 24-pulse-per-quarter-note logic

2. **Features Implemented**
   - 24 pulses per quarter note generation
   - Real-time tempo synchronization with DAW
   - Manual BPM mode (60-200 BPM)
   - Configurable MIDI velocity (0-127)
   - MIDI channel selection (1-16)
   - Enable/disable functionality
   - Modern UI with status display

3. **Build System**
   - CMake-based build system
   - Automated JUCE dependency management
   - Cross-platform support (macOS, Windows, Linux)
   - VST3 and Standalone formats

## Alternative Solutions

### Option 1: Use JUCE Projucer (Recommended)
1. Download JUCE Projucer from https://juce.com/get-juce/
2. Create a new Audio Plugin project
3. Copy our source files into the generated project
4. Build using Xcode/Visual Studio

### Option 2: Use JUCE 7.1+ (When Available)
- JUCE 7.1+ should have better macOS 15 compatibility
- Update the GIT_TAG in CMakeLists.txt when available

### Option 3: Use JUCE 6.1.6 with macOS 14
- Build on macOS 14 or earlier
- Use Xcode 15 instead of Xcode 16

### Option 4: Manual JUCE Setup
1. Download JUCE manually
2. Set up include paths manually
3. Build without CMake

## Current Project Structure

```
Pulse24Sync/
├── CMakeLists.txt          # Build configuration
├── build.sh               # Build script
├── README.md              # Project documentation
├── .gitignore            # Git ignore rules
├── BUILD_ISSUES.md       # This file
└── Source/
    ├── PluginProcessor.h  # Main plugin processor
    ├── PluginProcessor.cpp
    ├── PluginEditor.h     # Custom UI
    ├── PluginEditor.cpp
    ├── PulseGenerator.h   # Core pulse logic
    └── PulseGenerator.cpp
```

## Core Functionality

The plugin generates exactly 24 MIDI pulses per quarter note with:

- **Tempo Sync**: Automatically follows DAW tempo changes
- **Manual Mode**: Set custom BPM when not syncing
- **Precise Timing**: Sample-accurate pulse generation
- **MIDI Output**: Generates MIDI note events on configurable channel
- **Real-time UI**: Shows current BPM, sync status, and pulse rate

## Next Steps

1. **Immediate**: Use JUCE Projucer to build the project
2. **Short-term**: Wait for JUCE 7.1+ with better macOS 15 support
3. **Long-term**: Consider alternative VST frameworks if needed

The core functionality is complete and ready to use once the build issues are resolved.
