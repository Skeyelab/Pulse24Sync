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

# Function to parse semantic version into components
parse_version() {
    local version=$1
    # Remove 'v' prefix if present
    version=${version#v}
    
    # Extract main version and pre-release parts
    if [[ $version =~ ^([0-9]+)\.([0-9]+)\.([0-9]+)(-(.+))?$ ]]; then
        MAJOR=${BASH_REMATCH[1]}
        MINOR=${BASH_REMATCH[2]}
        PATCH=${BASH_REMATCH[3]}
        PRERELEASE=${BASH_REMATCH[5]}
        echo "MAJOR=$MAJOR MINOR=$MINOR PATCH=$PATCH PRERELEASE='$PRERELEASE'"
        return 0
    else
        return 1
    fi
}

# Function to get current release version from git tags
get_current_release() {
    local latest_tag
    latest_tag=$(git describe --tags --abbrev=0 2>/dev/null || echo "")
    
    if [ -z "$latest_tag" ]; then
        echo ""
        return 1
    else
        echo "$latest_tag"
        return 0
    fi
}

# Function to suggest next version numbers
suggest_next_versions() {
    local current_version=$1
    
    if [ -z "$current_version" ]; then
        echo "No previous releases found. Suggested starting version: 1.0.0"
        echo "Suggestions:"
        echo "  1.0.0       - First stable release"
        echo "  1.0.0-beta.1 - First beta release"
        echo "  0.1.0       - Initial development release"
        return
    fi
    
    # Parse current version
    local version_info
    version_info=$(parse_version "$current_version")
    if [ $? -ne 0 ]; then
        echo "Could not parse current version: $current_version"
        return 1
    fi
    
    eval "$version_info"
    
    echo "Current release: $current_version"
    echo
    echo "Suggested next versions:"
    
    if [ -n "$PRERELEASE" ]; then
        # Current version is a pre-release
        if [[ $PRERELEASE =~ ^(alpha|beta|rc)\.([0-9]+)$ ]]; then
            local pre_type=${BASH_REMATCH[1]}
            local pre_num=${BASH_REMATCH[2]}
            local next_pre_num=$((pre_num + 1))
            
            echo "  $MAJOR.$MINOR.$PATCH-$pre_type.$next_pre_num  - Next $pre_type release"
            
            # Suggest progression through pre-release types
            case $pre_type in
                "alpha")
                    echo "  $MAJOR.$MINOR.$PATCH-beta.1      - First beta release"
                    ;;
                "beta")
                    echo "  $MAJOR.$MINOR.$PATCH-rc.1        - First release candidate"
                    ;;
                "rc")
                    echo "  $MAJOR.$MINOR.$PATCH             - Stable release"
                    ;;
            esac
        else
            # Generic pre-release increment
            echo "  $MAJOR.$MINOR.$PATCH-${PRERELEASE}.1 - Next pre-release"
        fi
        echo "  $MAJOR.$MINOR.$PATCH             - Stable release"
    else
        # Current version is stable
        echo "  $MAJOR.$MINOR.$((PATCH + 1))             - Patch release (bug fixes)"
        echo "  $MAJOR.$((MINOR + 1)).0             - Minor release (new features)"
        echo "  $((MAJOR + 1)).0.0             - Major release (breaking changes)"
        echo "  $MAJOR.$((MINOR + 1)).0-beta.1     - Next minor beta"
        echo "  $((MAJOR + 1)).0.0-beta.1     - Next major beta"
    fi
    echo
}

# Function to validate version format
validate_version() {
    local version=$1
    if [[ $version =~ ^[0-9]+\.[0-9]+\.[0-9]+(-[a-zA-Z0-9.-]+)?$ ]]; then
        return 0
    else
        return 1
    fi
}

# Function to compare versions (returns 0 if v1 >= v2, 1 if v1 < v2)
compare_versions() {
    local v1=$1
    local v2=$2
    
    # Remove 'v' prefix if present
    v1=${v1#v}
    v2=${v2#v}
    
    # Handle special case: stable version vs pre-release of same version
    # e.g., 1.0.0 > 1.0.0-beta.1
    local v1_base v2_base
    v1_base=$(echo "$v1" | cut -d'-' -f1)
    v2_base=$(echo "$v2" | cut -d'-' -f1)
    
    if [[ "$v1_base" == "$v2_base" ]]; then
        # Same base version
        if [[ "$v1" != *"-"* && "$v2" == *"-"* ]]; then
            return 0  # v1 (stable) >= v2 (pre-release)
        elif [[ "$v1" == *"-"* && "$v2" != *"-"* ]]; then
            return 1  # v1 (pre-release) < v2 (stable)
        fi
    fi
    
    # Use sort with version comparison for normal cases
    if [[ "$(printf '%s\n' "$v1" "$v2" | sort -V | head -n1)" == "$v2" ]]; then
        return 0  # v1 >= v2
    else
        return 1  # v1 < v2
    fi
}

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

# Get current release from git tags and suggest next versions
echo
echo -e "${BLUE}🔍 Checking current releases...${NC}"
CURRENT_RELEASE=$(get_current_release)

if [ $? -eq 0 ] && [ -n "$CURRENT_RELEASE" ]; then
    echo -e "${GREEN}📋 Latest git release tag: ${CURRENT_RELEASE}${NC}"
else
    echo -e "${YELLOW}📋 No previous release tags found${NC}"
fi

echo
echo -e "${BLUE}💡 Version suggestions:${NC}"
suggest_next_versions "$CURRENT_RELEASE"

# Ask for the release version
echo -e "${BLUE}Enter the release version (e.g., 1.0.0, 1.2.3-beta.1):${NC}"
read -p "Version: " RELEASE_VERSION

if [ -z "$RELEASE_VERSION" ]; then
    echo -e "${RED}❌ Error: Version cannot be empty${NC}"
    exit 1
fi

# Validate version format
if ! validate_version "$RELEASE_VERSION"; then
    echo -e "${YELLOW}⚠️  Warning: Version format might not be standard (e.g., 1.0.0 or 1.0.0-beta.1)${NC}"
    read -p "Continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${RED}❌ Release cancelled${NC}"
        exit 1
    fi
fi

# Check if version already exists
TAG_NAME="v$RELEASE_VERSION"
if git rev-parse "$TAG_NAME" >/dev/null 2>&1; then
    echo -e "${RED}❌ Error: Tag $TAG_NAME already exists${NC}"
    echo "Use 'git tag -d $TAG_NAME' to delete it locally if needed"
    exit 1
fi

# Compare with current release to ensure we're moving forward
if [ -n "$CURRENT_RELEASE" ]; then
    if ! compare_versions "$RELEASE_VERSION" "${CURRENT_RELEASE#v}"; then
        echo -e "${YELLOW}⚠️  Warning: New version ($RELEASE_VERSION) appears to be older than current release (${CURRENT_RELEASE#v})${NC}"
        read -p "Continue anyway? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            echo -e "${RED}❌ Release cancelled${NC}"
            exit 1
        fi
    fi
fi

echo
echo -e "${BLUE}========================================"
echo "📋 Release Summary"
echo "========================================"
echo -e "${NC}"
echo -e "Previous release: ${GREEN}${CURRENT_RELEASE:-"None"}${NC}"
echo -e "New version: ${GREEN}$RELEASE_VERSION${NC}"
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
