[![Build Pulse24Sync](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/build.yml/badge.svg)](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/build.yml)

# Pulse24Sync VST Plugin

A VST3 plugin that generates precisely 24 MIDI pulses per quarter note, with real-time tempo synchronization to your DAW.

## Features

- **24 Pulses per Quarter Note**: Generates exactly 24 MIDI note events per quarter note
- **Tempo Synchronization**: Automatically syncs to your DAW's tempo and stays in time during tempo changes
- **Manual Mode**: Option to set a manual BPM when not syncing to host
- **Configurable Parameters**:
  - Enable/Disable pulse generation
  - Adjust MIDI velocity (0-127)
  - Select MIDI channel (1-16)
  - Toggle host tempo synchronization
  - Manual BPM setting (60-200 BPM)
- **Real-time Status Display**: Shows current BPM, sync status, and pulse rate
- **VST3 Format**: Compatible with all major DAWs

## Build Requirements

- **CMake** 3.15 or higher
- **C++17** compatible compiler
- **JUCE Framework** (will be downloaded automatically)

## Building the Plugin

### macOS

1. **Clone the repository and navigate to it**:
   ```bash
   cd /Users/edahl/Documents/GitHub/Pulse24Sync
   ```

2. **Create a build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the plugin**:
   ```bash
   make -j$(nproc)
   ```

### Windows

1. **Open Command Prompt or PowerShell**:
   ```cmd
   cd C:\path\to\Pulse24Sync
   ```

2. **Create build directory**:
   ```cmd
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```cmd
   cmake .. -G "Visual Studio 16 2019" -A x64
   ```

4. **Build the plugin**:
   ```cmd
   cmake --build . --config Release
   ```

### Linux

1. **Navigate to the project directory**:
   ```bash
   cd /path/to/Pulse24Sync
   ```

2. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the plugin**:
   ```bash
   make -j$(nproc)
   ```

## Installation

After building, the VST3 plugin will be automatically copied to the appropriate system location:

- **macOS**: `~/Library/Audio/Plug-Ins/VST3/Pulse24Sync.vst3`

## Creating Releases

### Automatic Release Creation

This project includes an automated release system that builds both macOS and Windows versions and creates a GitHub release with all plugin formats.

#### To create a release:

1. **Make sure all changes are committed**:
   ```bash
   git add .
   git commit -m "Your commit message"
   git push origin main
   ```

2. **Run the release script**:
   ```bash
   ./create-release.sh
   ```

3. **Follow the prompts** to enter the version number (e.g., `1.0.0`, `1.2.3-beta.1`)

4. **The script will**:
   - Create a git tag (e.g., `v1.0.0`)
   - Push the tag to GitHub
   - Trigger the automated build and release workflow

#### What gets built automatically:

- **macOS**: VST3, Audio Unit (.component), and Standalone (.app)
- **Windows**: VST3 and Standalone (.exe)

#### Release workflow:

1. GitHub Actions builds both platforms in parallel
2. All plugin formats are packaged
3. A GitHub release is created with:
   - Release notes
   - Download links for all platforms
   - Proper versioning and tagging

#### Manual release creation:

If you prefer to create releases manually:
1. Go to [GitHub Releases](https://github.com/your-repo/releases)
2. Click "Create a new release"
3. Set the tag version (e.g., `v1.0.0`)
4. Upload the built plugin files manually
- **Windows**: `C:\Program Files\Common Files\VST3\Pulse24Sync.vst3`
- **Linux**: `~/.vst3/Pulse24Sync.vst3`

## Usage

1. **Load the plugin** in your DAW as a VST3 effect
2. **Enable the plugin** using the "Enabled" button
3. **Choose sync mode**:
   - **Sync to Host**: Automatically follows your DAW's tempo
   - **Manual Mode**: Set your own BPM using the Manual BPM slider
4. **Configure MIDI settings**:
   - Set the MIDI velocity for the pulses (0-127)
   - Choose the MIDI channel (1-16)
5. **Start playback** in your DAW - the plugin will generate 24 MIDI pulses per quarter note

## Technical Details

- **Sample Rate**: Supports all common sample rates (44.1kHz, 48kHz, 96kHz, etc.)
- **Buffer Size**: Works with any buffer size
- **Tempo Range**: 60-200 BPM (manual mode)
- **MIDI Output**: Generates MIDI note on/off events on the selected channel
- **Precision**: High-precision timing using sample-accurate calculations

## Troubleshooting

### Plugin not appearing in DAW
- Ensure the plugin was built successfully
- Check that the VST3 file was copied to the correct location
- Restart your DAW after installation

### Tempo sync issues
- Make sure "Sync to Host" is enabled
- Verify your DAW is sending tempo information
- Check that the DAW is playing (plugin only generates pulses during playback)

### Build errors
- Ensure you have CMake 3.15+ installed
- Check that your compiler supports C++17
- Verify internet connection (JUCE will be downloaded automatically)

## License

This project is open source. Feel free to modify and distribute according to your needs.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
