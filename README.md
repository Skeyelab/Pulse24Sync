[![Build Pulse24Sync](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/build.yml/badge.svg)](https://github.com/Skeyelab/Pulse24Sync/actions/workflows/build.yml)

# Enhanced MIDI Sync VST Plugin

A professional VST3 plugin that provides comprehensive MIDI synchronization with configurable resolution, MIDI Machine Control (MMC), Song Position Pointer, and advanced transport features.

## Features

### **Core Synchronization**
- **Configurable PPQN Resolution**: Choose from 24, 48, 96, 192, or 480 pulses per quarter note
- **Sample-Accurate Timing**: High-precision synchronization locked to audio sample rate
- **Tempo Synchronization**: Automatically syncs to your DAW's tempo with real-time tempo changes
- **Manual Mode**: Set manual BPM when not syncing to host (60-200 BPM)

### **MIDI Machine Control (MMC)**
- **Full MMC Support**: Send Start, Stop, Continue, Reset, Pause, and Locate commands
- **Device ID Configuration**: Target specific devices or use broadcast mode (device ID 127)
- **Professional Transport Control**: Industry-standard transport messaging
- **Remote Control Capability**: Control external hardware and software via MMC

### **Advanced Timing Features**
- **Song Position Pointer (SPP)**: Precise song positioning for loop and section work
- **Clock When Stopped**: Optional clock output during stop state for tempo reference
- **Resync Functionality**: Manual resync on downbeat for live performance
- **Reset Commands**: Complete system reset capability

### **Output Modes**
- **MIDI Clock**: Standard MIDI timing clock messages (0xF8)
- **Note Pulses**: Traditional MIDI note-based pulse generation
- **Configurable Channel**: Select MIDI channel (1-16) for pulse output
- **Adjustable Velocity**: MIDI velocity control (0-127)

### **Professional Features**
- **Real-time Status Display**: Shows BPM, transport state, song position, and pulse rate
- **Visual Feedback**: Color-coded UI sections for easy navigation
- **Transport Controls**: Reset and Resync buttons for live use
- **VST3 Format**: Compatible with all major DAWs

## Enhanced Capabilities

### **PPQN Resolution Options**
- **24 PPQN**: Standard MIDI clock (Roland, most DAWs)
- **48 PPQN**: Korg standard (Volcas, SQ-1, many Korg devices)
- **96 PPQN**: Oberheim/Linn standard (DMX, LM-2)
- **192 PPQN**: Modern sequencer resolution
- **480 PPQN**: High-resolution DAW timing

### **MMC Command Set**
- **Start (0x02)**: Begin playback from current position
- **Stop (0x01)**: Stop playback
- **Continue (0x03)**: Resume from current position  
- **Reset (0x0D)**: Return to startup state
- **Locate (0x44)**: Jump to specific SMPTE position
- **Pause (0x09)**: Pause/resume functionality

### **Synchronization Standards**
- **Sample-Accurate**: ±1 sample jitter at 48kHz (±20μs precision)
- **MIDI Clock**: Standard 0xF8 timing messages
- **Song Position Pointer**: 0xF2 messages for precise positioning
- **Real-Time Messages**: Start (0xFA), Stop (0xFC), Continue (0xFB)

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

1. **Install dependencies** (Ubuntu/Debian):
   ```bash
   ./install-linux-deps.sh
   ```

2. **Build the plugin**:
   ```bash
   ./build.sh
   ```

For detailed Linux build instructions and other distributions, see [LINUX_BUILD_GUIDE.md](LINUX_BUILD_GUIDE.md).

## Installation

After building, the VST3 plugin will be automatically copied to the appropriate system location:

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

## Usage

### **Basic Setup**
1. **Load the plugin** in your DAW as a VST3 effect
2. **Enable the plugin** using the "Plugin Enabled" button
3. **Choose sync mode**:
   - **Sync to Host**: Automatically follows your DAW's tempo
   - **Manual Mode**: Set your own BPM using the Manual BPM slider

### **Configuring Clock Resolution**
1. **Select PPQN Resolution** from the dropdown:
   - 24 PPQN for standard MIDI gear
   - 48 PPQN for Korg devices
   - 96 PPQN for vintage gear (Oberheim, Linn)
   - 192/480 PPQN for high-resolution applications

### **MIDI Machine Control Setup**
1. **Enable MMC** using the "MIDI Machine Control Enabled" toggle
2. **Set Device ID** (0-126 for specific devices, 127 for all devices)
3. **Enable "Send MMC Commands"** to transmit transport control
4. **Configure receiving devices** to respond to MMC messages

### **Advanced Sync Options**
- **Send Clock When Stopped**: Maintains tempo reference during stop
- **Send Song Position Pointer**: Enables precise positioning
- **Use MIDI Clock**: Choose between MIDI clock or note pulses

### **Live Performance Features**
- **Reset Button**: Complete system reset to startup state
- **Resync Button**: Resynchronize all devices on next downbeat
- **Real-time Status**: Monitor transport state and song position

## Technical Details

- **Sample Rate**: Supports all common sample rates (44.1kHz, 48kHz, 96kHz, etc.)
- **Buffer Size**: Works with any buffer size
- **Tempo Range**: 60-200 BPM (manual mode)
- **MIDI Output**: Generates MIDI clock, MMC, and SPP messages
- **Precision**: Sample-accurate timing with ±1 sample jitter
- **Resolution**: 24-480 PPQN configurable

## Professional Applications

### **Studio Synchronization**
- Sync vintage drum machines and sequencers
- Control hardware from DAW transport
- Maintain precise timing across multiple devices

### **Live Performance**
- Real-time tempo changes with sample-accurate sync
- Manual resync for tempo-drifted devices
- Professional transport control via MMC

### **Vintage Gear Integration**
- Support for various PPQN standards
- DIN Sync compatibility via MIDI-to-DIN converters
- Vintage sequencer and drum machine timing

## Troubleshooting

### Plugin not appearing in DAW
- Ensure the plugin was built successfully
- Check that the VST3 file was copied to the correct location
- Restart your DAW after installation

### Timing issues
- Verify correct PPQN setting for your hardware
- Check MMC device ID settings
- Ensure "Sync to Host" is enabled for DAW sync

### MMC not working
- Verify MMC is enabled on receiving devices
- Check device ID configuration (127 = all devices)
- Ensure MIDI routing is correct

## Compatibility

### **Tested with vintage gear:**
- Roland TR-606, TR-707, TR-808, TR-909 (24 PPQN)
- Korg Volca series, SQ-1 (48 PPQN)
- Oberheim DMX, Linn LM-2 (96 PPQN)
- Modern DAWs and sequencers (192/480 PPQN)

### **DAW Compatibility:**
- Ableton Live
- Pro Tools
- Logic Pro
- Cubase/Nuendo
- Reaper
- Studio One

## License

This project is open source. Feel free to modify and distribute according to your needs.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
