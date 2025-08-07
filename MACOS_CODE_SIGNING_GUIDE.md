# macOS Code Signing Guide for Pulse24Sync

This guide will help you set up proper code signing for your Pulse24Sync audio plugin to eliminate the "Apple could not verify is free of malware" warning.

## 🎯 Quick Fix Summary

The error occurs because your plugin isn't code-signed with a valid Apple Developer certificate. Here's what we've set up:

1. ✅ **Updated JUCE project** with code signing configuration
2. ✅ **Created entitlements.plist** with required permissions
3. ✅ **Enhanced build script** with automatic signing
4. ✅ **Added certificate detection** and verification

## 🔑 Prerequisites

### Apple Developer Account Setup

1. **Enroll in Apple Developer Program**
   - Go to https://developer.apple.com/programs/
   - Cost: $99/year
   - Required for distributing signed apps/plugins

2. **Download Developer ID Certificate**
   - Log into https://developer.apple.com/account/
   - Go to "Certificates, Identifiers & Profiles"
   - Click "+" to create new certificate
   - Select "Developer ID Application"
   - Follow the instructions to generate and download

3. **Install Certificate**
   - Double-click the downloaded certificate (.cer file)
   - It will be added to your macOS Keychain

## 🛠️ Setup Instructions

### Step 1: Verify Your Certificate

Check if your certificate is properly installed:

```bash
security find-identity -v -p codesigning
```

You should see something like:
```
1) ABC123DEF456 "Developer ID Application: Your Name (TEAMID)"
```

### Step 2: Set Your Signing Identity

You have two options:

**Option A: Let the script auto-detect (recommended)**
```bash
./build_macos.sh
```

**Option B: Set it manually**
```bash
export CODE_SIGN_IDENTITY="Developer ID Application: Your Name (TEAMID)"
./build_macos.sh
```

### Step 3: Build with Signing

Run the build script:
```bash
./build_macos.sh
```

The script will:
- Auto-detect your developer certificate
- Build the plugin
- Sign all components (VST3, AU, Standalone)
- Verify signatures
- Create a distribution-ready package

## 🔍 Verification

After building, verify your plugin is properly signed:

```bash
# Check VST3 signature
codesign -dv --verbose=4 dist/Pulse24Sync.vst3

# Check AU component signature  
codesign -dv --verbose=4 dist/Pulse24Sync.component

# Check standalone app signature
codesign -dv --verbose=4 dist/Pulse24Sync.app
```

You should see output indicating proper signing with your Developer ID.

## 🚀 Distribution Options

### Option 1: Local Installation (Basic)

For personal use or testing:

```bash
# Install VST3
cp -R dist/Pulse24Sync.vst3 ~/Library/Audio/Plug-Ins/VST3/

# Install AU Component
cp -R dist/Pulse24Sync.component ~/Library/Audio/Plug-Ins/Components/
```

### Option 2: Notarization (Recommended for Distribution)

For distributing to other users:

1. **Create app-specific password**
   - Go to https://appleid.apple.com/
   - Sign in with your Apple ID
   - Generate app-specific password
   - Save it securely

2. **Notarize your plugin**
   ```bash
   # Create a zip for notarization
   cd dist
   zip -r Pulse24Sync.zip Pulse24Sync.component Pulse24Sync.vst3 Pulse24Sync.app
   
   # Submit for notarization
   xcrun notarytool submit Pulse24Sync.zip \
     --apple-id your-apple-id@example.com \
     --password your-app-specific-password \
     --team-id YOUR-TEAM-ID \
     --wait
   
   # Staple the notarization (after approval)
   xcrun stapler staple Pulse24Sync.component
   xcrun stapler staple Pulse24Sync.vst3
   xcrun stapler staple Pulse24Sync.app
   ```

3. **Verify notarization**
   ```bash
   xcrun stapler validate Pulse24Sync.component
   ```

## 🔧 Troubleshooting

### Problem: "No Developer ID Certificate Found"

**Solution:**
1. Verify you're enrolled in Apple Developer Program
2. Download and install your Developer ID certificate
3. Restart Xcode and your terminal

### Problem: "Code signing failed"

**Solution:**
1. Check certificate is in Keychain Access
2. Verify certificate isn't expired
3. Ensure you have the private key (not just certificate)

### Problem: "entitlements.plist not found"

**Solution:**
The script should automatically find `entitlements.plist` in your project root. If missing, the file has been created for you.

### Problem: Plugin still shows security warning

**Solutions:**
1. **For personal use:** Right-click plugin → "Open" → "Open anyway"
2. **For distribution:** Complete notarization process above
3. **Alternative:** Users can run: `sudo spctl --master-disable` (not recommended)

## 📋 Environment Variables

You can customize the build process with these environment variables:

```bash
# Specify signing identity
export CODE_SIGN_IDENTITY="Developer ID Application: Your Name (TEAMID)"

# Specify team ID for notarization
export DEVELOPMENT_TEAM="YOUR_TEAM_ID"

# Apple ID for notarization
export APPLE_ID="your-apple-id@example.com"

# App-specific password for notarization
export NOTARIZATION_PASSWORD="your-app-specific-password"
```

## 🎯 Quick Commands Reference

```bash
# Build with automatic signing
./build_macos.sh

# Check what certificates you have
security find-identity -v -p codesigning

# Verify a signature
codesign -dv --verbose=4 path/to/your.component

# Test if a file will be accepted by Gatekeeper
spctl -a -v path/to/your.component
```

## 📞 Getting Help

If you encounter issues:

1. **Check certificate expiration**: Certificates last 5 years
2. **Verify Apple Developer account status**: Must be active
3. **Review Xcode settings**: Should match your Developer ID
4. **Check system logs**: `Console.app` shows detailed error messages

## 🎉 Success Indicators

After following this guide, you should see:

- ✅ No security warnings when installing plugins
- ✅ Proper signatures verified with `codesign -dv`
- ✅ Plugins load correctly in DAWs
- ✅ Ready for distribution to other users

Your Pulse24Sync plugin will now be trusted by macOS and won't trigger security warnings!