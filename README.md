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

- **JUCE Framework** (will be downloaded automatically)
- **Projucer** (for project generation)
- **Platform-specific build tools**:
  - **macOS**: Xcode with command line tools
  - **Windows**: Visual Studio 2022 with C++ workload
  - **Linux**: GCC/Clang, Make, and audio development libraries

## Building the Plugin

### macOS

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Skeyelab/Pulse24Sync.git
   cd Pulse24Sync
   ```

2. **Build the plugin**:
   ```bash
   ./build_macos.sh
   ```

3. **Install the plugin**:
   ```bash
   # Copy from the dist folder to your plugin directory
   cp -R dist/Pulse24Sync.vst3 ~/Library/Audio/Plug-Ins/VST3/
   cp -R dist/Pulse24Sync.component ~/Library/Audio/Plug-Ins/Components/
   ```

### Windows

1. **Clone the repository**:
   ```cmd
   git clone https://github.com/Skeyelab/Pulse24Sync.git
   cd Pulse24Sync
   ```

2. **Build the plugin**:
   ```cmd
   build_windows.bat
   ```

3. **Install the plugin**:
   ```cmd
   # Copy from the dist folder to your plugin directory
   xcopy "dist\Pulse24Sync.vst3" "C:\Program Files\Common Files\VST3\" /E /I /Y
   ```

### Linux

1. **Install dependencies** (Ubuntu/Debian):
   ```bash
   sudo apt-get install build-essential cmake git libasound2-dev libjack-jackd2-dev libfreetype6-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev libxrender-dev libxfixes-dev libxss-dev libgl1-mesa-dev libglu1-mesa-dev libwebkit2gtk-4.0-dev
   ```

2. **Build the plugin**:
   ```bash
   ./build.sh
   ```

3. **Install the plugin**:
   ```bash
   # Copy from the dist folder to your plugin directory
   cp -R dist/Pulse24Sync.vst3 ~/.vst3/
   ```

For detailed Linux build instructions, see [LINUX_BUILD_GUIDE.md](LINUX_BUILD_GUIDE.md).

## Distribution Scripts

If you have existing build artifacts and want to create a clean distribution:

### Windows
```cmd
create-windows-distribution.bat
```

### macOS
```bash
./create-macos-distribution.sh
```

These scripts extract only the final plugin files (excluding development artifacts) into a `dist/` folder.

## Installation

After building, manually copy the plugin files from the `dist/` folder to the appropriate system location:

- **macOS**:
  - VST3: `~/Library/Audio/Plug-Ins/VST3/`
  - AU: `~/Library/Audio/Plug-Ins/Components/`
- **Windows**: `C:\Program Files\Common Files\VST3\`
- **Linux**: `~/.vst3/`

Then restart your DAW and scan for new plugins.

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
- Check that the plugin files were copied to the correct location
- Restart your DAW after installation

### Tempo sync issues
- Make sure "Sync to Host" is enabled
- Verify your DAW is sending tempo information
- Check that the DAW is playing (plugin only generates pulses during playback)

### Build errors
- Ensure you have the required build tools installed
- Check that Projucer is available at the expected location
- Verify internet connection (JUCE will be downloaded automatically)

## License

This project is open source. Feel free to modify and distribute according to your needs.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
