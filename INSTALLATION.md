# Pulse24Sync Installation Guide

Thank you for downloading Pulse24Sync! This guide will help you install the plugin on your system.

## About Pulse24Sync

Pulse24Sync is an audio plugin that generates precisely 24 audio pulses per quarter note, with real-time tempo synchronization to your DAW. It's ideal for timing/tempo sync testing and clocking other audio-driven gear.

## System Requirements

- **macOS**: 10.15 (Catalina) or higher
- **Windows**: Windows 10/11 (64-bit)
- **Linux**: Ubuntu 20.04+ or equivalent distribution
- **DAW**: Any VST3-compatible DAW (e.g., Ableton Live, Cubase, Reaper, Bitwig, FL Studio, Studio One). On macOS, an AU build is also provided for Logic Pro and GarageBand.

## Installation Instructions

### macOS Installation

#### Option 1: Using the Downloaded Packages

1. **Download the appropriate package**:
   - `Pulse24Sync-macOS-vst3.zip` - For VST3 plugin
   - `Pulse24Sync-macOS-au.zip` - For Audio Unit plugin (Logic Pro, GarageBand)
   - `Pulse24Sync-macOS-standalone.zip` - For standalone application

2. **Extract the package** by double-clicking the downloaded .zip file

3. **Install the plugin**:
   
   **For VST3 Plugin**:
   - Open Finder and navigate to your home folder
   - Go to `Library/Audio/Plug-Ins/VST3/` (create the folder if it doesn't exist)
   - Copy `Pulse24Sync.vst3` to this folder
   
   **For Audio Unit Plugin**:
   - Go to `Library/Audio/Plug-Ins/Components/`
   - Copy `Pulse24Sync.component` to this folder
   
   **For Standalone Application**:
   - Copy `Pulse24Sync.app` to your Applications folder or anywhere you prefer

4. **Restart your DAW** to detect the new plugin

#### Option 2: System-wide Installation (Admin Required)

For system-wide installation (all users):
- **VST3**: Copy to `/Library/Audio/Plug-Ins/VST3/`
- **Audio Unit**: Copy to `/Library/Audio/Plug-Ins/Components/`

### Windows Installation

1. **Download the appropriate package**:
   - `Pulse24Sync-Windows-vst3.zip` - For VST3 plugin
   - `Pulse24Sync-Windows-standalone.zip` - For standalone application

2. **Extract the package** by right-clicking and selecting "Extract All"

3. **Install the plugin**:
   
   **For VST3 Plugin**:
   - Open Windows Explorer
   - Navigate to `C:\Program Files\Common Files\VST3\`
   - Copy the `Pulse24Sync.vst3` folder to this location
   - You may need administrator privileges
   
   **For Standalone Application**:
   - Copy `Pulse24Sync.exe` to any location you prefer
   - Create a desktop shortcut if desired

4. **Restart your DAW** to detect the new plugin

#### Alternative VST3 Locations

Some DAWs may look for VST3 plugins in other locations:
- `C:\Program Files\Steinberg\VST3\`
- Check your DAW's plugin scan paths in preferences

### Linux Installation

1. **Download the appropriate package**:
   - `Pulse24Sync-Linux-vst3.zip` - For VST3 plugin
   - `Pulse24Sync-Linux-standalone.zip` - For standalone application

2. **Extract the package** using your file manager or command line:
   ```bash
   unzip Pulse24Sync-Linux-vst3.zip
   # or
   unzip Pulse24Sync-Linux-standalone.zip
   ```

3. **Install the plugin**:
   
   **For VST3 Plugin**:
   ```bash
   # Create VST3 directory if it doesn't exist
   mkdir -p ~/.vst3
   
   # Copy the plugin
   cp -r Pulse24Sync.vst3 ~/.vst3/
   ```
   
   **For Standalone Application**:
   ```bash
   # Copy to a directory in your PATH or create a local directory
   cp Pulse24Sync ~/bin/  # or any preferred location
   
   # Make executable
   chmod +x ~/bin/Pulse24Sync
   ```

4. **Restart your DAW** to detect the new plugin

#### Alternative Installation Methods

**System-wide Installation (requires sudo)**:
- **VST3**: Copy to `/usr/lib/vst3/` or `/usr/local/lib/vst3/`

**For specific DAWs**:
- Some Linux DAWs may use different VST3 paths
- Check your DAW's plugin directory settings
- Common additional paths: `/usr/share/vst3/`, `~/.local/share/vst3/`

**Building from Source**:
If you prefer to build from source, see the build instructions in the main README.md file.

## Verification

### In Your DAW

1. **Open your DAW**
2. **Scan for new plugins** (usually in Preferences/Settings)
3. **Look for "Pulse24Sync"** in your plugin list under:
   - Effects/MIDI Effects
   - Instruments (some DAWs categorize MIDI generators here)
   - Utilities

### Standalone Application

1. **Launch the standalone app**
2. **You should see the Pulse24Sync interface**
3. **Configure your audio/MIDI settings** in the app's settings

## Usage Quick Start

1. **Load Pulse24Sync** as a VST3 effect in your DAW
2. **Enable the plugin** using the "Enabled" button
3. **Choose sync mode**:
   - **Sync to Host**: Automatically follows your DAW's tempo (recommended)
   - **Manual Mode**: Set your own BPM using the Manual BPM slider
4. **Configure audio settings**:
   - Set pulse velocity (0–127) and pulse width (1–50 ms)

5. **Start playback** - the plugin generates 24 audio pulses per quarter note

## Troubleshooting

### Plugin Not Showing Up

**macOS**:
- Verify the plugin is in the correct folder
- Check folder permissions (should be readable)
- Some DAWs have security restrictions - allow the plugin in System Preferences > Security

**Windows**:
- Run your DAW as administrator
- Check if Windows Defender blocked the plugin
- Verify the plugin is in the correct VST3 folder

**All Platforms**:
- Force a plugin rescan in your DAW
- Check your DAW's plugin blacklist
- Restart your DAW completely

### Audio Issues

- Ensure your DAW is sending tempo information to the plugin
- Ensure the plugin is inserted on an audio-capable track or as an effect


### Performance Issues

- The plugin is very lightweight and should not cause performance issues
- If experiencing problems, try increasing your DAW's buffer size

## Support

- **Issues/Bugs**: [GitHub Issues](https://github.com/Skeyelab/Pulse24Sync/issues)
- **Documentation**: [GitHub Repository](https://github.com/Skeyelab/Pulse24Sync)
- **Email**: contact@ericdahl.dev

## License

This software is open source. See the README for licensing details.

---

**Thank you for using Pulse24Sync!** 🎵