#!/bin/bash

# Pulse24Sync VST Plugin Docker Build Script
# This script builds the VST plugin using Docker to avoid macOS compatibility issues

set -e  # Exit on any error

echo "🎵 Building Pulse24Sync VST Plugin with Docker..."
echo "================================================"

# Check if Docker is installed
if ! command -v docker &> /dev/null; then
    echo "❌ Error: Docker is not installed. Please install Docker first."
    exit 1
fi

# Check if docker-compose is installed
if ! command -v docker-compose &> /dev/null; then
    echo "❌ Error: docker-compose is not installed. Please install docker-compose first."
    exit 1
fi

# Build the Docker image
echo "🐳 Building Docker image..."
docker-compose build

# Run the build
echo "🔨 Building VST plugin in Docker container..."
docker-compose run --rm pulse24sync-build bash -c "
    cd /workspace/Pulse24Sync
    rm -rf build
    mkdir build
    cd build
    cmake ..
    make -j\$(nproc)
    echo '✅ Build completed successfully!'
    echo '📦 VST3 plugin should be in the build directory'
"

echo ""
echo "✅ Docker build completed successfully!"
echo "📦 Check the build/ directory for your VST plugin files"
echo ""
echo "🎉 You can now copy the VST plugin to your DAW's plugin directory!" 