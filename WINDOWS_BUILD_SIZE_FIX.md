# Build Size Issues - Fixed

## Problem
Both Windows and macOS builds were including **development files** that should **NOT** be distributed to end users, resulting in unnecessarily large distribution packages.

### Windows Builds
- **Before**: ~1GB (includes all intermediate files)
- **After**: ~5MB (distribution files only)

### macOS Builds
- **Before**: ~35MB (includes static library and helper files)
- **After**: ~12MB (distribution files only)

## Root Cause

### Windows Build Issues
The Windows build script was copying the entire build directory, which included:
- **`.obj` files** (compiled object files) - ~200MB
- **`.pdb` files** (debug symbols) - ~57MB
- **`.iobj` files** (IntelliSense files) - ~207MB
- **`.lib` files** (static libraries) - ~267MB
- **Build logs and temporary files**

### macOS Build Issues
The macOS build was including:
- **`libPulse24Sync.a`** (static library) - ~24MB
- **`juce_vst3_helper`** (build helper) - ~188KB
- **Build artifacts and temporary files**

## Solution
Modified the build processes to create **clean distributions** that only include the final plugin files.

## Files Modified

### Windows
1. **`build_windows.bat`** - Updated to create clean distribution
2. **`create-windows-distribution.bat`** - New script to extract distribution files from existing builds

### macOS
1. **`build_macos.sh`** - Updated to create clean distribution
2. **`create-macos-distribution.sh`** - New script to extract distribution files from existing builds

## Usage

### Windows

#### Option 1: Build with Clean Distribution
```bash
# Run the updated build script
build_windows.bat
```
This will build and automatically create a clean `dist` folder with only the distribution files.

#### Option 2: Extract from Existing Build
```bash
# If you already have a large build, extract just the distribution files
create-windows-distribution.bat
```

### macOS

#### Option 1: Build with Clean Distribution
```bash
# Run the updated build script
./build_macos.sh
```
This will build and automatically create a clean `dist` folder with only the distribution files.

#### Option 2: Extract from Existing Build
```bash
# If you already have a large build, extract just the distribution files
./create-macos-distribution.sh
```

## File Sizes Comparison

| Platform | Before (with dev files) | After (distribution only) |
|----------|------------------------|---------------------------|
| Windows  | ~1GB                   | ~5MB                     |
| macOS    | ~35MB                  | ~12MB                    |
| Linux    | ~5MB                   | ~5MB                     |

## Distribution Files

### Windows Distribution
```
dist/
├── Pulse24Sync.vst3/          # VST3 plugin (~5MB)
│   └── Contents/
│       ├── Resources/
│       └── x86_64-win/
│           └── Pulse24Sync.vst3
└── Pulse24Sync.exe            # Standalone app (~5MB)
```

### macOS Distribution
```
dist/
├── Pulse24Sync.vst3/          # VST3 plugin (~3.7MB)
├── Pulse24Sync.component/     # AU plugin (~3.6MB)
└── Pulse24Sync.app/           # Standalone app (~4.4MB)
```

## Installation

### Windows
Copy the `Pulse24Sync.vst3` folder to:
```
C:\Program Files\Common Files\VST3\
```

### macOS
Copy the files from the `dist` folder to:
```
VST3: ~/Library/Audio/Plug-Ins/VST3/
AU:   ~/Library/Audio/Plug-Ins/Components/
```

Then restart your DAW and scan for new plugins.
