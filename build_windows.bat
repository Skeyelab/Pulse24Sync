@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Pulse24Sync - Windows Build Script
echo ========================================
echo.

REM Function to cleanup temporary files
:cleanup
echo 🧹 Performing cleanup...

REM Clean up temporary files
del /s /q *.tmp >nul 2>&1
del /s /q *.temp >nul 2>&1
del /s /q *~ >nul 2>&1

REM Clean up Windows system files
del /s /q Thumbs.db >nul 2>&1
del /s /q ehthumbs.db >nul 2>&1

REM Clean up Visual Studio temporary files
for /d /r . %%d in (*.build) do @if exist "%%d" rd /s /q "%%d" >nul 2>&1
for /d /r . %%d in (.vs) do @if exist "%%d" rd /s /q "%%d" >nul 2>&1

if "%BUILD_FAILED%"=="1" (
    echo   - Cleaning up failed build artifacts...
    if exist "Builds\VisualStudio2022\build" rd /s /q "Builds\VisualStudio2022\build" >nul 2>&1
    echo ❌ Build failed - temporary files cleaned up
    exit /b 1
)

echo 🧹 Cleanup completed
goto :eof

REM Check if Visual Studio is available
where msbuild >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: MSBuild not found. Please install Visual Studio 2022 with C++ workload.
    echo Download from: https://visualstudio.microsoft.com/downloads/
    set BUILD_FAILED=1
    call :cleanup
    pause
    exit /b 1
)

echo Building Pulse24Sync VST3 Plugin...
echo.

REM Build the VST3 plugin
echo [1/3] Building VST3 Plugin...
msbuild Builds\VisualStudio2022\Pulse24Sync.sln /p:Configuration=Release /p:Platform=x64 /p:BuildProjectReferences=true /m

if %errorlevel% neq 0 (
    echo ERROR: VST3 build failed!
    set BUILD_FAILED=1
    call :cleanup
    pause
    exit /b 1
)

REM Build the standalone application
echo [2/3] Building Standalone Application...
msbuild Builds\VisualStudio2022\Pulse24Sync.sln /p:Configuration=Release /p:Platform=x64 /p:BuildProjectReferences=true /m /t:Pulse24Sync_StandalonePlugin

if %errorlevel% neq 0 (
    echo ERROR: Standalone build failed!
    set BUILD_FAILED=1
    call :cleanup
    pause
    exit /b 1
)

echo [3/3] Build completed successfully!
echo.

REM Check if build outputs exist
if exist "Builds\VisualStudio2022\build\Release\Pulse24Sync.vst3" (
    echo VST3 Plugin: Builds\VisualStudio2022\build\Release\Pulse24Sync.vst3
) else (
    echo WARNING: VST3 plugin not found in expected location
)

if exist "Builds\VisualStudio2022\build\Release\Pulse24Sync.exe" (
    echo Standalone App: Builds\VisualStudio2022\build\Release\Pulse24Sync.exe
) else (
    echo WARNING: Standalone application not found in expected location
)

echo.
echo ========================================
echo Installation Instructions:
echo ========================================
echo.
echo To install the VST3 plugin, copy:
echo   Builds\VisualStudio2022\build\Release\Pulse24Sync.vst3
echo to:
echo   C:\Program Files\Common Files\VST3\
echo.
echo Then restart your DAW and scan for new plugins.
echo.

REM Perform cleanup of temporary files (but keep successful build artifacts)
call :cleanup

pause
