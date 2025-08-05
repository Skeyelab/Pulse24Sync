#!/bin/bash

# GitHub API Release Creation Script
# This script creates a GitHub release using the GitHub API

VERSION="1.0.0-beta.1"
REPO="Skeyelab/Pulse24Sync"
GITHUB_TOKEN="${GITHUB_TOKEN:-$1}"

if [ -z "$GITHUB_TOKEN" ]; then
    echo "Error: GitHub token required"
    echo "Usage: $0 <github_token>"
    echo "Or set GITHUB_TOKEN environment variable"
    exit 1
fi

echo "Creating GitHub release v$VERSION for $REPO..."

# Create the release
RELEASE_RESPONSE=$(curl -s -X POST \
    -H "Authorization: token $GITHUB_TOKEN" \
    -H "Accept: application/vnd.github.v3+json" \
    "https://api.github.com/repos/$REPO/releases" \
    -d "{
        \"tag_name\": \"v$VERSION\",
        \"name\": \"Pulse24Sync v$VERSION\",
        \"body\": \"## Pulse24Sync v$VERSION\n\n### 🧪 Beta Release\nThis is a pre-release version for testing and feedback.\n\n### ⚠️ Important Notes\n- This is a beta version - may contain bugs\n- Please report any issues on GitHub\n- Not recommended for production use\n\n### 🎉 What's New\n- **Complete VST3 and AU plugin implementation**\n- **24 pulses per quarter note with tempo synchronization**\n- **Cross-platform support (macOS and Windows)**\n- **Modern dark theme UI with parameter automation**\n- **Inspired by community request from Zac Caruso**\n\n### 🔧 Features\n- **Tempo Sync**: Automatic synchronization with host tempo\n- **Pulse Generation**: Exactly 24 pulses per quarter note\n- **Parameter Controls**: Volume, pulse width, and enable/disable\n- **Real-time Processing**: Zero latency audio processing\n- **Cross-platform**: macOS (AU/VST3) and Windows (VST3)\n\n### 🚀 Installation\n1. Download the appropriate plugin for your platform\n2. Extract the archive\n3. Copy to your DAW's plugin directory:\n   - **macOS AU**: \`/Library/Audio/Plug-Ins/Components/\`\n   - **macOS VST3**: \`/Library/Audio/Plug-Ins/VST3/\`\n4. Restart your DAW and scan for new plugins\n\n### 📋 System Requirements\n- **macOS**: 10.13+ (Intel/Apple Silicon)\n- **DAW**: Any VST3 or AU compatible host\n\n### 🙏 Acknowledgments\nInspired by a request from Zac Caruso in the Synthesizer Zone Facebook group.\n\n### 📝 Feedback\nPlease report bugs and provide feedback on GitHub Issues.\n\n### 🔮 Coming Soon\n- Windows VST3 builds\n- Additional pulse patterns\n- MIDI output support\n- Advanced tempo sync options\",
        \"draft\": false,
        \"prerelease\": true
    }")

# Check if release was created successfully
if echo "$RELEASE_RESPONSE" | grep -q '"id"'; then
    RELEASE_ID=$(echo "$RELEASE_RESPONSE" | grep '"id"' | head -1 | sed 's/.*"id": \([0-9]*\).*/\1/')
    echo "✅ Release created successfully! ID: $RELEASE_ID"

    # Upload assets
    for file in releases/v$VERSION/*.zip; do
        if [ -f "$file" ]; then
            filename=$(basename "$file")
            echo "📦 Uploading $filename..."

            UPLOAD_RESPONSE=$(curl -s -X POST \
                -H "Authorization: token $GITHUB_TOKEN" \
                -H "Accept: application/vnd.github.v3+json" \
                -H "Content-Type: application/zip" \
                --data-binary "@$file" \
                "https://uploads.github.com/repos/$REPO/releases/$RELEASE_ID/assets?name=$filename")

            if echo "$UPLOAD_RESPONSE" | grep -q '"id"'; then
                echo "✅ $filename uploaded successfully!"
            else
                echo "❌ Failed to upload $filename"
                echo "Response: $UPLOAD_RESPONSE"
            fi
        fi
    done

    echo
    echo "🎉 Release v$VERSION created successfully!"
    echo "📝 View at: https://github.com/$REPO/releases/tag/v$VERSION"
    echo "📦 Download at: https://github.com/$REPO/releases/latest"

else
    echo "❌ Failed to create release"
    echo "Response: $RELEASE_RESPONSE"
    exit 1
fi
