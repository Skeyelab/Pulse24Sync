#!/bin/bash

# Pulse24Sync Build Cleanup Script
# This script cleans up build artifacts and temporary files

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}🧹 Pulse24Sync Build Cleanup${NC}"
echo "=================================="

# Function to show size before and after cleanup
show_size() {
    local path="$1"
    local label="$2"
    if [ -d "$path" ] || [ -f "$path" ]; then
        local size=$(du -sh "$path" 2>/dev/null | cut -f1)
        echo -e "${YELLOW}  $label: $size${NC}"
    fi
}

# Function to clean up files/directories with size reporting
cleanup_with_size() {
    local pattern="$1"
    local description="$2"
    local found_files=()
    
    echo -e "${BLUE}Cleaning $description...${NC}"
    
    # Find files/directories matching pattern
    while IFS= read -r -d '' item; do
        found_files+=("$item")
    done < <(find . -name "$pattern" -print0 2>/dev/null)
    
    if [ ${#found_files[@]} -eq 0 ]; then
        echo -e "${GREEN}  ✓ No $description found${NC}"
        return 0
    fi
    
    # Show sizes and remove
    local total_size=0
    for item in "${found_files[@]}"; do
        if [ -e "$item" ]; then
            show_size "$item" "$(basename "$item")"
            rm -rf "$item" 2>/dev/null || true
        fi
    done
    
    echo -e "${GREEN}  ✓ Cleaned ${#found_files[@]} $description${NC}"
}

# Check if we're in the project root
if [ ! -f "CMakeLists.txt" ] && [ ! -f "Pulse24Sync.jucer" ]; then
    echo -e "${RED}❌ Error: Not in the project root directory${NC}"
    echo "Please run this script from the Pulse24Sync project root."
    exit 1
fi

echo -e "${YELLOW}📊 Analyzing current disk usage...${NC}"
if command -v du &> /dev/null; then
    INITIAL_SIZE=$(du -sh . 2>/dev/null | cut -f1)
    echo -e "${YELLOW}  Project size: $INITIAL_SIZE${NC}"
fi

echo ""
echo -e "${BLUE}🧹 Starting cleanup...${NC}"

# Clean up build directories
cleanup_with_size "build" "build directories"
cleanup_with_size "build-*" "variant build directories"

# Clean up CMake generated files
cleanup_with_size "CMakeCache.txt" "CMake cache files"
cleanup_with_size "CMakeFiles" "CMake generated directories"
cleanup_with_size "cmake_install.cmake" "CMake install files"
cleanup_with_size "Makefile" "generated Makefiles"
cleanup_with_size "*.cmake" "CMake configuration files"

# Clean up JUCE generated files
cleanup_with_size "JuceLibraryCode" "JUCE library code directories"
cleanup_with_size "Builds" "JUCE build directories"

# Clean up platform-specific build artifacts
echo -e "${BLUE}Cleaning platform-specific artifacts...${NC}"

# macOS specific
if [[ "$OSTYPE" == "darwin"* ]]; then
    cleanup_with_size ".DS_Store" "macOS .DS_Store files"
    cleanup_with_size "*.build" "Xcode build directories"
    cleanup_with_size "DerivedData" "Xcode DerivedData directories"
    cleanup_with_size "*.xcworkspace" "Xcode workspace files"
fi

# Windows specific (when running under WSL or Git Bash)
cleanup_with_size "Thumbs.db" "Windows thumbnail cache"
cleanup_with_size "ehthumbs.db" "Windows thumbnail cache"
cleanup_with_size ".vs" "Visual Studio directories"

# Clean up compiled object files
cleanup_with_size "*.o" "object files"
cleanup_with_size "*.obj" "object files"
cleanup_with_size "*.slo" "shared object files"
cleanup_with_size "*.lo" "libtool object files"

# Clean up precompiled headers
cleanup_with_size "*.gch" "precompiled headers"
cleanup_with_size "*.pch" "precompiled headers"

# Clean up static libraries
cleanup_with_size "*.a" "static libraries"
cleanup_with_size "*.lib" "static libraries"
cleanup_with_size "*.lai" "libtool archive files"
cleanup_with_size "*.la" "libtool shared library files"

# Clean up dynamic libraries
cleanup_with_size "*.so" "shared libraries"
cleanup_with_size "*.dylib" "dynamic libraries"
cleanup_with_size "*.dll" "dynamic libraries"

# Clean up executables
cleanup_with_size "*.exe" "executable files"
cleanup_with_size "*.out" "executable files"
cleanup_with_size "*.app" "application bundles"

# Clean up temporary files
cleanup_with_size "*.tmp" "temporary files"
cleanup_with_size "*.temp" "temporary files"
cleanup_with_size "*~" "backup files"

# Clean up log files
cleanup_with_size "*.log" "log files"

# Clean up local build artifacts directories
cleanup_with_size "linux-builds*" "Linux build artifact directories"
cleanup_with_size "release-packages" "release package directories"

# Docker cleanup (if Docker is available)
if command -v docker &> /dev/null; then
    echo -e "${BLUE}Cleaning Docker resources...${NC}"
    
    # Remove project-specific containers
    CONTAINERS=$(docker ps -a --filter "name=build-container" --filter "name=pulse24sync" -q 2>/dev/null || true)
    if [ -n "$CONTAINERS" ]; then
        echo -e "${YELLOW}  Stopping and removing containers...${NC}"
        docker stop $CONTAINERS >/dev/null 2>&1 || true
        docker rm $CONTAINERS >/dev/null 2>&1 || true
        echo -e "${GREEN}  ✓ Cleaned Docker containers${NC}"
    fi
    
    # Remove project-specific images
    IMAGES=$(docker images --filter "reference=pulse24sync*" -q 2>/dev/null || true)
    if [ -n "$IMAGES" ]; then
        echo -e "${YELLOW}  Removing Docker images...${NC}"
        docker rmi $IMAGES >/dev/null 2>&1 || true
        echo -e "${GREEN}  ✓ Cleaned Docker images${NC}"
    fi
    
    # Clean up dangling images
    echo -e "${YELLOW}  Cleaning dangling Docker resources...${NC}"
    docker system prune -f >/dev/null 2>&1 || true
    echo -e "${GREEN}  ✓ Cleaned dangling Docker resources${NC}"
fi

# Clean up temporary Dockerfiles
cleanup_with_size "Dockerfile.debug" "temporary Dockerfiles"

echo ""
echo -e "${BLUE}📊 Final disk usage analysis...${NC}"
if command -v du &> /dev/null; then
    FINAL_SIZE=$(du -sh . 2>/dev/null | cut -f1)
    echo -e "${GREEN}  Project size after cleanup: $FINAL_SIZE${NC}"
    echo -e "${GREEN}  Original size: $INITIAL_SIZE${NC}"
fi

echo ""
echo -e "${GREEN}✅ Cleanup completed successfully!${NC}"
echo -e "${BLUE}📋 What was cleaned:${NC}"
echo "  • Build directories and CMake cache files"
echo "  • JUCE generated project files"
echo "  • Compiled object files and libraries"
echo "  • Platform-specific temporary files"
echo "  • Docker containers and images (if Docker available)"
echo "  • Log files and backup files"
echo ""
echo -e "${YELLOW}💡 Note: This cleanup preserves source code and configuration files.${NC}"
echo -e "${YELLOW}   Run your build script again to rebuild the project.${NC}"