[![Build Pulse24Sync](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/build.yml/badge.svg)](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/build.yml)
[![Tests](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/test-simple.yml/badge.svg)](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/test-simple.yml)
[![PR Check](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/pr-check.yml/badge.svg)](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/pr-check.yml)

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
- **Comprehensive Testing**: Full test suite with automated CI/CD

## Build Requirements

- **CMake** 3.15 or higher
- **C++17** compatible compiler
- **JUCE Framework** (will be downloaded automatically)

## Testing

Pulse24Sync includes a comprehensive test suite to ensure code quality and prevent regressions.

### Running Tests

```bash
# Run all tests
./run-tests.sh

# Run with verbose output
./run-tests.sh --verbose

# Run specific test categories
./run-tests.sh --category=Core

# Clean build and run in debug mode
./run-tests.sh --clean --debug
```

### Test Coverage

- **Core Logic**: PulseGenerator functionality, timing accuracy, MIDI generation
- **Plugin Interface**: Parameter management, state persistence, host integration  
- **Edge Cases**: Boundary conditions, error handling, performance validation
- **Multi-platform**: Automated testing on Ubuntu, macOS, and Windows

### Continuous Integration

The project uses GitHub Actions for automated testing:

- ✅ **Multi-platform testing** on every push and PR
- ✅ **Sanitizer testing** (AddressSanitizer, UndefinedBehaviorSanitizer)
- ✅ **Code coverage** reporting
- ✅ **Dependency caching** for faster builds
- ✅ **Automatic PR comments** with test results

For detailed testing information, see [docs/TESTING.md](docs/TESTING.md).

## Building the Plugin

### macOS

1. **Clone the repository and navigate to it**:
   ```bash
   git clone https://github.com/Skeyelab/Pulse24Sync.git
   cd Pulse24Sync
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

1. **Clone the repository and open Command Prompt or PowerShell**:
   ```cmd
   git clone https://github.com/Skeyelab/Pulse24Sync.git
   cd Pulse24Sync
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

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Skeyelab/Pulse24Sync.git
   cd Pulse24Sync
   ```

2. **Install dependencies** (Ubuntu/Debian):
   ```bash
   ./install-linux-deps.sh
   ```

3. **Build the plugin**:
   ```bash
   ./build.sh
   ```

For detailed Linux build instructions and other distributions, see [LINUX_BUILD_GUIDE.md](LINUX_BUILD_GUIDE.md).

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
- **Linux**: VST3 and Standalone

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
