# Build Artifact Caching System

This document explains how the Pulse24Sync project implements cross-workflow artifact caching to speed up releases by reusing successful PR builds.

## Overview

The caching system allows release builds to reuse artifacts from successful PR builds when the source code hasn't changed, significantly reducing build times and CI/CD costs.

## How It Works

### 1. PR Build Artifact Storage

When PR builds complete successfully (in `build.yml`):

- **Build Artifacts**: VST3 plugins, Audio Units, and standalone applications are uploaded with 30-day retention
- **Build Metadata**: Source code hash, commit SHA, run ID, and build timestamp are stored alongside artifacts
- **Naming Convention**: Artifacts are named using the pattern `build-metadata-{OS}-{COMMIT_SHA}`

### 2. Release Build Artifact Reuse

When a release is triggered (in `release.yml`):

1. **Search for Cached Artifacts**: The workflow searches recent successful build runs for artifacts matching the current commit SHA
2. **Hash Verification**: Downloads and compares the source code hash to ensure the cached artifacts match the current codebase
3. **Artifact Restoration**: If valid, copies cached artifacts to the expected build directories
4. **Build Skipping**: Skips the time-consuming compilation steps (JUCE setup, Projucer generation, Xcode/MSBuild compilation)
5. **Fallback**: If no valid cached artifacts are found, falls back to building from scratch

### 3. Cache Key Strategy

The system uses consistent cache keys across workflows:

```yaml
key: ${{ runner.os }}-build-cache-${{ hashFiles('Pulse24Sync.jucer', 'Source/**') }}
```

This ensures that artifacts are only reused when:
- The operating system matches (macOS artifacts for macOS releases, Windows for Windows)
- The source code is identical (same hash of `Pulse24Sync.jucer` and all `Source/**` files)
- The commit SHA matches exactly

## Benefits

### Time Savings
- **Typical Build Time**: 5-15 minutes per platform
- **Cached Build Time**: 1-2 minutes per platform
- **Total Savings**: 60-80% reduction in release build time

### Cost Reduction
- Reduces GitHub Actions minutes consumption
- Fewer compute resources needed for releases
- More efficient CI/CD pipeline

### Reliability
- Uses the exact same artifacts that were tested in PR builds
- Eliminates potential differences between PR and release builds
- Maintains full traceability of which PR build the artifacts came from

## Workflow Integration

### Build Workflow (`build.yml`)
```yaml
# Uploads artifacts after successful builds
- name: Upload macOS build artifacts
  uses: actions/upload-artifact@v4
  with:
    name: macos-builds
    path: Builds/MacOSX/build/Release/
    retention-days: 30

# Stores metadata for later verification
- name: Upload build metadata
  uses: actions/upload-artifact@v4
  with:
    name: build-metadata-${{ runner.os }}-${{ github.sha }}
    path: build-metadata/
    retention-days: 30
```

### Release Workflow (`release.yml`)
```yaml
# Attempts to download cached artifacts
- name: Download cached build artifacts
  uses: actions/github-script@v7
  # [searches for artifacts matching current commit]

# Verifies artifacts are still valid
- name: Verify cached artifacts
  run: |
    if [ "$cached_hash" = "$current_hash" ]; then
      echo "✅ Cached artifacts are valid"
      # Copy to build directories
    fi

# Only builds if no valid cache found
- name: Build macOS Plugin
  if: matrix.os == 'macOS' && steps.verify_cache.outputs.cache_valid != 'true'
```

## Monitoring and Debugging

### Build Status Indicators
The workflow provides clear indicators of cache usage:

- 🚀 **Cache Hit**: "Used cached build artifacts - skipped compilation"
- 🔨 **Cache Miss**: "Built from source - no suitable cached artifacts found"
- ✅ **Validation**: Confirms all expected plugin formats are present

### Troubleshooting

If releases fail after implementing caching:

1. **Check Artifact Availability**: Ensure the PR build completed successfully and artifacts were uploaded
2. **Verify Hash Matching**: Source code changes between PR merge and tag creation will invalidate cache
3. **Artifact Expiration**: Artifacts are retained for 30 days; older builds won't be cached
4. **Path Consistency**: Ensure artifact paths match between build and release workflows

## Implementation Details

### Artifact Retention
- **Retention Period**: 30 days
- **Storage Location**: GitHub Actions artifact storage
- **Cleanup**: Automatic cleanup after retention period

### Security Considerations
- Artifacts are scoped to the repository
- Only successful builds from the same commit are used
- Hash verification prevents using outdated or modified artifacts

### Platform Support
- **macOS**: VST3, Audio Unit (.component), Standalone (.app)
- **Windows**: VST3, Standalone (.exe)
- **Linux**: Currently disabled in build workflow

## Future Improvements

Potential enhancements to the caching system:

1. **Extended Cache Lookup**: Search for artifacts from recent commits on the same branch
2. **Incremental Builds**: Cache intermediate build artifacts (object files, dependencies)
3. **Cross-Platform Sharing**: Share JUCE modules and dependencies between platforms
4. **Cache Analytics**: Track cache hit rates and time savings
5. **Manual Cache Invalidation**: Workflow dispatch option to force rebuild

## Configuration

### Artifact Retention Adjustment
To change retention period, modify the `retention-days` in both workflows:

```yaml
retention-days: 30  # Change to desired number of days
```

### Disabling Cache
To disable the caching system, add this environment variable:

```yaml
env:
  DISABLE_ARTIFACT_CACHE: true
```

### Platform-Specific Caching
Caching can be enabled/disabled per platform by modifying the `if` conditions in the workflows.