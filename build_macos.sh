#!/bin/bash

# Pulse24Sync macOS Build Script
# This script builds the VST plugin using Projucer and Xcode

set -e  # Exit on any error

# Cleanup function to ensure temporary files are cleaned up
cleanup() {
    local exit_code=$?
    echo "🧹 Performing cleanup..."

    # Clean up temporary files
    find . -name "*.tmp" -o -name "*.temp" -o -name "*~" -type f -delete 2>/dev/null || true

    # Clean up Xcode temporary files
    find . -name "*.build" -type d -exec rm -rf {} + 2>/dev/null || true
    find . -name "DerivedData" -type d -exec rm -rf {} + 2>/dev/null || true

    # Clean up macOS system files
    find . -name ".DS_Store" -type f -delete 2>/dev/null || true

    if [ $exit_code -ne 0 ]; then
        echo "❌ Build failed - temporary files cleaned up"
        # Clean up failed build artifacts
        if [ -d "Builds/MacOSX/build" ]; then
            echo "  - Cleaning up failed build artifacts..."
            rm -rf Builds/MacOSX/build
        fi
    fi

    exit $exit_code
}

# Function to create clean distribution
create_distribution() {
    echo "📦 Creating clean distribution..."

    # Create distribution directory
    if [ -d "dist" ]; then
        rm -rf dist
    fi
    mkdir -p dist

    # Copy VST3 plugin
    if [ -d "Builds/MacOSX/build/Release/Pulse24Sync.vst3" ]; then
        echo "  - Copying VST3 plugin..."
        cp -R "Builds/MacOSX/build/Release/Pulse24Sync.vst3" "dist/"
    fi

    # Copy AU component
    if [ -d "Builds/MacOSX/build/Release/Pulse24Sync.component" ]; then
        echo "  - Copying AU component..."
        cp -R "Builds/MacOSX/build/Release/Pulse24Sync.component" "dist/"
    fi

    # Copy standalone application
    if [ -d "Builds/MacOSX/build/Release/Pulse24Sync.app" ]; then
        echo "  - Copying standalone application..."
        cp -R "Builds/MacOSX/build/Release/Pulse24Sync.app" "dist/"
    fi

    # Show distribution size
    echo ""
    echo "📊 Distribution created in 'dist' folder:"
    if [ -d "dist" ]; then
        du -sh dist/*
    fi

    echo ""
    echo "========================================"
    echo "Clean Distribution Created Successfully!"
    echo "========================================"
    echo ""
    echo "Distribution files are in the 'dist' folder:"
    echo "  - Pulse24Sync.vst3 (VST3 plugin)"
    echo "  - Pulse24Sync.component (AU plugin)"
    echo "  - Pulse24Sync.app (Standalone application)"
    echo ""
    echo "These files are ready for distribution to end users."
    echo ""
}

# Set trap to ensure cleanup runs on script exit
trap cleanup EXIT INT TERM

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

# Create clean distribution
create_distribution

echo ""
echo "========================================"
echo "Installation Instructions:"
echo "========================================"
echo ""
echo "To install the plugins, copy the files from the 'dist' folder to:"
echo "  VST3: ~/Library/Audio/Plug-Ins/VST3/"
echo "  AU:   ~/Library/Audio/Plug-Ins/Components/"
echo ""
echo "Then restart your DAW and scan for new plugins."
echo ""

# Note: cleanup() will be called automatically, but successful builds keep artifacts
