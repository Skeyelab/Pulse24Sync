#!/bin/bash

# Install Linux dependencies for Pulse24Sync VST Plugin build
# This script installs all required development libraries for Ubuntu/Debian systems

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}📦 Installing Linux dependencies for Pulse24Sync...${NC}"

# Check if running on Ubuntu/Debian
if ! command -v apt-get &> /dev/null; then
    echo -e "${YELLOW}⚠️  Warning: This script is designed for Ubuntu/Debian systems with apt-get${NC}"
    echo -e "${YELLOW}   For other distributions, please install equivalent packages manually${NC}"
    echo -e "${YELLOW}   See LINUX_BUILD_GUIDE.md for details${NC}"
fi

# Update package list
echo -e "${BLUE}🔄 Updating package list...${NC}"
sudo apt-get update

# Install essential build tools
echo -e "${BLUE}🔨 Installing build tools...${NC}"
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config

# Install audio libraries
echo -e "${BLUE}🎵 Installing audio libraries...${NC}"
sudo apt-get install -y \
    libasound2-dev \
    libjack-jackd2-dev

# Install graphics libraries
echo -e "${BLUE}🖼️  Installing graphics libraries...${NC}"
sudo apt-get install -y \
    libfreetype6-dev \
    libgl1-mesa-dev

# Install networking libraries
echo -e "${BLUE}🌐 Installing networking libraries...${NC}"
sudo apt-get install -y \
    libcurl4-openssl-dev

# Install X11 libraries
echo -e "${BLUE}🖥️  Installing X11 libraries...${NC}"
sudo apt-get install -y \
    libx11-dev \
    libxinerama-dev \
    libxext-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxcomposite-dev

# Install GUI libraries
echo -e "${BLUE}🎨 Installing GUI libraries...${NC}"
sudo apt-get install -y \
    libgtk-3-dev

# Verify installations
echo -e "${GREEN}✅ Verifying installations...${NC}"

# Check essential tools
echo -e "${BLUE}Checking build tools:${NC}"
cmake --version && echo -e "${GREEN}✓ CMake${NC}" || echo -e "${RED}✗ CMake${NC}"
gcc --version | head -1 && echo -e "${GREEN}✓ GCC${NC}" || echo -e "${RED}✗ GCC${NC}"
make --version | head -1 && echo -e "${GREEN}✓ Make${NC}" || echo -e "${RED}✗ Make${NC}"

# Check libraries using pkg-config
echo -e "${BLUE}Checking libraries:${NC}"
pkg-config --exists alsa && echo -e "${GREEN}✓ ALSA${NC}" || echo -e "${RED}✗ ALSA${NC}"
pkg-config --exists jack && echo -e "${GREEN}✓ JACK${NC}" || echo -e "${RED}✗ JACK (optional)${NC}"
pkg-config --exists freetype2 && echo -e "${GREEN}✓ FreeType2${NC}" || echo -e "${RED}✗ FreeType2${NC}"
pkg-config --exists gl && echo -e "${GREEN}✓ OpenGL${NC}" || echo -e "${RED}✗ OpenGL${NC}"
pkg-config --exists libcurl && echo -e "${GREEN}✓ cURL${NC}" || echo -e "${RED}✗ cURL${NC}"
pkg-config --exists x11 && echo -e "${GREEN}✓ X11${NC}" || echo -e "${RED}✗ X11${NC}"
pkg-config --exists gtk+-3.0 && echo -e "${GREEN}✓ GTK3${NC}" || echo -e "${RED}✗ GTK3${NC}"

echo -e "${GREEN}🎉 Linux dependencies installation complete!${NC}"
echo -e "${BLUE}💡 You can now build the plugin with: ./build.sh${NC}"
echo -e "${BLUE}📖 For more details, see: LINUX_BUILD_GUIDE.md${NC}"