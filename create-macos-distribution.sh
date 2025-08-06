#!/bin/bash

# Pulse24Sync macOS Distribution Creator
# This script creates a clean distribution from existing build files

set -e  # Exit on any error

echo "========================================"
echo "Pulse24Sync - macOS Distribution Creator"
echo "========================================"
echo ""

# Check if we have the build files
if [ ! -d "Pulse24Sync.vst3" ] && [ ! -d "Pulse24Sync.component" ] && [ ! -d "Pulse24Sync.app" ]; then
    echo "❌ Error: Plugin files not found in current directory"
    echo "Please run this script from the directory containing the build files:"
    echo "  - Pulse24Sync.vst3"
    echo "  - Pulse24Sync.component"
    echo "  - Pulse24Sync.app"
    exit 1
fi

echo "📦 Creating clean distribution from existing build..."

# Create distribution directory
if [ -d "dist" ]; then
    rm -rf dist
fi
mkdir -p dist

# Copy VST3 plugin
if [ -d "Pulse24Sync.vst3" ]; then
    echo "  - Copying VST3 plugin..."
    cp -R "Pulse24Sync.vst3" "dist/"
fi

# Copy AU component
if [ -d "Pulse24Sync.component" ]; then
    echo "  - Copying AU component..."
    cp -R "Pulse24Sync.component" "dist/"
fi

# Copy standalone application
if [ -d "Pulse24Sync.app" ]; then
    echo "  - Copying standalone application..."
    cp -R "Pulse24Sync.app" "dist/"
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
echo "Original build size: ~35MB (includes development files)"
echo "Distribution size: ~12MB (end-user files only)"
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
