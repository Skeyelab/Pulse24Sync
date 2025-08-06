#!/bin/bash

# Build Cleanup Script - Smart Cache-Aware Version
# This script provides different levels of cleanup while respecting cache optimization

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Default cleanup level
CLEANUP_LEVEL="normal"

# Function to display help
show_help() {
    echo -e "${CYAN}Build Cleanup Script - Cache-Aware Version${NC}"
    echo
    echo "Usage: $0 [OPTION]"
    echo
    echo "Cleanup levels:"
    echo "  --gentle     Clean temporary files but preserve all caches"
    echo "  --normal     Standard cleanup with cache preservation (default)"
    echo "  --aggressive Complete cleanup including all caches"
    echo "  --docker     Clean only Docker resources"
    echo "  --temp       Clean only temporary files"
    echo
    echo "Options:"
    echo "  --dry-run    Show what would be cleaned without actually doing it"
    echo "  --help       Show this help message"
    echo
    echo "Cache-aware cleanup preserves:"
    echo "  - Docker build cache layers"
    echo "  - GitHub Actions cache compatibility"
    echo "  - CMake build caches (when safe)"
    echo "  - JUCE project generation cache"
}

# Function to get disk usage
get_disk_usage() {
    if [ -d "$1" ]; then
        du -sh "$1" 2>/dev/null | cut -f1
    else
        echo "0B"
    fi
}

# Function to show what will be cleaned
show_cleanup_plan() {
    echo -e "${BLUE}📋 Cleanup Plan (Level: $CLEANUP_LEVEL)${NC}"
    echo
    
    case $CLEANUP_LEVEL in
        gentle)
            echo "✅ Will clean:"
            echo "  - Temporary files (.tmp, .temp, *~)"
            echo "  - OS-specific files (.DS_Store, Thumbs.db)"
            echo "  - Log files (*.log)"
            echo
            echo "❌ Will preserve:"
            echo "  - All Docker images and cache"
            echo "  - Build directories (build/, Builds/)"
            echo "  - CMake cache files"
            echo "  - JUCE generated files"
            ;;
        normal)
            echo "✅ Will clean:"
            echo "  - Temporary files and logs"
            echo "  - Build intermediates (.o, .obj files)"
            echo "  - Old build directories (keeping cache structure)"
            echo "  - Dangling Docker resources"
            echo
            echo "❌ Will preserve:"
            echo "  - Docker build cache (with content hashing)"
            echo "  - Recent cached builds"
            echo "  - JUCE project files (if recently generated)"
            ;;
        aggressive)
            echo "✅ Will clean:"
            echo "  - All temporary files and logs"
            echo "  - All build directories and artifacts"
            echo "  - All Docker images and containers"
            echo "  - All CMake and JUCE generated files"
            echo "  - All caches"
            echo
            echo "⚠️  Complete rebuild will be required after this cleanup"
            ;;
    esac
    echo
}

# Function to clean temporary files
clean_temp_files() {
    echo -e "${YELLOW}🧹 Cleaning temporary files...${NC}"
    
    # Cross-platform temporary files
    find . -name "*.tmp" -type f -delete 2>/dev/null || true
    find . -name "*.temp" -type f -delete 2>/dev/null || true
    find . -name "*~" -type f -delete 2>/dev/null || true
    find . -name "*.log" -type f -delete 2>/dev/null || true
    
    # macOS specific
    find . -name ".DS_Store" -type f -delete 2>/dev/null || true
    
    # Windows specific
    find . -name "Thumbs.db" -type f -delete 2>/dev/null || true
    find . -name "ehthumbs.db" -type f -delete 2>/dev/null || true
    
    echo "  ✅ Temporary files cleaned"
}

# Function to clean build intermediates
clean_build_intermediates() {
    echo -e "${YELLOW}🧹 Cleaning build intermediates...${NC}"
    
    # Object files
    find . -name "*.o" -type f -delete 2>/dev/null || true
    find . -name "*.obj" -type f -delete 2>/dev/null || true
    
    # Static libraries
    find . -name "*.a" -type f -delete 2>/dev/null || true
    find . -name "*.lib" -type f -delete 2>/dev/null || true
    
    echo "  ✅ Build intermediates cleaned"
}

# Function to clean Docker resources (cache-aware)
clean_docker_cache_aware() {
    echo -e "${YELLOW}🐳 Cleaning Docker resources (cache-aware)...${NC}"
    
    # Stop and remove any running build containers
    for container in $(docker ps -a --format '{{.Names}}' | grep -E '^build-container|^pulse24sync' 2>/dev/null || true); do
        echo "  - Stopping container: $container"
        docker stop "$container" >/dev/null 2>&1 || true
        docker rm "$container" >/dev/null 2>&1 || true
    done
    
    # Clean up old cached images (keep last 3)
    echo "  - Cleaning old cached Docker images..."
    docker images --format '{{.Repository}}:{{.Tag}}' | grep '^pulse24sync-build:cache-' | sort -r | tail -n +4 | xargs -r docker rmi >/dev/null 2>&1 || true
    
    # Clean dangling images
    if [ "$(docker images -f "dangling=true" -q)" ]; then
        echo "  - Cleaning dangling Docker images..."
        docker image prune -f >/dev/null 2>&1 || true
    fi
    
    echo "  ✅ Docker resources cleaned (cache preserved)"
}

# Function to clean Docker aggressively
clean_docker_aggressive() {
    echo -e "${YELLOW}🐳 Cleaning Docker resources (aggressive)...${NC}"
    
    # Stop and remove all related containers
    for container in $(docker ps -a --format '{{.Names}}' | grep -E 'pulse24sync|build-container' 2>/dev/null || true); do
        echo "  - Removing container: $container"
        docker stop "$container" >/dev/null 2>&1 || true
        docker rm "$container" >/dev/null 2>&1 || true
    done
    
    # Remove all related images
    for image in $(docker images --format '{{.Repository}}:{{.Tag}}' | grep 'pulse24sync' 2>/dev/null || true); do
        echo "  - Removing image: $image"
        docker rmi "$image" >/dev/null 2>&1 || true
    done
    
    # System cleanup
    docker system prune -f >/dev/null 2>&1 || true
    
    echo "  ✅ Docker resources completely cleaned"
}

# Function to clean build directories (cache-aware)
clean_build_dirs_cache_aware() {
    echo -e "${YELLOW}🏗️  Cleaning build directories (cache-aware)...${NC}"
    
    # Remove build artifacts but preserve structure that can be cached
    if [ -d "build" ]; then
        # Only remove specific subdirectories, not the entire build cache
        find build/ -name "CMakeFiles" -type d -exec rm -rf {} + 2>/dev/null || true
        find build/ -name "*.cmake" -type f -delete 2>/dev/null || true
        rm -f build/CMakeCache.txt 2>/dev/null || true
    fi
    
    # Clean platform-specific build artifacts
    if [ -d "Builds" ]; then
        # Clean intermediate files but preserve project structure
        find Builds/ -name "*.build" -type d -exec rm -rf {} + 2>/dev/null || true
        find Builds/ -name ".vs" -type d -exec rm -rf {} + 2>/dev/null || true
        find Builds/ -name "DerivedData" -type d -exec rm -rf {} + 2>/dev/null || true
    fi
    
    echo "  ✅ Build directories cleaned (cache structure preserved)"
}

# Function to clean build directories aggressively
clean_build_dirs_aggressive() {
    echo -e "${YELLOW}🏗️  Cleaning build directories (aggressive)...${NC}"
    
    # Remove everything
    rm -rf build/ 2>/dev/null || true
    rm -rf Builds/ 2>/dev/null || true
    rm -rf JuceLibraryCode/ 2>/dev/null || true
    rm -rf linux-builds/ 2>/dev/null || true
    
    # CMake files
    rm -f CMakeCache.txt 2>/dev/null || true
    rm -rf CMakeFiles/ 2>/dev/null || true
    find . -name "*.cmake" -type f -delete 2>/dev/null || true
    
    echo "  ✅ Build directories completely removed"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --gentle)
            CLEANUP_LEVEL="gentle"
            shift
            ;;
        --normal)
            CLEANUP_LEVEL="normal"
            shift
            ;;
        --aggressive)
            CLEANUP_LEVEL="aggressive"
            shift
            ;;
        --docker)
            CLEANUP_LEVEL="docker"
            shift
            ;;
        --temp)
            CLEANUP_LEVEL="temp"
            shift
            ;;
        --dry-run)
            DRY_RUN=true
            shift
            ;;
        --help)
            show_help
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Show initial disk usage
echo -e "${CYAN}📊 Initial Disk Usage:${NC}"
echo "  Project size: $(get_disk_usage .)"
if [ -d "build" ]; then
    echo "  Build dir: $(get_disk_usage build)"
fi
if [ -d "Builds" ]; then
    echo "  Builds dir: $(get_disk_usage Builds)"
fi
echo

# Show cleanup plan
show_cleanup_plan

if [ "$DRY_RUN" = "true" ]; then
    echo -e "${YELLOW}🔍 DRY RUN - No actual cleanup performed${NC}"
    exit 0
fi

# Confirm if aggressive cleanup
if [ "$CLEANUP_LEVEL" = "aggressive" ]; then
    echo -e "${RED}⚠️  WARNING: Aggressive cleanup will remove all caches!${NC}"
    echo "This will require complete rebuilds for future builds."
    read -p "Are you sure you want to continue? (y/N): " confirm
    if [[ ! $confirm =~ ^[Yy]$ ]]; then
        echo "Cleanup cancelled."
        exit 0
    fi
fi

echo -e "${GREEN}🚀 Starting cleanup...${NC}"
echo

# Perform cleanup based on level
case $CLEANUP_LEVEL in
    gentle)
        clean_temp_files
        ;;
    normal)
        clean_temp_files
        clean_build_intermediates
        clean_docker_cache_aware
        clean_build_dirs_cache_aware
        ;;
    aggressive)
        clean_temp_files
        clean_build_intermediates
        clean_docker_aggressive
        clean_build_dirs_aggressive
        ;;
    docker)
        clean_docker_cache_aware
        ;;
    temp)
        clean_temp_files
        ;;
esac

echo
echo -e "${CYAN}📊 Final Disk Usage:${NC}"
echo "  Project size: $(get_disk_usage .)"
if [ -d "build" ]; then
    echo "  Build dir: $(get_disk_usage build)"
fi
if [ -d "Builds" ]; then
    echo "  Builds dir: $(get_disk_usage Builds)"
fi

echo
echo -e "${GREEN}✅ Cleanup completed!${NC}"

# Show cache status
case $CLEANUP_LEVEL in
    gentle|normal)
        echo -e "${BLUE}💾 Cache Status:${NC}"
        echo "  - Docker build cache: Preserved"
        echo "  - GitHub Actions will use cached builds when source unchanged"
        echo "  - Next build will be faster if source hasn't changed"
        ;;
    aggressive)
        echo -e "${YELLOW}⚠️  Cache Status:${NC}"
        echo "  - All caches cleared"
        echo "  - Next build will be slower (full rebuild required)"
        echo "  - Run with --normal for cache-preserving cleanup"
        ;;
esac