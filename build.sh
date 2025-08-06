#!/bin/bash

# Pulse24Sync VST Plugin Build Script
# This script builds the VST plugin using CMake

set -e  # Exit on any error

# Cleanup function to ensure temporary files are cleaned up
cleanup() {
    local exit_code=$?
    echo "🧹 Performing cleanup..."
    
    # Clean up CMake cache and temporary files if build failed
    if [ $exit_code -ne 0 ] && [ -d "build" ]; then
        echo "  - Cleaning up failed build artifacts..."
        rm -rf build/CMakeCache.txt build/CMakeFiles/ build/*.cmake 2>/dev/null || true
    fi
    
    # Clean up any temporary files that might have been created
    find . -name "*.tmp" -o -name "*.temp" -o -name "*~" -type f -delete 2>/dev/null || true
    
    if [ $exit_code -ne 0 ]; then
        echo "❌ Build failed - temporary files cleaned up"
    fi
    
    exit $exit_code
}

# Set trap to ensure cleanup runs on script exit
trap cleanup EXIT INT TERM

echo "🎵 Building Pulse24Sync VST Plugin..."
echo "======================================"

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "❌ Error: CMakeLists.txt not found. Please run this script from the project root directory."
    exit 1
fi

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "❌ Error: CMake is not installed. Please install CMake 3.15 or higher."
    exit 1
fi

# Check CMake version
CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
echo "📋 CMake version: $CMAKE_VERSION"

# Create build directory
echo "📁 Creating build directory..."
rm -rf build
mkdir build
cd build

# Configure with CMake
echo "⚙️  Configuring project with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "🔨 Building project..."
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    make -j$(sysctl -n hw.ncpu)
else
    # Linux
    make -j$(nproc)
fi

echo ""
echo "✅ Build completed successfully!"
echo "📦 VST3 plugin should be installed to:"
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "   ~/Library/Audio/Plug-Ins/VST3/Pulse24Sync.vst3"
else
    echo "   ~/.vst3/Pulse24Sync.vst3"
fi
echo ""
echo "🎉 You can now load Pulse24Sync in your DAW!"

# Note: cleanup() will be called automatically, but successful builds keep artifacts
