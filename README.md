# Pulse24Sync

A JUCE-based VST/AU plugin that generates 24 pulses per quarter note and stays synchronized with tempo changes.

## Inspiration

This plugin was inspired by a Facebook post in the **Synthesizer Zone** group by **Zac Caruso**, who asked:
> "Anyone know of a plugin that can do 24 pulses per quarter note and stay in time with tempo changes? I'd actually pay someone to"

This request highlighted a specific need in the music production community for precise rhythmic pulse generation that can adapt to tempo changes in real-time.

## Use Cases

- **Rhythmic Modulation**: Create complex rhythmic patterns for synthesizer modulation
- **Clock Generation**: Provide precise timing signals for modular systems
- **Tempo-Synced Effects**: Generate tempo-locked trigger signals for effects processing
- **Sequencer Clocking**: Create high-resolution clock signals for external sequencers
- **Experimental Music**: Explore micro-rhythmic compositions with 24 divisions per beat

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
4. **Set pulse width**: Adjust the pulse width slider to control pulse duration
5. **Tempo sync**: The plugin automatically syncs to your DAW's tempo

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

## Technical Details

- **Framework**: JUCE 6.1.6
- **Plugin Formats**: VST3, AU (Audio Unit)
- **Platforms**: macOS, Windows
- **Architecture**: Universal Binary (arm64 + x86_64) on macOS

## Development

This project uses JUCE's Projucer for project management. To modify the project:

1. Open `Pulse24Sync.jucer` in Projucer
2. Make your changes
3. Save and regenerate project files
4. Build in your preferred IDE

## License

This project is open source and available under the MIT License.

## Acknowledgments

- **Zac Caruso** for the original idea and inspiration
- **Synthesizer Zone** Facebook group for fostering creative discussions
- **JUCE Framework** for providing the excellent audio plugin development platform