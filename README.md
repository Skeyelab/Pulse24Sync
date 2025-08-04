# Pulse24Sync

A JUCE-based VST/AU plugin that generates 24 pulses per quarter note and stays synchronized with tempo changes.

## Features

- **Tempo Synchronization**: Automatically syncs to the host's tempo and stays in time with tempo changes
- **24 Pulses per Quarter Note**: Generates exactly 24 evenly spaced pulses for every quarter note
- **Adjustable Parameters**:
  - **Enabled/Disabled**: Toggle the pulse generation on/off
  - **Volume**: Control the amplitude of the pulses (0.0 to 1.0)
  - **Pulse Width**: Adjust the duration of each pulse (1% to 50% of pulse period)

## Installation

The plugin has been built and installed to:
- **AU Plugin**: `/Users/edahl/Library/Audio/Plug-Ins/Components/Pulse24Sync.component`
- **Standalone App**: `Builds/MacOSX/build/Release/Pulse24Sync.app`

## Usage

1. **Load in your DAW**: The plugin will appear as "Pulse24Sync" in your plugin list
2. **Enable the plugin**: Use the "Enabled" toggle to start/stop pulse generation
3. **Adjust volume**: Use the volume slider to control pulse amplitude
4. **Set pulse width**: Use the pulse width slider to control how long each pulse lasts

## Technical Details

- **Sample Rate**: Automatically adapts to the host's sample rate
- **Tempo Detection**: Uses JUCE's AudioPlayHead to get real-time tempo information
- **Precise Timing**: Calculates pulse timing based on samples per quarter note divided by 24
- **Real-time Updates**: Responds to tempo changes during playback

## Building from Source

### macOS
1. Open the project in Xcode: `Builds/MacOSX/Pulse24Sync.xcodeproj`
2. Select the target you want to build (AU Plugin or Standalone)
3. Build the project (⌘+B)

### Windows
1. Open the solution in Visual Studio 2022: `Builds/VisualStudio2022/Pulse24Sync.sln`
2. Select the "Pulse24Sync_VST3" project
3. Build the solution (Ctrl+Shift+B)
4. Or use the automated build script: `build_windows.bat`

For detailed Windows build instructions, see [WINDOWS_BUILD_GUIDE.md](WINDOWS_BUILD_GUIDE.md).

## Requirements

- macOS 10.13 or later
- JUCE framework
- Xcode 12.0 or later

## License

This project uses the JUCE framework. Please refer to JUCE's licensing terms.
