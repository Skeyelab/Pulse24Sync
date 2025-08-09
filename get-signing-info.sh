#!/bin/bash

# Pulse24Sync - Get Signing Information
# This script helps you get the information needed for GitHub secrets

echo "========================================"
echo "Pulse24Sync - Signing Information Helper"
echo "========================================"
echo ""

echo "🔍 Looking for available code signing certificates..."
echo ""

# Find Developer ID Application certificates
IDENTITIES=$(security find-identity -v -p codesigning | grep "Developer ID Application")

if [ -z "$IDENTITIES" ]; then
    echo "❌ No Developer ID Application certificates found"
    echo ""
    echo "To fix this:"
    echo "1. Make sure you're enrolled in the Apple Developer Program"
    echo "2. Download and install your Developer ID Application certificate"
    echo "3. Run this script again"
    exit 1
fi

echo "✅ Found Developer ID Application certificates:"
echo ""

# Display all found certificates
echo "$IDENTITIES" | while IFS= read -r line; do
    echo "  $line"
done

echo ""
echo "========================================"
echo "GitHub Secrets Setup"
echo "========================================"
echo ""

# Extract the first (or most relevant) certificate
FIRST_IDENTITY=$(echo "$IDENTITIES" | head -1)

if [ ! -z "$FIRST_IDENTITY" ]; then
    # Extract the identity name
    IDENTITY_NAME=$(echo "$FIRST_IDENTITY" | sed -n 's/.*"\(.*\)"/\1/p')

    # Extract the team ID (the part in parentheses)
    TEAM_ID=$(echo "$IDENTITY_NAME" | sed -n 's/.*(\([^)]*\))/\1/p')

    echo "📋 Use these values for your GitHub repository secrets:"
    echo ""
    echo "1. CODE_SIGN_IDENTITY:"
    echo "   $IDENTITY_NAME"
    echo ""
    echo "2. DEVELOPMENT_TEAM:"
    echo "   $TEAM_ID"
    echo ""
    echo "3. APPLE_ID:"
    echo "   [Your Apple ID email address]"
    echo ""
    echo "4. NOTARIZATION_PASSWORD:"
    echo "   [App-specific password from https://appleid.apple.com/]"
    echo ""
    echo "========================================"
    echo "Setup Instructions:"
    echo "========================================"
    echo ""
    echo "1. Go to your GitHub repository"
    echo "2. Click Settings → Secrets and variables → Actions"
    echo "3. Click 'New repository secret' for each of the above"
    echo "4. Copy and paste the values exactly as shown"
    echo ""
    echo "For the app-specific password:"
    echo "1. Go to https://appleid.apple.com/"
    echo "2. Sign in with your Apple ID"
    echo "3. Click 'Sign-in and Security' → 'App-Specific Passwords'"
    echo "4. Click 'Generate Password'"
    echo "5. Name it 'Pulse24Sync Notarization'"
    echo "6. Copy the generated password"
    echo ""
    echo "After setting up the secrets, push your changes to trigger a new build!"
else
    echo "❌ Could not extract certificate information"
    echo "Please check your certificate installation"
fi

echo ""
echo "========================================"
echo "Verification"
echo "========================================"
echo ""
echo "After setting up the secrets, you can verify the setup by:"
echo "1. Pushing changes to trigger a new GitHub Actions build"
echo "2. Checking the workflow logs for the 'Notarize macOS Plugin' step"
echo "3. Looking for success messages like '✅ Notarization completed'"
echo ""
echo "For more details, see NOTARIZATION_SETUP.md"
