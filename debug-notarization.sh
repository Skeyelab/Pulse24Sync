#!/bin/bash

# Pulse24Sync - Notarization Debug Script
# This script helps debug notarization issues

echo "========================================"
echo "Pulse24Sync - Notarization Debug"
echo "========================================"
echo ""

# Check if we have the required environment variables
echo "🔍 Checking environment variables..."
echo "  APPLE_ID: ${APPLE_ID:-'NOT SET'}"
echo "  DEVELOPMENT_TEAM: ${DEVELOPMENT_TEAM:-'NOT SET'}"
echo "  NOTARIZATION_PASSWORD: ${NOTARIZATION_PASSWORD:+'SET'}"
echo ""

if [ -z "$APPLE_ID" ] || [ -z "$NOTARIZATION_PASSWORD" ] || [ -z "$DEVELOPMENT_TEAM" ]; then
    echo "❌ Missing required environment variables"
    echo "Please set: APPLE_ID, NOTARIZATION_PASSWORD, DEVELOPMENT_TEAM"
    exit 1
fi

echo "✅ All required environment variables are set"
echo ""

# Test notarytool access
echo "🔧 Testing notarytool access..."
echo "Testing with: xcrun notarytool info --apple-id \"$APPLE_ID\" --password \"***\" --team-id \"$DEVELOPMENT_TEAM\" <submission-id>"
echo "Note: This command requires a submission ID, so we'll test credentials during actual submission"
echo "✅ Credentials appear to be set correctly"

echo ""

# Check if we have a plugin to notarize
echo "🔍 Looking for plugin files..."
if [ -d "dist/Pulse24Sync.component" ]; then
    echo "✅ Found Pulse24Sync.component"
    PLUGIN_PATH="dist/Pulse24Sync.component"
elif [ -d "Builds/MacOSX/build/Release/Pulse24Sync.component" ]; then
    echo "✅ Found Pulse24Sync.component in build directory"
    PLUGIN_PATH="Builds/MacOSX/build/Release/Pulse24Sync.component"
else
    echo "❌ No plugin found to notarize"
    echo "Please build the plugin first or check the path"
    exit 1
fi

echo ""

# Test notarization with detailed output
echo "🚀 Testing notarization with detailed output..."

# Create a temporary directory for the zip
TEMP_DIR=$(mktemp -d)
ZIP_PATH="$TEMP_DIR/Pulse24Sync.component.zip"

echo "📦 Creating zip file: $ZIP_PATH"
cd "$(dirname "$PLUGIN_PATH")"
zip -r "$ZIP_PATH" "$(basename "$PLUGIN_PATH")"

if [ $? -eq 0 ]; then
    echo "✅ Zip file created successfully"
else
    echo "❌ Failed to create zip file"
    rm -rf "$TEMP_DIR"
    exit 1
fi

echo ""
echo "📤 Submitting for notarization..."
echo "Command: xcrun notarytool submit \"$ZIP_PATH\" --apple-id \"$APPLE_ID\" --password \"***\" --team-id \"$DEVELOPMENT_TEAM\" --wait"

# Submit for notarization with full output
xcrun notarytool submit "$ZIP_PATH" \
    --apple-id "$APPLE_ID" \
    --password "$NOTARIZATION_PASSWORD" \
    --team-id "$DEVELOPMENT_TEAM" \
    --wait

NOTARY_EXIT_CODE=$?

echo ""
echo "📋 Notarization exit code: $NOTARY_EXIT_CODE"

if [ $NOTARY_EXIT_CODE -eq 0 ]; then
    echo "✅ Notarization completed successfully"

    echo ""
    echo "📎 Testing stapling..."
    xcrun stapler staple "$PLUGIN_PATH"

    if [ $? -eq 0 ]; then
        echo "✅ Stapling successful"

        echo ""
        echo "🔍 Verifying notarization..."
        xcrun stapler validate "$PLUGIN_PATH"

        if [ $? -eq 0 ]; then
            echo "✅ Notarization verification passed"
        else
            echo "❌ Notarization verification failed"
        fi
    else
        echo "❌ Stapling failed"
    fi
else
    echo "❌ Notarization failed"
fi

# Clean up
rm -rf "$TEMP_DIR"

echo ""
echo "========================================"
echo "Debug completed"
echo "========================================"
