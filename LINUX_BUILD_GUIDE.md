# Linux Build Guide for Pulse24Sync VST Plugin

This guide covers building the Pulse24Sync VST plugin on Linux systems.

## Prerequisites

The Linux build requires several development libraries that are not typically installed by default. This guide covers Ubuntu/Debian systems, but similar packages are available for other distributions.

## Quick Setup (Ubuntu/Debian)

1. **Run the dependency installer:**
   ```bash
   ./install-linux-deps.sh
   ```

2. **Build the plugin:**
   ```bash
   ./build.sh
   ```

## Manual Installation of Dependencies

If you prefer to install dependencies manually or are using a different distribution:

### Essential Build Tools
```bash
sudo apt-get install -y build-essential cmake git pkg-config
```

### Audio Libraries
```bash
sudo apt-get install -y libasound2-dev libjack-jackd2-dev
```

### Graphics Libraries
```bash
sudo apt-get install -y libfreetype6-dev libgl1-mesa-dev
```

### Networking Libraries
```bash
sudo apt-get install -y libcurl4-openssl-dev
```

### X11 Libraries
```bash
sudo apt-get install -y libx11-dev libxinerama-dev libxext-dev libxrandr-dev libxcursor-dev libxcomposite-dev
```

### GUI Libraries
```bash
sudo apt-get install -y libgtk-3-dev
```

## For Other Linux Distributions

### Fedora/RHEL/CentOS
```bash
sudo dnf install -y gcc gcc-c++ cmake git pkgconfig alsa-lib-devel jack-audio-connection-kit-devel freetype-devel mesa-libGL-devel libcurl-devel libX11-devel libXinerama-devel libXext-devel libXrandr-devel libXcursor-devel libXcomposite-devel gtk3-devel
```

### Arch Linux
```bash
sudo pacman -S base-devel cmake git pkgconfig alsa-lib jack freetype2 mesa libcurl libx11 libxinerama libxext libxrandr libxcursor libxcomposite gtk3
```

## Build Process

1. **Clone and navigate to the project:**
   ```bash
   cd /path/to/Pulse24Sync
   ```

2. **Install dependencies (if not done already):**
   ```bash
   ./install-linux-deps.sh
   ```

3. **Build the plugin:**
   ```bash
   ./build.sh
   ```

## Build Output

After a successful build, you will find:

- **VST3 Plugin:** `~/.vst3/Pulse24Sync.vst3/` (automatically installed)
- **Standalone Application:** `build/Pulse24Sync_artefacts/Release/Standalone/Pulse24Sync`
- **Build Artifacts:** `build/Pulse24Sync_artefacts/Release/`

## Troubleshooting

### Common Issues

1. **Missing pkg-config packages:**
   ```
   Error: Package 'alsa' was not found
   ```
   **Solution:** Install the missing development packages using the commands above.

2. **CMake version too old:**
   ```
   Error: CMake 3.15 or higher is required
   ```
   **Solution:** Update CMake or install from CMake's official website.

3. **Compiler not found:**
   ```
   Error: No CMAKE_CXX_COMPILER could be found
   ```
   **Solution:** Install build-essential package.

### Verification Commands

Check if all dependencies are available:
```bash
pkg-config --exists alsa && echo "ALSA: ✓" || echo "ALSA: ✗"
pkg-config --exists jack && echo "JACK: ✓" || echo "JACK: ✗"
pkg-config --exists freetype2 && echo "FreeType2: ✓" || echo "FreeType2: ✗"
pkg-config --exists gl && echo "OpenGL: ✓" || echo "OpenGL: ✗"
pkg-config --exists libcurl && echo "cURL: ✓" || echo "cURL: ✗"
```

## Plugin Installation

The build script automatically installs the VST3 plugin to `~/.vst3/`, which is the standard location for user VST3 plugins on Linux. Most DAWs will automatically detect plugins in this location.

### Manual Installation

If you need to install to a different location:

```bash
# Copy to system-wide location (requires sudo)
sudo cp -r ~/.vst3/Pulse24Sync.vst3 /usr/local/lib/vst3/

# Or copy to a custom VST3 path
cp -r ~/.vst3/Pulse24Sync.vst3 /path/to/your/vst3/directory/
```

## Running the Standalone Application

```bash
cd build/Pulse24Sync_artefacts/Release/Standalone/
./Pulse24Sync
```

## Development Notes

- The plugin is built using JUCE 7.0.12
- Supports VST3 and Standalone formats
- Requires C++17 compiler
- Uses CMake build system
- All warnings during build are non-critical and don't affect functionality

## Clean Build

To perform a clean build:
```bash
rm -rf build
./build.sh
```

## Success! 🎉

Your Linux build environment is now properly configured and the Pulse24Sync VST plugin should build successfully!