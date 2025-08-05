#!/bin/bash

# Pulse24Sync VST Plugin Build Script
# This script builds the VST plugin using CMake

set -e  # Exit on any error

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
