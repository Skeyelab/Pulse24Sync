@echo off
echo ========================================
echo Pulse24Sync - Windows Build Script
echo ========================================
echo.

REM Check if Visual Studio is available
where msbuild >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: MSBuild not found. Please install Visual Studio 2022 with C++ workload.
    echo Download from: https://visualstudio.microsoft.com/downloads/
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
    pause
    exit /b 1
)

REM Build the standalone application
echo [2/3] Building Standalone Application...
msbuild Builds\VisualStudio2022\Pulse24Sync.sln /p:Configuration=Release /p:Platform=x64 /p:BuildProjectReferences=true /m /t:Pulse24Sync_StandalonePlugin

if %errorlevel% neq 0 (
    echo ERROR: Standalone build failed!
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
pause
