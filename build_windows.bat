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

REM Function to create clean distribution
:create_distribution
echo 📦 Creating clean distribution...

REM Create distribution directory
if exist "dist" rd /s /q "dist" >nul 2>&1
mkdir "dist"

REM Copy VST3 plugin
if exist "Builds\VisualStudio2022\x64\Release\VST3\Pulse24Sync.vst3" (
    echo   - Copying VST3 plugin...
    xcopy "Builds\VisualStudio2022\x64\Release\VST3\Pulse24Sync.vst3" "dist\Pulse24Sync.vst3\" /E /I /Y >nul
)

REM Copy standalone application
if exist "Builds\VisualStudio2022\x64\Release\Standalone Plugin\Pulse24Sync.exe" (
    echo   - Copying standalone application...
    copy "Builds\VisualStudio2022\x64\Release\Standalone Plugin\Pulse24Sync.exe" "dist\Pulse24Sync.exe" >nul
)

REM Show distribution size
echo.
echo 📊 Distribution created in 'dist' folder:
if exist "dist" (
    for /f "tokens=*" %%i in ('dir /s "dist" ^| find "File(s)"') do echo   %%i
    for /f "tokens=*" %%i in ('dir /s "dist" ^| find "Dir(s)"') do echo   %%i
)

echo.
echo ========================================
echo Clean Distribution Created Successfully!
echo ========================================
echo.
echo Distribution files are in the 'dist' folder:
echo   - Pulse24Sync.vst3 (VST3 plugin)
echo   - Pulse24Sync.exe (Standalone application)
echo.
echo These files are ready for distribution to end users.
echo.

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

echo [3/3] Creating clean distribution...
call :create_distribution

REM Perform cleanup of temporary files (but keep successful build artifacts)
call :cleanup

echo ========================================
echo Installation Instructions:
echo ========================================
echo.
echo To install the VST3 plugin, copy:
echo   dist\Pulse24Sync.vst3
echo to:
echo   C:\Program Files\Common Files\VST3\
echo.
echo Then restart your DAW and scan for new plugins.
echo.

pause
