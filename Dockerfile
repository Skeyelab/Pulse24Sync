FROM ubuntu:22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV JUCE_VERSION=7.0.12

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    libasound2-dev \
    libfreetype6-dev \
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxinerama-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libwebkit2gtk-4.0-dev \
    libcurl4-openssl-dev \
    libssl-dev \
    libjack-jackd2-dev \
    libvst3sdk-dev \
    && rm -rf /var/lib/apt/lists/*

# Create working directory
WORKDIR /workspace

# Download and install JUCE
RUN wget -O juce.zip https://github.com/juce-framework/JUCE/archive/refs/tags/${JUCE_VERSION}.zip \
    && unzip juce.zip \
    && mv JUCE-${JUCE_VERSION} JUCE \
    && rm juce.zip

# Set JUCE path
ENV JUCE_PATH=/workspace/JUCE

# Copy project files
COPY . /workspace/Pulse24Sync/

# Set working directory to project
WORKDIR /workspace/Pulse24Sync

# Create build directory and build
RUN mkdir -p build \
    && cd build \
    && cmake .. \
    && make -j$(nproc)

# Default command
CMD ["bash"] 