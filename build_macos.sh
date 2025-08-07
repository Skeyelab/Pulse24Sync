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

# Function to sign a bundle with proper entitlements
sign_bundle() {
    local bundle_path="$1"
    local bundle_name=$(basename "$bundle_path")
    
    if [ -z "$CODE_SIGN_IDENTITY" ]; then
        echo "  ⚠️  Skipping code signing for $bundle_name (no signing identity set)"
        return 0
    fi
    
    echo "  🔏 Code signing $bundle_name..."
    
    # Sign with entitlements and hardened runtime
    codesign --force --options runtime --entitlements entitlements.plist \
        --sign "$CODE_SIGN_IDENTITY" --timestamp "$bundle_path"
    
    if [ $? -eq 0 ]; then
        echo "  ✅ Successfully signed $bundle_name"
        
        # Verify the signature
        echo "  🔍 Verifying signature..."
        codesign --verify --deep --strict "$bundle_path"
        if [ $? -eq 0 ]; then
            echo "  ✅ Signature verification passed"
        else
            echo "  ⚠️  Signature verification failed"
        fi
    else
        echo "  ❌ Failed to sign $bundle_name"
        return 1
    fi
}

# Function to create clean distribution
create_distribution() {
    echo "📦 Creating clean distribution..."

    # Create distribution directory
    if [ -d "dist" ]; then
        rm -rf dist
    fi
    mkdir -p dist

    # Copy and sign VST3 plugin
    if [ -d "Builds/MacOSX/build/Release/Pulse24Sync.vst3" ]; then
        echo "  - Copying VST3 plugin..."
        cp -R "Builds/MacOSX/build/Release/Pulse24Sync.vst3" "dist/"
        sign_bundle "dist/Pulse24Sync.vst3"
    fi

    # Copy and sign AU component
    if [ -d "Builds/MacOSX/build/Release/Pulse24Sync.component" ]; then
        echo "  - Copying AU component..."
        cp -R "Builds/MacOSX/build/Release/Pulse24Sync.component" "dist/"
        sign_bundle "dist/Pulse24Sync.component"
    fi

    # Copy and sign standalone application
    if [ -d "Builds/MacOSX/build/Release/Pulse24Sync.app" ]; then
        echo "  - Copying standalone application..."
        cp -R "Builds/MacOSX/build/Release/Pulse24Sync.app" "dist/"
        sign_bundle "dist/Pulse24Sync.app"
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

# Check for code signing setup
echo "🔐 Checking code signing configuration..."

# Look for signing identity (can be set via environment variable or detected)
if [ -z "$CODE_SIGN_IDENTITY" ]; then
    # Try to detect Apple Developer certificates
    AVAILABLE_IDENTITIES=$(security find-identity -v -p codesigning | grep "Developer ID Application" | head -1)
    if [ ! -z "$AVAILABLE_IDENTITIES" ]; then
        # Extract the identity from the security output
        CODE_SIGN_IDENTITY=$(echo "$AVAILABLE_IDENTITIES" | sed -n 's/.*"\(.*\)"/\1/p')
        echo "  🔍 Auto-detected signing identity: $CODE_SIGN_IDENTITY"
    else
        echo "  ⚠️  No Developer ID Application certificate found"
        echo "  ℹ️  Plugins will be built unsigned (may trigger security warnings)"
        echo "  ℹ️  To enable signing, set CODE_SIGN_IDENTITY environment variable"
        echo "     Example: export CODE_SIGN_IDENTITY=\"Developer ID Application: Your Name (TEAMID)\""
    fi
else
    echo "  ✅ Using specified signing identity: $CODE_SIGN_IDENTITY"
fi

# Check for entitlements file
if [ ! -f "entitlements.plist" ]; then
    echo "  ⚠️  entitlements.plist not found - code signing may fail"
else
    echo "  ✅ Found entitlements.plist"
fi

echo ""

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
