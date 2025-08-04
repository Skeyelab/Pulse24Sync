#!/bin/bash

# Pulse24Sync Release Helper Script
# Usage: ./release-helper.sh <version> [alpha|beta|rc|stable]

set -e  # Exit on any error

VERSION=$1
RELEASE_TYPE=${2:-stable}

if [ -z "$VERSION" ]; then
    echo "Usage: ./release-helper.sh <version> [alpha|beta|rc|stable]"
    echo "Example: ./release-helper.sh 1.0.0 beta"
    exit 1
fi

echo "========================================"
echo "Pulse24Sync Release Helper v$VERSION"
echo "Release Type: $RELEASE_TYPE"
echo "========================================"
echo

# Create releases directory
mkdir -p releases/v$VERSION

echo "Step 1: Building project..."
xcodebuild -project Builds/MacOSX/Pulse24Sync.xcodeproj -configuration Release

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
else
    echo "❌ Build failed!"
    exit 1
fi

echo
echo "Step 2: Packaging assets..."

# Package macOS AU Plugin
if [ -d "/Users/edahl/Library/Audio/Plug-Ins/Components/Pulse24Sync.component" ]; then
    cd "/Users/edahl/Library/Audio/Plug-Ins/Components"
    zip -r "../../releases/v$VERSION/Pulse24Sync-AU-macOS-v$VERSION.zip" Pulse24Sync.component
    echo "✅ macOS AU Plugin packaged"
else
    echo "⚠️  macOS AU Plugin not found"
fi

# Package macOS VST3 Plugin
if [ -d "/Users/edahl/Library/Audio/Plug-Ins/VST3/Pulse24Sync.vst3" ]; then
    cd "/Users/edahl/Library/Audio/Plug-Ins/VST3"
    zip -r "../../releases/v$VERSION/Pulse24Sync-VST3-macOS-v$VERSION.zip" Pulse24Sync.vst3
    echo "✅ macOS VST3 Plugin packaged"
else
    echo "⚠️  macOS VST3 Plugin not found"
fi

# Package macOS Standalone App
if [ -d "Builds/MacOSX/build/Release/Pulse24Sync.app" ]; then
    cd "Builds/MacOSX/build/Release"
    zip -r "../../../releases/v$VERSION/Pulse24Sync-Standalone-macOS-v$VERSION.zip" Pulse24Sync.app
    echo "✅ macOS Standalone App packaged"
else
    echo "⚠️  macOS Standalone App not found"
fi

# Create source archive
cd /Users/edahl/Documents/Pulse24/NewProject
zip -r "releases/v$VERSION/Pulse24Sync-Source-v$VERSION.zip" Source/ Pulse24Sync.jucer README.md WINDOWS_BUILD_GUIDE.md build_windows.bat .cursor/
echo "✅ Source code archived"

echo
echo "Step 3: Generating release notes..."

# Create release notes template
RELEASE_NOTES="releases/v$VERSION/release-notes-v$VERSION.md"

cat > "$RELEASE_NOTES" << EOF
## Pulse24Sync v$VERSION

### 🎉 Release
$(if [ "$RELEASE_TYPE" = "stable" ]; then
    echo "- Production-ready VST3 and AU plugin"
    echo "- 24 pulses per quarter note with tempo synchronization"
    echo "- Cross-platform support (macOS and Windows)"
    echo "- Modern dark theme UI with parameter automation"
else
    echo "- Pre-release version for testing and feedback"
    echo "- Please report any issues on GitHub"
    echo "- Not recommended for production use"
fi)

### 🔧 Features
- **Tempo Sync**: Automatic synchronization with host tempo
- **Pulse Generation**: Exactly 24 pulses per quarter note
- **Parameter Controls**: Volume, pulse width, and enable/disable
- **Real-time Processing**: Zero latency audio processing
- **Cross-platform**: macOS (AU/VST3) and Windows (VST3)

### 📦 Downloads
- **macOS AU Plugin**: [Pulse24Sync-AU-macOS-v$VERSION.zip](link)
- **macOS VST3 Plugin**: [Pulse24Sync-VST3-macOS-v$VERSION.zip](link)
- **macOS Standalone**: [Pulse24Sync-Standalone-macOS-v$VERSION.zip](link)
- **Source Code**: [Pulse24Sync-Source-v$VERSION.zip](link)

### 🚀 Installation
1. Download the appropriate plugin for your platform
2. Extract the archive
3. Copy to your DAW's plugin directory:
   - **macOS AU**: \`/Library/Audio/Plug-Ins/Components/\`
   - **macOS VST3**: \`/Library/Audio/Plug-Ins/VST3/\`
4. Restart your DAW and scan for new plugins

### 📋 System Requirements
- **macOS**: 10.13+ (Intel/Apple Silicon)
- **DAW**: Any VST3 or AU compatible host

### 🙏 Acknowledgments
Inspired by a request from Zac Caruso in the Synthesizer Zone Facebook group.

$(if [ "$RELEASE_TYPE" != "stable" ]; then
    echo "### 📝 Feedback"
    echo "Please report bugs and provide feedback on GitHub Issues."
fi)
EOF

echo "✅ Release notes generated: $RELEASE_NOTES"

echo
echo "Step 4: Release summary..."
echo "========================================"
echo "Release v$VERSION ($RELEASE_TYPE) prepared successfully!"
echo
echo "📁 Release files created in: releases/v$VERSION/"
ls -la "releases/v$VERSION/"
echo
echo "📝 Next steps:"
echo "1. Review the release notes: $RELEASE_NOTES"
echo "2. Create GitHub release using MCP tools"
echo "3. Upload the packaged assets"
echo "4. Tag the release with v$VERSION"
echo
echo "🎉 Ready for GitHub release!"