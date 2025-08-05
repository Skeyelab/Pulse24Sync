#!/bin/bash

# Pulse24Sync macOS Build Script
# This script builds the VST plugin using Projucer and Xcode

set -e  # Exit on any error

echo "🎵 Building Pulse24Sync VST Plugin for macOS..."
echo "================================================"

# Check if we're in the right directory
if [ ! -f "Pulse24Sync.jucer" ]; then
    echo "❌ Error: Pulse24Sync.jucer not found. Please run this script from the project root directory."
    exit 1
fi

# Check if Projucer is available
PROJUCER_PATH="/Applications/JUCE 2/Projucer.app/Contents/MacOS/Projucer"
if [ ! -f "$PROJUCER_PATH" ]; then
    echo "❌ Error: Projucer not found at $PROJUCER_PATH"
    echo "Please install Projucer from https://juce.com/get-juce/"
    exit 1
fi

# Clean previous builds
echo "🧹 Cleaning previous builds..."
rm -rf Builds/MacOSX/build

# Generate Xcode project with Projucer
echo "⚙️  Generating Xcode project with Projucer..."
"$PROJUCER_PATH" --resave Pulse24Sync.jucer

# Check if Xcode project was created
if [ ! -d "Builds/MacOSX" ]; then
    echo "❌ Error: Xcode project was not generated. Check Projucer output."
    exit 1
fi

# Build with Xcode
echo "🔨 Building with Xcode..."
xcodebuild -project Builds/MacOSX/Pulse24Sync.xcodeproj -configuration Release -jobs $(sysctl -n hw.ncpu)

echo ""
echo "✅ Build completed successfully!"
echo "📦 VST3 plugin should be installed to:"
echo "   ~/Library/Audio/Plug-Ins/VST3/Pulse24Sync.vst3"
echo "   ~/Library/Audio/Plug-Ins/Components/Pulse24Sync.component"
echo ""
echo "🎉 You can now load Pulse24Sync in your DAW!"
