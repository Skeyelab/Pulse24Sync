#!/bin/bash

# GitHub Release Creation Helper Script
# This script helps create a GitHub release manually

VERSION="1.0.0-beta.1"
REPO="Skeyelab/Pulse24Sync"

echo "========================================"
echo "GitHub Release Creation Helper"
echo "Version: $VERSION"
echo "Repository: $REPO"
echo "========================================"
echo

echo "📋 Steps to create GitHub release:"
echo
echo "1. Go to: https://github.com/$REPO/releases"
echo "2. Click 'Create a new release'"
echo "3. Set tag version to: v$VERSION"
echo "4. Set release title to: Pulse24Sync v$VERSION"
echo "5. Copy the release notes below:"
echo
echo "========================================"
echo "RELEASE NOTES TO COPY:"
echo "========================================"

cat "releases/v$VERSION/release-notes-v$VERSION.md"

echo
echo "========================================"
echo "ASSETS TO UPLOAD:"
echo "========================================"
echo "Upload these files as release assets:"
echo

for file in releases/v$VERSION/*.zip; do
    if [ -f "$file" ]; then
        filename=$(basename "$file")
        filesize=$(ls -lh "$file" | awk '{print $5}')
        echo "📦 $filename ($filesize)"
    fi
done

echo
echo "========================================"
echo "RELEASE SETTINGS:"
echo "========================================"
echo "✅ Set as: Pre-release (since this is beta)"
echo "✅ Target: main branch"
echo "✅ Auto-generate release notes: No (use our custom notes)"
echo
echo "🎉 After creating the release, users can download from:"
echo "https://github.com/$REPO/releases/tag/v$VERSION"
echo
echo "📝 The release will be available at:"
echo "https://github.com/$REPO/releases/latest"
