#!/bin/bash
set -e

echo "🐳 Building Pulse24Sync with Docker and Projucer..."

# Check if Docker is available
if ! command -v docker &> /dev/null; then
    echo "❌ Error: Docker is not installed or not in PATH"
    exit 1
fi

echo "📦 Building Docker image..."
docker build -t pulse24sync-build .

echo "🚀 Running build container..."
docker run -d --name build-container pulse24sync-build tail -f /dev/null

echo "📋 Copying build artifacts..."
mkdir -p linux-builds
docker cp build-container:/workspace/Builds/LinuxMakefile/build ./linux-builds

echo "🧹 Cleaning up..."
docker stop build-container
docker rm build-container

echo "✅ Build completed! Check the 'linux-builds' directory for artifacts."
ls -la linux-builds/
