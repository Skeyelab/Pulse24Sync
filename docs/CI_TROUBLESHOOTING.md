# CI/CD Troubleshooting Guide

This guide helps resolve common issues with the GitHub Actions CI/CD pipeline.

## Common Issues and Solutions

### 1. Deprecated Action Versions

**Error:**
```
This request has been automatically failed because it uses a deprecated version of `actions/upload-artifact: v3`
```

**Solution:**
Update to the latest action versions:
- `actions/checkout@v4`
- `actions/cache@v4`
- `actions/upload-artifact@v4`
- `codecov/codecov-action@v4`

### 2. Windows Path Issues

**Error:**
```
'./tests/Pulse24SyncTests' is not recognized as an internal or external command
```

**Solution:**
Windows uses different executable paths. Use the correct path based on the platform:
```yaml
- name: Run tests
  run: |
    if [[ "${{ runner.os }}" == "Windows" ]]; then
      ./tests/${{ env.BUILD_TYPE }}/Pulse24SyncTests.exe --verbose
    else
      ./tests/Pulse24SyncTests --verbose
    fi
  shell: bash
```

### 3. CMake Configuration Errors

**Error:**
```
CMake Error: Could not find CMAKE_MAKE_PROGRAM
```

**Solution:**
Ensure proper generator is specified for each platform:
```yaml
# For Windows
cmake -B build -A x64 -DCMAKE_BUILD_TYPE=Release

# For macOS/Linux
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

### 4. Missing Dependencies

**Error:**
```
Package 'alsa' not found
```

**Solution:**
Install all required Linux dependencies:
```yaml
- name: Install dependencies
  run: |
    sudo apt-get update
    sudo apt-get install -y \
      build-essential \
      cmake \
      libasound2-dev \
      libjack-jackd2-dev \
      libfreetype6-dev \
      libx11-dev \
      libxcomposite-dev \
      libxcursor-dev \
      libxext-dev \
      libxinerama-dev \
      libxrandr-dev \
      libxrender-dev \
      libgl1-mesa-dev \
      libglu1-mesa-dev \
      libcurl4-openssl-dev
```

### 5. Test Execution Failures

**Error:**
```
Test executable not found
```

**Solution:**
Use CTest instead of direct executable calls for better cross-platform compatibility:
```yaml
- name: Test
  working-directory: build-tests
  run: ctest --output-on-failure --verbose -C ${{ env.BUILD_TYPE }}
```

### 6. Cache Issues

**Error:**
```
Cache not found
```

**Solution:**
Use appropriate cache keys and paths:
```yaml
- name: Cache dependencies
  uses: actions/cache@v4
  with:
    path: |
      build-tests/_deps
    key: ${{ runner.os }}-deps-${{ hashFiles('CMakeLists.txt') }}
    restore-keys: ${{ runner.os }}-deps-
```

### 7. JUCE Download Failures

**Error:**
```
Failed to download JUCE
```

**Solutions:**
1. **Use FetchContent properly:**
   ```cmake
   FetchContent_Declare(
       JUCE
       GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
       GIT_TAG 7.0.12
   )
   FetchContent_MakeAvailable(JUCE)
   ```

2. **Cache JUCE downloads:**
   ```yaml
   - name: Cache JUCE
     uses: actions/cache@v4
     with:
       path: build-tests/_deps
       key: juce-${{ hashFiles('CMakeLists.txt') }}
   ```

### 8. Permission Issues

**Error:**
```
Permission denied
```

**Solution:**
Ensure scripts have execute permissions:
```yaml
- name: Make script executable
  run: chmod +x ./run-tests.sh
```

### 9. Runner Image Issues

**Error:**
```
The runner has received a shutdown signal
```

**Solutions:**
1. **Use specific runner versions:**
   ```yaml
   runs-on: ubuntu-22.04  # Instead of ubuntu-latest
   ```

2. **Reduce parallel jobs:**
   ```yaml
   run: cmake --build build --parallel 2  # Instead of full parallel
   ```

### 10. Timeout Issues

**Error:**
```
The job was canceled because it exceeded the maximum execution time
```

**Solutions:**
1. **Set appropriate timeouts:**
   ```yaml
   timeout-minutes: 30
   ```

2. **Optimize builds:**
   ```yaml
   - name: Build
     run: cmake --build build --config Release --parallel
   ```

## Best Practices

### 1. Workflow Organization
- Use separate workflows for different purposes (test, build, deploy)
- Keep workflows simple and focused
- Use matrix builds for cross-platform testing

### 2. Caching Strategy
- Cache dependencies, not build artifacts
- Use appropriate cache keys with file hashes
- Set reasonable cache retention periods

### 3. Error Handling
- Use `fail-fast: false` for matrix builds when appropriate
- Upload artifacts on failure for debugging
- Provide clear error messages and context

### 4. Performance Optimization
- Use parallel builds where possible
- Cache frequently downloaded dependencies
- Minimize unnecessary steps

## Debugging Workflows

### 1. Enable Debug Logging
```yaml
- name: Debug step
  run: |
    echo "Debug: Current directory: $(pwd)"
    echo "Debug: Files: $(ls -la)"
    echo "Debug: Environment: $(env)"
```

### 2. Upload Artifacts for Investigation
```yaml
- name: Upload debug artifacts
  if: failure()
  uses: actions/upload-artifact@v4
  with:
    name: debug-artifacts
    path: |
      build-tests/
      *.log
```

### 3. Use Simplified Workflows
Create minimal workflows to isolate issues:
```yaml
name: Debug Workflow
on: workflow_dispatch
jobs:
  debug:
    runs-on: ubuntu-latest
    steps:
    - name: Checkout
      uses: actions/checkout@v4
    - name: Test basic functionality
      run: |
        echo "Testing basic commands"
        cmake --version
        gcc --version
```

## Getting Help

1. **Check workflow logs** in the GitHub Actions tab
2. **Compare with working examples** from similar projects
3. **Test locally** using `act` or similar tools
4. **Review GitHub Actions documentation** for the latest features
5. **Check action repositories** for usage examples and known issues

## Useful Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [CMake Documentation](https://cmake.org/documentation/)
- [JUCE Documentation](https://juce.com/learn/documentation)
- [Actions Marketplace](https://github.com/marketplace?type=actions)

---

For project-specific issues, check the main [TESTING.md](TESTING.md) documentation.