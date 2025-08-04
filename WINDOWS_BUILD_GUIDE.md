# Windows VST Build Guide for Pulse24Sync

This guide will help you build the Windows VST3 version of the Pulse24Sync plugin.

## Prerequisites

### Required Software
1. **Visual Studio 2022** (Community Edition is free)
   - Download from: https://visualstudio.microsoft.com/downloads/
   - Install with "Desktop development with C++" workload
   - Make sure to include Windows 10/11 SDK

2. **JUCE Framework**
   - Download from: https://juce.com/get-juce/
   - Extract to a known location (e.g., `C:\JUCE`)

3. **VST3 SDK** (optional, but recommended)
   - Download from: https://developer.steinberg.help/display/VST/VST+3+SDK
   - Extract to `C:\JUCE\modules\juce_audio_processors\format_types\VST3_SDK`

## Building the Plugin

### Method 1: Using Visual Studio 2022

1. **Open the Solution**
   - Launch Visual Studio 2022
   - Navigate to: Builds/VisualStudio2022/Pulse24Sync.sln
   - Open the solution file

2. **Configure the Build**
   - Set the solution configuration to "Release"
   - Set the platform to "x64"
   - Right-click on **Pulse24Sync_VST3** in Solution Explorer
   - Select "Set as Startup Project"

3. **Build the Plugin**
   - Press Ctrl+Shift+B or go to Build → Build Solution
   - The VST3 plugin will be built to: `Builds/VisualStudio2022/build/Release/Pulse24Sync.vst3`

### Method 2: Using Command Line

1. **Open Developer Command Prompt**
   - Start → Visual Studio 2022 → Developer Command Prompt for VS 2022

2. **Navigate to Project Directory**
   ```cmd
   cd C:\path\to\your\Pulse24\Pulse24Sync
   ```

3. **Build the Plugin**
   ```cmd
   msbuild Builds\VisualStudio2022\Pulse24Sync.sln /p:Configuration=Release /p:Platform=x64
   ```

### Method 3: Using the Automated Script

1. **Run the Build Script**
   ```cmd
   build_windows.bat
   ```

2. **Follow the Prompts**
   - The script will automatically build both VST3 and standalone versions
   - It will display the build results and installation instructions

## Installation

### VST3 Plugin Installation

1. **Copy the Plugin**
   From: Builds/VisualStudio2022/build/Release/Pulse24Sync.vst3
   To: C:\Program Files\Common Files\VST3\Pulse24Sync.vst3

2. **Alternative Installation Paths**
   - C:\Program Files\VST3\Pulse24Sync.vst3
   - C:\Program Files (x86)\VST3\Pulse24Sync.vst3

### Standalone Application

The standalone application can be run directly from:
 Builds/VisualStudio2022/build/Release/Pulse24Sync.exe

## Testing the Plugin

1. **Load in your DAW**
   - Open your DAW (Ableton Live, FL Studio, Reaper, etc.)
   - Scan for new plugins
   - Insert the "Pulse24Sync" plugin on a track

2. **Test Functionality**
   - Enable the plugin using the "Enabled" toggle
   - Adjust volume and pulse width parameters
   - Verify tempo synchronization works

## Troubleshooting

### Common Issues

1. **Build Errors**
   - Ensure Visual Studio 2022 is installed with C++ workload
   - Check that JUCE paths are correctly configured
   - Verify Windows SDK is installed

2. **Plugin Not Found**
   - Make sure the plugin is copied to the correct VST3 directory
   - Restart your DAW after installation
   - Check DAW's plugin scanning settings

3. **Runtime Errors**
   - Ensure all required Visual C++ redistributables are installed
   - Check Windows compatibility settings

## Project Structure

```
Builds/VisualStudio2022/
├── Pulse24Sync.sln                    # Visual Studio solution
├── Pulse24Sync_VST3.vcxproj          # VST3 plugin project
├── Pulse24Sync_StandalonePlugin.vcxproj # Standalone app project
├── Pulse24Sync_SharedCode.vcxproj    # Shared code project
└── build/Release/                   # Build output directory
    ├── Pulse24Sync.vst3/             # VST3 plugin bundle
    └── Pulse24Sync.exe               # Standalone executable
```

## Updating the Project

If you modify the JUCE project file:

1. **Regenerate Project Files**
   ```cmd
   /path/to/Projucer --resave Pulse24Sync.jucer
   ```

2. **Rebuild the Solution**
   - Open the updated solution in Visual Studio
   - Build the project again

## Support

For issues or questions:
- Check the JUCE documentation: https://juce.com/learn/
- Visit the VST3 SDK documentation: https://developer.steinberg.help/
- Review the project README for additional information