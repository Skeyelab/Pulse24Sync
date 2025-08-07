# Notarization Setup Guide for Pulse24Sync

This guide will help you set up notarization for your Pulse24Sync plugin to eliminate the "Apple could not verify is free of malware" warning.

## 🎯 The Problem

Your plugin is currently code-signed with a Developer ID certificate, but it's not notarized. macOS requires Developer ID applications to be notarized by Apple before they can run without triggering security warnings.

## ✅ The Solution

We've added notarization to the GitHub Actions workflow. You need to set up the required secrets in your GitHub repository.

## 🔑 Required GitHub Secrets

You need to add these secrets to your GitHub repository:

1. **Go to your GitHub repository**
2. **Click Settings** → **Secrets and variables** → **Actions**
3. **Click "New repository secret"** for each of the following:

### 1. CODE_SIGN_IDENTITY
- **Name**: `CODE_SIGN_IDENTITY`
- **Value**: Your full signing identity (e.g., `"Developer ID Application: Eric Dahl (5HR8E5CWR7)"`)

To find your signing identity, run:
```bash
security find-identity -v -p codesigning
```

### 2. DEVELOPMENT_TEAM
- **Name**: `DEVELOPMENT_TEAM`
- **Value**: Your Apple Developer Team ID (e.g., `5HR8E5CWR7`)

This is the same ID that appears in your signing identity.

### 3. APPLE_ID
- **Name**: `APPLE_ID`
- **Value**: Your Apple ID email address (e.g., `your-email@example.com`)

### 4. NOTARIZATION_PASSWORD
- **Name**: `NOTARIZATION_PASSWORD`
- **Value**: An app-specific password for your Apple ID

## 🔐 Creating an App-Specific Password

1. **Go to https://appleid.apple.com/**
2. **Sign in** with your Apple ID
3. **Click "Sign-in and Security"** → **"App-Specific Passwords"**
4. **Click "Generate Password"**
5. **Enter a name** (e.g., "Pulse24Sync Notarization")
6. **Copy the generated password** (it will look like: `abcd-efgh-ijkl-mnop`)
7. **Add it as the `NOTARIZATION_PASSWORD` secret**

## 🚀 Testing the Setup

1. **Commit and push your changes** to trigger a new workflow run
2. **Check the workflow logs** for the "Notarize macOS Plugin" step
3. **Look for success messages** like:
   ```
   ✅ Notarization completed for Pulse24Sync.component
   ✅ Successfully stapled notarization to Pulse24Sync.component
   ✅ Notarization verification passed for Pulse24Sync.component
   ```

## 🔍 Verification

After a successful notarized build, you can verify the notarization:

```bash
# Download the notarized plugin from GitHub Actions artifacts
# Then verify notarization:
xcrun stapler validate Pulse24Sync.component
```

You should see: `Pulse24Sync.component: valid`

## 🛠️ Troubleshooting

### Problem: "Skipping notarization - missing required environment variables"

**Solution**: Make sure all four secrets are set in your GitHub repository:
- `CODE_SIGN_IDENTITY`
- `DEVELOPMENT_TEAM`
- `APPLE_ID`
- `NOTARIZATION_PASSWORD`

### Problem: "Notarization failed"

**Solutions**:
1. **Check your Apple ID**: Make sure it's the same one associated with your Developer account
2. **Verify app-specific password**: Generate a new one if needed
3. **Check team ID**: Ensure it matches your Developer account
4. **Check certificate**: Make sure your Developer ID certificate is valid and not expired

### Problem: "Failed to staple notarization"

**Solution**: This usually means the notarization was successful but stapling failed. The plugin should still work, but you may need to manually staple it.

## 📋 Manual Notarization (Alternative)

If you prefer to notarize manually:

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

## 🎉 Expected Results

After setting up notarization:

- ✅ No more "Apple could not verify is free of malware" warnings
- ✅ Plugins install and run without security prompts
- ✅ Users can download and use your plugin without issues
- ✅ Professional distribution-ready builds

## 📞 Getting Help

If you encounter issues:

1. **Check workflow logs**: Look for detailed error messages in the "Notarize macOS Plugin" step
2. **Verify Apple Developer account**: Ensure your account is active and has the right permissions
3. **Check certificate status**: Make sure your Developer ID certificate is valid
4. **Test locally first**: Try manual notarization to verify your credentials work

Your Pulse24Sync plugin will now be fully trusted by macOS and ready for distribution!
