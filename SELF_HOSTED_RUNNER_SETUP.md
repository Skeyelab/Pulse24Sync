# Self-Hosted Runner Notarization Setup

This guide is specifically for setting up notarization when using self-hosted GitHub Actions runners.

## ✅ **Good News: Secrets Work the Same**

GitHub secrets are automatically available to self-hosted runners, so the notarization setup will work identically to GitHub-hosted runners.

## 🔧 **Self-Hosted Runner Specific Setup**

### 1. **Certificate Verification**

Your self-hosted runner needs access to the Developer ID certificate. Let's verify it's available:

```bash
# Run this on your self-hosted runner machine
security find-identity -v -p codesigning
```

You should see output like:
```
2) 2E67EC7BC23153A9707EDA307526D5C70780C72E "Developer ID Application: Eric Dahl (5HR8E5CWR7)"
```

### 2. **Keychain Access**

The runner needs access to the keychain containing your certificate. If you're running the runner as a different user or service, you may need to:

```bash
# Check which keychain the certificate is in
security find-certificate -c "Developer ID Application: Eric Dahl" -a

# If needed, add the keychain to the search list
security list-keychains -s login.keychain
```

### 3. **Environment Variables**

The workflow will automatically use the GitHub secrets, but you can also set environment variables directly on the runner if needed:

```bash
# Set these on your runner machine (optional, secrets are preferred)
export CODE_SIGN_IDENTITY="Developer ID Application: Eric Dahl (5HR8E5CWR7)"
export DEVELOPMENT_TEAM="5HR8E5CWR7"
export APPLE_ID="your-apple-id@example.com"
export NOTARIZATION_PASSWORD="your-app-specific-password"
```

## 🚀 **Testing the Setup**

### 1. **Test Certificate Access**

Create a test script to verify the runner can access your certificate:

```bash
#!/bin/bash
# test-certificate-access.sh

echo "Testing certificate access..."

# Check if certificate is available
IDENTITY=$(security find-identity -v -p codesigning | grep "Developer ID Application" | head -1)

if [ -z "$IDENTITY" ]; then
    echo "❌ No Developer ID Application certificate found"
    exit 1
else
    echo "✅ Certificate found: $IDENTITY"
fi

# Test code signing
echo "Testing code signing..."
echo "test" > test.txt
codesign --sign "Developer ID Application: Eric Dahl (5HR8E5CWR7)" test.txt

if [ $? -eq 0 ]; then
    echo "✅ Code signing test passed"
    rm test.txt
else
    echo "❌ Code signing test failed"
    rm test.txt
    exit 1
fi
```

### 2. **Test Notarization Credentials**

Test if the notarization credentials work:

```bash
#!/bin/bash
# test-notarization.sh

echo "Testing notarization credentials..."

# Check if required variables are set
if [ -z "$APPLE_ID" ] || [ -z "$NOTARIZATION_PASSWORD" ] || [ -z "$DEVELOPMENT_TEAM" ]; then
    echo "❌ Missing required environment variables"
    echo "Set APPLE_ID, NOTARIZATION_PASSWORD, and DEVELOPMENT_TEAM"
    exit 1
fi

echo "✅ Notarization credentials available"

# Test notarytool access
echo "Testing notarytool..."
xcrun notarytool info --apple-id "$APPLE_ID" --password "$NOTARIZATION_PASSWORD" --team-id "$DEVELOPMENT_TEAM"

if [ $? -eq 0 ]; then
    echo "✅ Notarization credentials work"
else
    echo "❌ Notarization credentials failed"
    exit 1
fi
```

## 🔍 **Workflow Verification**

After setting up the secrets, your workflow should show:

### Successful Code Signing:
```
🔐 Checking code signing configuration...
  🔍 Auto-detected signing identity: Developer ID Application: Eric Dahl (5HR8E5CWR7)
  ✅ Found entitlements.plist
  - Signing VST3 plugin...
  🔏 Code signing Pulse24Sync.vst3...
  ✅ Successfully signed Pulse24Sync.vst3
```

### Successful Notarization:
```
🍎 Starting notarization process...
  ✅ Notarization credentials available
  - Notarizing AU component...
  📦 Creating zip for notarization: Pulse24Sync.component
  🚀 Submitting Pulse24Sync.component for notarization...
  ✅ Notarization completed for Pulse24Sync.component (ID: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx)
  📎 Stapling notarization ticket to Pulse24Sync.component...
  ✅ Successfully stapled notarization to Pulse24Sync.component
  🔍 Verifying notarization...
  ✅ Notarization verification passed for Pulse24Sync.component
```

## 🛠️ **Troubleshooting Self-Hosted Runners**

### Problem: "No Developer ID Application certificate found"

**Solutions:**
1. **Check certificate installation**: Run `security find-identity -v -p codesigning`
2. **Check keychain access**: Ensure the runner user can access the keychain
3. **Install certificate for runner user**: If running as a service, install the certificate for that user

### Problem: "Code signing failed"

**Solutions:**
1. **Check user permissions**: Ensure the runner user has access to the certificate
2. **Check keychain unlock**: The keychain may need to be unlocked
3. **Test manually**: Try signing a test file manually

### Problem: "Notarization credentials failed"

**Solutions:**
1. **Verify secrets**: Check that all 4 secrets are set in GitHub
2. **Test credentials**: Use the test script above
3. **Check Apple ID**: Ensure it's the same one associated with your Developer account

## 📋 **Self-Hosted Runner Best Practices**

1. **Use GitHub Secrets**: Don't hardcode credentials in runner configuration
2. **Test locally first**: Run the test scripts on your runner machine
3. **Monitor logs**: Check runner logs for detailed error messages
4. **Keep certificates updated**: Ensure certificates don't expire
5. **Secure the runner**: Ensure only authorized users can access the runner

## 🎯 **Quick Setup Checklist**

- [ ] Developer ID certificate installed and accessible
- [ ] GitHub secrets configured (CODE_SIGN_IDENTITY, DEVELOPMENT_TEAM, APPLE_ID, NOTARIZATION_PASSWORD)
- [ ] Keychain accessible to runner user
- [ ] Test scripts pass
- [ ] Workflow runs successfully with notarization

Your self-hosted runner will now produce fully notarized plugins that macOS trusts without security warnings!
