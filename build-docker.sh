#!/bin/bash
set -e

echo "🐳 Building Pulse24Sync with Docker and CMake..."

# Check if Docker is available
if ! command -v docker &> /dev/null; then
    echo "❌ Error: Docker is not installed or not in PATH"
    exit 1
fi

# Calculate content hash for cache invalidation
echo "🔍 Calculating content hash for cache optimization..."
CONTENT_HASH=$(find Source/ CMakeLists.txt Pulse24Sync.jucer -type f -exec sha256sum {} \; 2>/dev/null | sha256sum | cut -d' ' -f1 | head -c 12)
IMAGE_NAME="pulse24sync-build:cache-${CONTENT_HASH}"
CONTAINER_NAME="build-container-${CONTENT_HASH}"

echo "📋 Using cache key: ${CONTENT_HASH}"

# Check if we already have a cached image with this content
if docker images --format '{{.Repository}}:{{.Tag}}' | grep -q "^${IMAGE_NAME}$"; then
    echo "✅ Found cached build for current content - skipping rebuild"
    SKIP_BUILD=true
else
    echo "🔨 Content changed or no cache found - building new image"
    SKIP_BUILD=false
fi

# Cleanup function for failed builds and old containers
cleanup() {
    local exit_code=$?
    echo "🧹 Performing cleanup..."
    
    # Stop and remove container if it exists (always clean containers)
    if docker ps -a --format '{{.Names}}' | grep -q "^${CONTAINER_NAME}$"; then
        echo "  - Stopping and removing build container..."
        docker stop ${CONTAINER_NAME} >/dev/null 2>&1 || true
        docker rm ${CONTAINER_NAME} >/dev/null 2>&1 || true
    fi
    
    # Only remove Docker image if build failed
    if [ $exit_code -ne 0 ] && docker images --format '{{.Repository}}:{{.Tag}}' | grep -q "^${IMAGE_NAME}$"; then
        echo "  - Build failed: removing incomplete Docker image..."
        docker rmi ${IMAGE_NAME} >/dev/null 2>&1 || true
    fi
    
    # Clean up old cached images (keep last 3 cache entries)
    echo "  - Cleaning up old cached images..."
    docker images --format '{{.Repository}}:{{.Tag}}' | grep '^pulse24sync-build:cache-' | sort -r | tail -n +4 | xargs -r docker rmi >/dev/null 2>&1 || true
    
    # Clean up dangling images
    if [ "$(docker images -f "dangling=true" -q)" ]; then
        echo "  - Cleaning up dangling Docker images..."
        docker image prune -f >/dev/null 2>&1 || true
    fi
    
    echo "🧹 Cleanup completed"
    exit $exit_code
}

# Set trap to ensure cleanup runs on script exit (success, failure, or interruption)
trap cleanup EXIT INT TERM

if [ "$SKIP_BUILD" = "false" ]; then
    echo "📦 Building Docker image with cache key ${CONTENT_HASH}..."
    docker build -t ${IMAGE_NAME} .
else
    echo "📦 Using cached Docker image ${IMAGE_NAME}"
fi

echo "🚀 Running build container..."
docker run -d --name ${CONTAINER_NAME} ${IMAGE_NAME} tail -f /dev/null

echo "📋 Copying build artifacts..."
mkdir -p linux-builds
docker cp ${CONTAINER_NAME}:/workspace/build ./linux-builds

echo "✅ Build completed! Check the 'linux-builds' directory for artifacts."
ls -la linux-builds/

echo "💾 Cached image ${IMAGE_NAME} preserved for future builds with same content"

# Note: cleanup() will be called automatically due to the trap
