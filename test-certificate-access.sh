#!/bin/bash

# Pulse24Sync - Certificate Access Test
# This script tests if the self-hosted runner can access the Developer ID certificate

echo "========================================"
echo "Pulse24Sync - Certificate Access Test"
echo "========================================"
echo ""

echo "🔍 Testing certificate access..."

# Check if certificate is available
IDENTITY=$(security find-identity -v -p codesigning | grep "Developer ID Application" | head -1)

if [ -z "$IDENTITY" ]; then
    echo "❌ No Developer ID Application certificate found"
    echo ""
    echo "Troubleshooting:"
    echo "1. Check if certificate is installed: security find-identity -v -p codesigning"
    echo "2. Check keychain access: security list-keychains"
    echo "3. Install certificate for the runner user if needed"
    exit 1
else
    echo "✅ Certificate found: $IDENTITY"
fi

# Extract the identity name for testing
IDENTITY_NAME=$(echo "$IDENTITY" | sed -n 's/.*"\(.*\)"/\1/p')

echo ""
echo "🔏 Testing code signing..."

# Create a test file
echo "test" > test.txt

# Test code signing
codesign --sign "$IDENTITY_NAME" test.txt

if [ $? -eq 0 ]; then
    echo "✅ Code signing test passed"

    # Verify the signature
    echo "🔍 Verifying signature..."
    codesign --verify test.txt

    if [ $? -eq 0 ]; then
        echo "✅ Signature verification passed"
    else
        echo "⚠️  Signature verification failed"
    fi

    # Clean up
    rm test.txt
else
    echo "❌ Code signing test failed"
    rm test.txt
    exit 1
fi

echo ""
echo "========================================"
echo "Certificate Access Test: PASSED ✅"
echo "========================================"
echo ""
echo "Your self-hosted runner can access the Developer ID certificate"
echo "and perform code signing operations."
echo ""
echo "Next step: Set up GitHub secrets for notarization"
