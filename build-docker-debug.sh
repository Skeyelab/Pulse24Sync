#!/bin/bash
set -e

echo "🐳 Building Pulse24Sync with Docker and Projucer (DEBUG MODE)..."

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
    if docker ps -a --format '{{.Names}}' | grep -q "^build-container-debug$"; then
        echo "  - Stopping and removing debug build container..."
        docker stop build-container-debug >/dev/null 2>&1 || true
        docker rm build-container-debug >/dev/null 2>&1 || true
    fi
    
    # Remove Docker image to prevent accumulation
    if docker images --format '{{.Repository}}:{{.Tag}}' | grep -q "^pulse24sync-build-debug:latest$"; then
        echo "  - Removing debug Docker image..."
        docker rmi pulse24sync-build-debug:latest >/dev/null 2>&1 || true
    fi
    
    # Clean up the temporary Dockerfile.debug
    if [ -f "Dockerfile.debug" ]; then
        echo "  - Removing temporary Dockerfile.debug..."
        rm -f Dockerfile.debug
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

# Create a Dockerfile for the build environment
cat > Dockerfile.debug << 'EOF'
FROM ubuntu:24.04

# Install dependencies
RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  git \
  libasound2-dev \
  libjack-jackd2-dev \
  libfreetype6-dev \
  libcurl4-openssl-dev \
  libwebkit2gtk-4.1-dev \
  libgtk-3-dev \
  libx11-dev \
  libxrandr-dev \
  libxinerama-dev \
  libxcursor-dev \
  libxi-dev \
  && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy project files
COPY . .

# Clone JUCE and build Projucer
RUN git clone https://github.com/juce-framework/JUCE.git && \
    cd JUCE && \
    git checkout 7.0.12 && \
    cd extras/Projucer/Builds/LinuxMakefile && \
    make -j$(nproc)

# Debug: Show directory structure
RUN echo "=== DIRECTORY STRUCTURE ===" && \
    pwd && \
    ls -la && \
    echo "=== JUCE DIRECTORY ===" && \
    ls -la JUCE/ && \
    echo "=== JUCE MODULES ===" && \
    ls -la JUCE/modules/ && \
    echo "=== ORIGINAL JUCER PATHS ===" && \
    grep -n "path=" Pulse24Sync.jucer && \
    echo "=== MODULEPATHS ===" && \
    grep -n "<MODULEPATH" Pulse24Sync.jucer

# Update project file with correct paths
RUN sed -i 's|path="../../../Downloads/JUCE/modules"|path="JUCE/modules"|g' Pulse24Sync.jucer && \
    sed -i 's|path="../JUCE/modules"|path="JUCE/modules"|g' Pulse24Sync.jucer

# Debug: Show updated paths
RUN echo "=== UPDATED JUCER PATHS ===" && \
    grep -n "path=" Pulse24Sync.jucer && \
    echo "=== UPDATED MODULEPATHS ===" && \
    grep -n "<MODULEPATH" Pulse24Sync.jucer

# Debug: Test if paths exist
RUN echo "=== PATH EXISTENCE TESTS ===" && \
    ls -la JUCE/modules/ && \
    echo "Path JUCE/modules exists: $([ -d "JUCE/modules" ] && echo "YES" || echo "NO")" && \
    echo "Path JUCE/modules/juce_audio_basics exists: $([ -d "JUCE/modules/juce_audio_basics" ] && echo "YES" || echo "NO")"

# Generate project with Projucer
RUN ./JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave Pulse24Sync.jucer

# Build the project
RUN cd Builds/LinuxMakefile && make -j$(nproc)

# Keep container running to copy artifacts
CMD ["tail", "-f", "/dev/null"]
EOF

echo "📦 Building Docker image (debug mode)..."
docker build -f Dockerfile.debug -t pulse24sync-build-debug .

echo "🚀 Running build container (debug mode)..."
docker run -d --name build-container-debug pulse24sync-build-debug tail -f /dev/null

echo "📋 Copying build artifacts..."
mkdir -p linux-builds-debug
docker cp build-container-debug:/workspace/Builds/LinuxMakefile/build ./linux-builds-debug

echo "✅ Debug build completed! Check the 'linux-builds-debug' directory for artifacts."
ls -la linux-builds-debug/

# Note: cleanup() will be called automatically due to the trap
