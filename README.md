[![Build Pulse24Sync](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/build.yml/badge.svg)](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/build.yml)

# Pulse24Sync

A JUCE-based audio plugin that generates a 24 PPQN pulse train for tempo sync testing. Ships as VST3/AU/Standalone.

## Features
- 24 PPQN pulse generation, audible 1 kHz tone per pulse
- Host tempo sync with resilient re-sync on transport jumps
- Manual BPM mode when host sync is disabled
- Adjustable pulse width (1–50 ms) and velocity (0–127)

## Dev Docs
- See `ARCHITECTURE.md` for an overview of components, parameters, and audio flow.
- See platform build guides for signing and packaging:
  - `MACOS_CODE_SIGNING_GUIDE.md`
  - `WINDOWS_BUILD_GUIDE.md`
  - `LINUX_BUILD_GUIDE.md`

## Building
- Cross-platform build scripts are in the repo root.
- CMake project file: `CMakeLists.txt`

## Installation

### For Released Binaries

Download the latest release from [GitHub Releases](https://github.com/Skeyelab/Pulse24Sync/releases):

#### macOS
1. Download the appropriate package:
   - `Pulse24Sync-macOS-vst3.zip` for VST3 plugin
   - `Pulse24Sync-macOS-au.zip` for Audio Unit plugin
   - `Pulse24Sync-macOS-standalone.zip` for standalone application
2. Extract the archive
3. Copy the plugin files to:
   - **VST3**: `~/Library/Audio/Plug-Ins/VST3/` or `/Library/Audio/Plug-Ins/VST3/`
   - **Audio Unit**: `~/Library/Audio/Plug-Ins/Components/` or `/Library/Audio/Plug-Ins/Components/`
   - **Standalone**: Applications folder or anywhere you prefer

#### Windows
1. Download the appropriate package:
   - `Pulse24Sync-Windows-vst3.zip` for VST3 plugin
   - `Pulse24Sync-Windows-standalone.zip` for standalone application
2. Extract the archive
3. Copy the plugin files to:
   - **VST3**: `C:\Program Files\Common Files\VST3\`
   - **Standalone**: Anywhere you prefer

#### Linux
1. Download the appropriate package:
   - `Pulse24Sync-Linux-vst3.zip` for VST3 plugin
   - `Pulse24Sync-Linux-standalone.zip` for standalone application
2. Extract the archive
3. Copy the plugin files to:
   - **VST3**: `~/.vst3/` (create the directory if it doesn't exist)
   - **Standalone**: Anywhere you prefer (make executable with `chmod +x Pulse24Sync`)

Alternatively, you can build from source using the instructions below.

### After Building from Source

When building from source, the plugin will be automatically copied to the appropriate system location:

- **macOS**: `~/Library/Audio/Plug-Ins/VST3/Pulse24Sync.vst3`
- **Windows**: `C:\Program Files\Common Files\VST3\Pulse24Sync.vst3`
- **Linux**: `~/.vst3/Pulse24Sync.vst3`

## Creating Releases

### Automatic Release Creation

This project includes an automated release system that builds macOS, Windows, and Linux versions and creates a GitHub release with all plugin formats.

### Code Signing and Notarization

The build system automatically code signs and notarizes macOS plugins to eliminate security warnings. For setup instructions:

- **General setup**: See [MACOS_CODE_SIGNING_GUIDE.md](MACOS_CODE_SIGNING_GUIDE.md)
- **GitHub Actions setup**: See [GITHUB_ACTIONS_CODESIGNING.md](GITHUB_ACTIONS_CODESIGNING.md)
- **Self-hosted runners**: See [SELF_HOSTED_RUNNER_SETUP.md](SELF_HOSTED_RUNNER_SETUP.md)
- **Notarization setup**: See [NOTARIZATION_SETUP.md](NOTARIZATION_SETUP.md)

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

3. **Follow the prompts** to enter the version number (e.g., `1.0.0`, `1.2.3-beta.1`). The release pipeline packages macOS (VST3/AU/Standalone), Windows (VST3/Standalone), and Linux (VST3/Standalone).

4. **The script will**:
   - Create a git tag (e.g., `v1.0.0`)
   - Push the tag to GitHub
   - Trigger the automated build and release workflow

#### What gets built automatically:

- **macOS**: VST3, Audio Unit (.component), and Standalone (.app)
- **Windows**: VST3 and Standalone (.exe)
- **Linux**: VST3 and Standalone (included in release packages)

#### Release workflow:

1. GitHub Actions builds both platforms in parallel
2. All plugin formats are packaged
3. A GitHub release is created with:
   - Release notes
   - Download links for all platforms
   - Proper versioning and tagging

#### Manual release creation:

If you prefer to create releases manually:
1. Go to [GitHub Releases](https://github.com/Skeyelab/Pulse24Sync/releases)
2. Click "Create a new release"
3. Set the tag version (e.g., `v1.0.0`)
4. Upload the built plugin files manually

## Usage

1. **Load the plugin** in your DAW as a VST3 effect
2. **Enable the plugin** using the "Enabled" button
3. **Choose sync mode**:
   - **Sync to Host**: Automatically follows your DAW's tempo
   - **Manual Mode**: Set your own BPM using the Manual BPM slider
4. **Configure audio settings**:
   - Set the pulse volume (0-127)
5. **Start playback** in your DAW - the plugin will generate 24 audio pulses per quarter note

## Technical Details

- **Sample Rate**: Supports all common sample rates (44.1kHz, 48kHz, 96kHz, etc.)
- **Buffer Size**: Works with any buffer size
- **Tempo Range**: 60–200 BPM (manual mode)
- **Audio Output**: Generates 1 kHz sine wave pulses (no MIDI output)
- **Pulse Width**: Adjustable 1–50 ms (default ~22 ms)
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
