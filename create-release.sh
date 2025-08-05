#!/bin/bash

# Automatic Release Creation Script
# This script creates a git tag and pushes it to trigger the GitHub Actions release workflow

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================"
echo "🚀 Pulse24Sync Release Creator"
echo "========================================"
echo -e "${NC}"

# Check if we're in a git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo -e "${RED}❌ Error: Not in a git repository${NC}"
    exit 1
fi

# Check if we have uncommitted changes
if ! git diff-index --quiet HEAD --; then
    echo -e "${YELLOW}⚠️  Warning: You have uncommitted changes${NC}"
    echo "Please commit or stash your changes before creating a release."
    echo
    echo "Current changes:"
    git status --short
    echo
    read -p "Do you want to continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${RED}❌ Release cancelled${NC}"
        exit 1
    fi
fi

# Get the current version from the .jucer file
CURRENT_VERSION=$(grep -o 'version="[^"]*"' Pulse24Sync.jucer | head -1 | cut -d'"' -f2)

if [ -z "$CURRENT_VERSION" ]; then
    echo -e "${RED}❌ Error: Could not find version in Pulse24Sync.jucer${NC}"
    exit 1
fi

echo -e "${GREEN}📋 Current version in .jucer file: ${CURRENT_VERSION}${NC}"

# Ask for the release version
echo
echo -e "${BLUE}Enter the release version (e.g., 1.0.0, 1.2.3-beta.1):${NC}"
read -p "Version: " RELEASE_VERSION

if [ -z "$RELEASE_VERSION" ]; then
    echo -e "${RED}❌ Error: Version cannot be empty${NC}"
    exit 1
fi

# Validate version format (basic check)
if ! [[ $RELEASE_VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+(-[a-zA-Z0-9.-]+)?$ ]]; then
    echo -e "${YELLOW}⚠️  Warning: Version format might not be standard (e.g., 1.0.0 or 1.0.0-beta.1)${NC}"
    read -p "Continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${RED}❌ Release cancelled${NC}"
        exit 1
    fi
fi

# Create the tag name
TAG_NAME="v$RELEASE_VERSION"

echo
echo -e "${BLUE}========================================"
echo "📋 Release Summary"
echo "========================================"
echo -e "${NC}"
echo -e "Version: ${GREEN}$RELEASE_VERSION${NC}"
echo -e "Tag: ${GREEN}$TAG_NAME${NC}"
echo -e "Branch: ${GREEN}$(git branch --show-current)${NC}"
echo

# Confirm the release
read -p "Create release $TAG_NAME? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo -e "${RED}❌ Release cancelled${NC}"
    exit 1
fi

echo
echo -e "${BLUE}🚀 Creating release...${NC}"

# Create and push the tag
echo -e "${YELLOW}📝 Creating git tag...${NC}"
git tag "$TAG_NAME"

echo -e "${YELLOW}📤 Pushing tag to remote...${NC}"
git push origin "$TAG_NAME"

echo
echo -e "${GREEN}✅ Release tag created and pushed!${NC}"
echo
echo -e "${BLUE}📋 What happens next:${NC}"
echo "1. GitHub Actions will automatically build both macOS and Windows versions"
echo "2. A release will be created with all plugin formats"
echo "3. You can monitor progress at: https://github.com/$(git config --get remote.origin.url | sed 's/.*github.com[:/]\([^/]*\/[^/]*\).*/\1/')/actions"
echo
echo -e "${GREEN}🎉 Release will be available at:${NC}"
echo "https://github.com/$(git config --get remote.origin.url | sed 's/.*github.com[:/]\([^/]*\/[^/]*\).*/\1/')/releases/tag/$TAG_NAME"
echo
echo -e "${BLUE}📝 Note: The release workflow will take a few minutes to complete.${NC}" 