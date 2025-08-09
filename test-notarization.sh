#!/bin/bash

# Pulse24Sync - Notarization Credentials Test
# This script tests if the notarization credentials work

echo "========================================"
echo "Pulse24Sync - Notarization Credentials Test"
echo "========================================"
echo ""

echo "🔍 Testing notarization credentials..."

# Check if required variables are set
if [ -z "$APPLE_ID" ] || [ -z "$NOTARIZATION_PASSWORD" ] || [ -z "$DEVELOPMENT_TEAM" ]; then
    echo "❌ Missing required environment variables"
    echo ""
    echo "Required variables:"
    echo "  APPLE_ID: $APPLE_ID"
    echo "  NOTARIZATION_PASSWORD: [set/not set]"
    echo "  DEVELOPMENT_TEAM: $DEVELOPMENT_TEAM"
    echo ""
    echo "To set these variables:"
    echo "1. Add them as GitHub secrets in your repository"
    echo "2. Or set them locally:"
    echo "   export APPLE_ID='your-apple-id@example.com'"
    echo "   export NOTARIZATION_PASSWORD='your-app-specific-password'"
    echo "   export DEVELOPMENT_TEAM='5HR8E5CWR7'"
    echo ""
    echo "Then run this script again."
    exit 1
fi

echo "✅ Notarization credentials available"
echo "  APPLE_ID: $APPLE_ID"
echo "  DEVELOPMENT_TEAM: $DEVELOPMENT_TEAM"
echo "  NOTARIZATION_PASSWORD: [set]"

echo ""
echo "🚀 Testing notarytool access..."

# Test notarytool access
xcrun notarytool info --apple-id "$APPLE_ID" --password "$NOTARIZATION_PASSWORD" --team-id "$DEVELOPMENT_TEAM"

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Notarization credentials work"
    echo ""
    echo "========================================"
    echo "Notarization Credentials Test: PASSED ✅"
    echo "========================================"
    echo ""
    echo "Your notarization credentials are working correctly."
    echo "The GitHub Actions workflow should be able to notarize your plugins."
    echo ""
    echo "Next step: Push changes to trigger a workflow run"
else
    echo ""
    echo "❌ Notarization credentials failed"
    echo ""
    echo "Troubleshooting:"
    echo "1. Check your Apple ID is correct"
    echo "2. Verify the app-specific password is valid"
    echo "3. Ensure your Apple Developer account is active"
    echo "4. Check that the team ID matches your Developer account"
    echo ""
    echo "To generate a new app-specific password:"
    echo "1. Go to https://appleid.apple.com/"
    echo "2. Sign in with your Apple ID"
    echo "3. Click 'Sign-in and Security' → 'App-Specific Passwords'"
    echo "4. Generate a new password named 'Pulse24Sync Notarization'"
    exit 1
fi
