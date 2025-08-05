# Windows VST Build Guide for Pulse24Sync

This guide will help you build the Windows VST3 version of the Pulse24Sync plugin, both locally and in the cloud.

## 🚀 Quick Start - Cloud Build (Recommended)

The easiest way to build for Windows is using our automated cloud builds:

### GitHub Actions (Free)
1. **Push to GitHub** - Your code is automatically built for both macOS and Windows
2. **Download artifacts** - Get the built plugins from the Actions tab
3. **No setup required** - Works on any platform

### Trigger a Cloud Build
```bash
# Push to main branch to trigger builds
git push origin main

# Or create a release tag
git tag v1.0.0
git push origin v1.0.0
```

## 🖥️ Local Windows Build

### Prerequisites

#### Required Software
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

### Building the Plugin

#### Method 1: Using the Automated Script (Easiest)

```bash
# Simply run the batch file
build_windows.bat
```

This script will:
- ✅ Check for Visual Studio installation
- ✅ Build VST3 plugin
- ✅ Build standalone application
- ✅ Show installation instructions

#### Method 2: Using Visual Studio 2022

1. **Open the Solution**
   ```
   Navigate to: Builds/VisualStudio2022/Pulse24Sync.sln
   Double-click to open in Visual Studio 2022
   ```

2. **Configure JUCE Paths**
   - In Visual Studio, go to **Project Properties**
   - Set the JUCE modules path to point to your JUCE installation
   - Update the module paths in the project settings if needed

3. **Select Build Configuration**
   - Choose **Release** configuration for production build
   - Choose **x64** platform (recommended for modern systems)

4. **Build the VST3 Plugin**
   - Right-click on **Pulse24Sync_VST3** in Solution Explorer
   - Select **Build**
   - The VST3 plugin will be built to: `Builds/VisualStudio2022/build/Release/Pulse24Sync.vst3`

#### Method 3: Using Command Line

1. **Open Developer Command Prompt**
   ```
   Start → Visual Studio 2022 → Developer Command Prompt for VS 2022
   ```

2. **Navigate to Project Directory**
   ```cmd
   cd C:\path\to\your\Pulse24\Pulse24Sync
   ```

3. **Build Using MSBuild**
   ```cmd
   msbuild Builds\VisualStudio2022\Pulse24Sync.sln /p:Configuration=Release /p:Platform=x64
   ```

## 📦 Installation

### VST3 Plugin Installation

1. **Copy the VST3 Plugin**
   ```
   From: Builds/VisualStudio2022/build/Release/Pulse24Sync.vst3
   To: C:\Program Files\Common Files\VST3\Pulse24Sync.vst3
   ```

2. **Alternative Locations** (if the above doesn't work):
   ```
   C:\Program Files\VST3\Pulse24Sync.vst3
   C:\Program Files (x86)\VST3\Pulse24Sync.vst3
   ```

### Standalone Application

The standalone version will be built to:
```
Builds/VisualStudio2022/build/Release/Pulse24Sync.exe
```

## ☁️ Cloud Build Options

### 1. GitHub Actions (Free)
- **Pros**: Free, automated, integrates with GitHub
- **Cons**: Limited build time, requires GitHub repository
- **Best for**: Open source projects, regular releases

### 2. Azure DevOps
- **Pros**: Microsoft ecosystem, good Windows support
- **Cons**: Requires Azure account
- **Best for**: Enterprise projects, Microsoft shops

### 3. Jenkins
- **Pros**: Self-hosted, highly customizable
- **Cons**: Requires server setup and maintenance
- **Best for**: Large teams, custom build requirements

### 4. GitHub Codespaces
- **Pros**: Full development environment in browser
- **Cons**: Requires GitHub Pro/Team account
- **Best for**: Development and testing

## 🔧 Testing the Plugin

### Supported DAWs
- **Ableton Live** (10.1+)
- **FL Studio** (20+)
- **Reaper** (5.0+)
- **Cubase** (10+)
- **Studio One** (4+)
- **Bitwig Studio** (3+)

### Testing Steps
1. Open your DAW
2. Scan for new plugins (if required)
3. Insert the "Pulse24Sync" plugin on a track
4. Enable the plugin using the "Enabled" toggle
5. Adjust volume and pulse width as needed
6. Play your project - you should hear 24 pulses per quarter note

## 🛠️ Troubleshooting

### Common Issues

1. **Plugin Not Found**
   - Ensure the VST3 file is in the correct directory
   - Restart your DAW after installation
   - Check DAW's plugin scanning settings

2. **Build Errors**
   - Verify Visual Studio 2022 is properly installed
   - Check that JUCE paths are correctly set
   - Ensure Windows SDK is installed

3. **Runtime Errors**
   - Make sure you're using the Release build
   - Check that all required Visual C++ redistributables are installed

4. **Cloud Build Failures**
   - Check GitHub Actions logs for specific errors
   - Ensure JUCE version compatibility
   - Verify project file paths are correct

### Build Configuration

The project includes these build targets:
- **Pulse24Sync_VST3**: VST3 plugin (main target)
- **Pulse24Sync_StandalonePlugin**: Standalone application
- **Pulse24Sync_SharedCode**: Shared code library

## 📁 File Structure

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

## 🔄 Cross-Platform Development

If you're developing on macOS and want to build for Windows:

1. **Use the same source code** - the C++ code is platform-independent
2. **Regenerate project files** when making changes:
   ```bash
   # On macOS
   /path/to/Projucer --resave Pulse24Sync.jucer
   ```
3. **Transfer the Builds/VisualStudio2022 folder** to a Windows machine
4. **Build on Windows** using the steps above
5. **Or use cloud builds** - just push to GitHub!

## ⚡ Performance Notes

- The plugin is optimized for real-time performance
- VST3 format provides better performance than VST2
- The plugin automatically adapts to different sample rates
- Tempo synchronization is sample-accurate

## 🆘 Support

For issues specific to the Windows build:
1. Check the Visual Studio Output window for build errors
2. Verify all prerequisites are installed correctly
3. Ensure you're using compatible versions of Visual Studio and JUCE
4. For cloud builds, check GitHub Actions logs
5. Create an issue on GitHub for persistent problems

## 🎯 Next Steps

1. **Set up cloud builds** - Push your code to GitHub to enable automated builds
2. **Test on multiple DAWs** - Verify compatibility with your target DAWs
3. **Create installer** - Consider using tools like Inno Setup for distribution
4. **Code signing** - Sign your plugin for better compatibility and trust
