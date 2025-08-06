#!/bin/bash
set -e

echo "🐳 Building Pulse24Sync with Docker and CMake..."

# Check if Docker is available
if ! command -v docker &> /dev/null; then
    echo "❌ Error: Docker is not installed or not in PATH"
    exit 1
fi

# Cleanup function to ensure resources are cleaned up on exit
cleanup() {
    local exit_code=$?
    echo "🧹 Performing cleanup..."
    
    # Stop and remove container if it exists
    if docker ps -a --format '{{.Names}}' | grep -q "^build-container$"; then
        echo "  - Stopping and removing build container..."
        docker stop build-container >/dev/null 2>&1 || true
        docker rm build-container >/dev/null 2>&1 || true
    fi
    
    # Remove Docker image to prevent accumulation
    if docker images --format '{{.Repository}}:{{.Tag}}' | grep -q "^pulse24sync-build:latest$"; then
        echo "  - Removing Docker image..."
        docker rmi pulse24sync-build:latest >/dev/null 2>&1 || true
    fi
    
    # Clean up any dangling images
    if [ "$(docker images -f "dangling=true" -q)" ]; then
        echo "  - Cleaning up dangling Docker images..."
        docker image prune -f >/dev/null 2>&1 || true
    fi
    
    echo "🧹 Cleanup completed"
    exit $exit_code
}

# Set trap to ensure cleanup runs on script exit (success, failure, or interruption)
trap cleanup EXIT INT TERM

echo "📦 Building Docker image..."
docker build -t pulse24sync-build .

echo "🚀 Running build container..."
docker run -d --name build-container pulse24sync-build tail -f /dev/null

echo "📋 Copying build artifacts..."
mkdir -p linux-builds
docker cp build-container:/workspace/build ./linux-builds

echo "✅ Build completed! Check the 'linux-builds' directory for artifacts."
ls -la linux-builds/

# Note: cleanup() will be called automatically due to the trap
