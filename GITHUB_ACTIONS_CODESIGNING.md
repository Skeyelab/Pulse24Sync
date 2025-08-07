# GitHub Actions Code Signing Setup

This guide explains how to set up code signing for your macOS builds in GitHub Actions, so they match your local builds.

## 🎯 Problem Solved

Previously, your macOS apps were codesigned when built locally but not when built via GitHub Actions. This has been fixed by adding the same codesigning logic to both workflows.

## ✅ What's Been Updated

1. **`.github/workflows/build.yml`** - Added codesigning step after build
2. **`.github/workflows/release.yml`** - Added codesigning step after build
3. Both workflows now use the same codesigning logic as your local `build_macos.sh` script

## 🔑 Setting Up Code Signing for GitHub Actions

### Option 1: Use GitHub Secrets (Recommended)

1. **Get your Developer ID certificate identity**:
   ```bash
   security find-identity -v -p codesigning
   ```
   Look for a line like: `"Developer ID Application: Your Name (TEAMID)"`

2. **Add GitHub repository secrets**:
   - Go to your GitHub repository
   - Click **Settings** → **Secrets and variables** → **Actions**
   - Click **New repository secret**
   - Add these secrets:
     - `CODE_SIGN_IDENTITY`: Your full signing identity (e.g., "Developer ID Application: Your Name (TEAMID)")
     - `DEVELOPMENT_TEAM`: Your Apple Developer Team ID
     - `APPLE_ID`: Your Apple ID email (for notarization)
     - `NOTARIZATION_PASSWORD`: App-specific password for notarization

3. **Update the workflows to use secrets** (optional):
   ```yaml
   env:
     CODE_SIGN_IDENTITY: ${{ secrets.CODE_SIGN_IDENTITY }}
     DEVELOPMENT_TEAM: ${{ secrets.DEVELOPMENT_TEAM }}
   ```

### Option 2: Auto-Detection (Current Setup)

The current workflow will automatically detect available certificates on the GitHub Actions runner. However, this depends on:

1. **Runner having certificates installed**: Most GitHub Actions macOS runners don't have Developer ID certificates pre-installed
2. **Certificate availability**: The runner needs access to your private key

## 🔧 Manual Certificate Installation on Runner

If you need to install certificates on the runner, you can add this step to your workflow:

```yaml
- name: Install Code Signing Certificate
  if: matrix.os == 'macOS'
  run: |
    # Create a temporary keychain
    security create-keychain -p "temp" temp.keychain
    security list-keychains -s temp.keychain
    security default-keychain -s temp.keychain
    security unlock-keychain -p "temp" temp.keychain
    
    # Import your certificate (you'll need to provide the certificate data)
    echo "${{ secrets.CERTIFICATE_DATA }}" | base64 -d > certificate.p12
    security import certificate.p12 -k temp.keychain -P "${{ secrets.CERTIFICATE_PASSWORD }}" -T /usr/bin/codesign
    
    # Set keychain search list
    security set-key-partition-list -S apple-tool:,apple:,codesign: -s -k "temp" temp.keychain
```

## 🚀 Testing the Setup

1. **Push your changes** to trigger a workflow run
2. **Check the workflow logs** for the "Code Sign macOS Plugin" step
3. **Verify the output** shows successful signing or appropriate warnings

## 📋 Expected Workflow Output

### Successful Code Signing:
```
🔐 Checking code signing configuration...
  🔍 Auto-detected signing identity: Developer ID Application: Your Name (TEAMID)
  ✅ Found entitlements.plist
  - Signing VST3 plugin...
  🔏 Code signing Pulse24Sync.vst3...
  ✅ Successfully signed Pulse24Sync.vst3
  🔍 Verifying signature...
  ✅ Signature verification passed
```

### No Certificate Available:
```
🔐 Checking code signing configuration...
  ⚠️  No Developer ID Application certificate found
  ℹ️  Plugins will be built unsigned (may trigger security warnings)
  ✅ Found entitlements.plist
  - Signing VST3 plugin...
  ⚠️  Skipping code signing for Pulse24Sync.vst3 (no signing identity set)
```

## 🔍 Verification Commands

After a successful build, you can verify the signatures:

```bash
# Check VST3 signature
codesign -dv --verbose=4 Pulse24Sync.vst3

# Check AU component signature  
codesign -dv --verbose=4 Pulse24Sync.component

# Check standalone app signature
codesign -dv --verbose=4 Pulse24Sync.app

# Test Gatekeeper acceptance
spctl -a -v Pulse24Sync.component
```

## 🛠️ Troubleshooting

### Problem: "No Developer ID Application certificate found"

**Solutions:**
1. **Use GitHub Secrets**: Set up `CODE_SIGN_IDENTITY` secret as described above
2. **Install certificate on runner**: Use the manual installation method
3. **Use self-signed certificate**: For testing purposes only

### Problem: "Code signing failed"

**Solutions:**
1. Check certificate expiration
2. Verify private key is available
3. Ensure entitlements.plist exists and is valid

### Problem: "entitlements.plist not found"

**Solution:**
The `entitlements.plist` file should be in your repository root. If missing, it will be created automatically by the build process.

## 📞 Getting Help

If you encounter issues:

1. **Check workflow logs**: Look for detailed error messages in the "Code Sign macOS Plugin" step
2. **Verify certificate status**: Ensure your Apple Developer account is active
3. **Test locally first**: Make sure codesigning works with your local `build_macos.sh` script
4. **Check GitHub Actions documentation**: For more information about secrets and environment variables

## 🎉 Success Indicators

After following this guide, you should see:

- ✅ GitHub Actions builds produce codesigned artifacts
- ✅ Same signing behavior as local builds
- ✅ No security warnings when installing plugins from GitHub Actions builds
- ✅ Consistent build output between local and CI environments

Your GitHub Actions builds will now match your local builds in terms of code signing!