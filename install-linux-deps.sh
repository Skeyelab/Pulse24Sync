#!/bin/bash

# Linux Dependencies Installation Script for Pulse24Sync VST Plugin
# This script installs all required development libraries for building JUCE applications on Linux

set -e

echo "🐧 Installing Linux Dependencies for Pulse24Sync VST Plugin..."
echo "============================================================="

# Update package lists
echo "📦 Updating package lists..."
sudo apt-get update

# Install essential build tools
echo "🔧 Installing essential build tools..."
sudo apt-get install -y build-essential cmake git pkg-config ccache

# Install JUCE dependencies for Linux
echo "🎵 Installing JUCE dependencies..."
sudo apt-get install -y \
    libasound2-dev \
    libjack-jackd2-dev \
    libfreetype6-dev \
    libgl1-mesa-dev \
    libcurl4-openssl-dev \
    libx11-dev \
    libxinerama-dev \
    libxext-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxcomposite-dev \
    libgtk-3-dev

echo ""
echo "⚡ Setting up ccache for faster builds..."
# Configure ccache for faster subsequent builds
ccache --set-config=cache_dir=$HOME/.cache/ccache
ccache --set-config=max_size=2G
ccache --set-config=compression=true

echo ""
echo "✅ All dependencies installed successfully!"
echo ""
echo "📋 Installed packages:"
echo "   - Build tools: build-essential, cmake, git, pkg-config, ccache"
echo "   - Audio: libasound2-dev (ALSA), libjack-jackd2-dev (JACK)"
echo "   - Graphics: libfreetype6-dev, libgl1-mesa-dev"
echo "   - Networking: libcurl4-openssl-dev"
echo "   - X11: libx11-dev, libxinerama-dev, libxext-dev, libxrandr-dev"
echo "   - GUI: libxcursor-dev, libxcomposite-dev, libgtk-3-dev"
echo ""
echo "⚡ ccache configured for faster compilation (cache at ~/.cache/ccache)"
echo "🚀 You can now run './build.sh' to build the VST plugin!"