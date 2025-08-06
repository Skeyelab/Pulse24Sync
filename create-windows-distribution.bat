@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Pulse24Sync - Windows Distribution Creator
echo ========================================
echo.

REM Check if we have the build files
if not exist "x64\Release\VST3\Pulse24Sync.vst3" (
    echo ERROR: VST3 plugin not found at x64\Release\VST3\Pulse24Sync.vst3
    echo Please run the build script first or ensure you have the build files.
    pause
    exit /b 1
)

echo 📦 Creating clean distribution from existing build...

REM Create distribution directory
if exist "dist" rd /s /q "dist" >nul 2>&1
mkdir "dist"

REM Copy VST3 plugin
echo   - Copying VST3 plugin...
xcopy "x64\Release\VST3\Pulse24Sync.vst3" "dist\Pulse24Sync.vst3\" /E /I /Y >nul

REM Copy standalone application
if exist "x64\Release\Standalone Plugin\Pulse24Sync.exe" (
    echo   - Copying standalone application...
    copy "x64\Release\Standalone Plugin\Pulse24Sync.exe" "dist\Pulse24Sync.exe" >nul
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
echo Original build size: ~1GB (includes development files)
echo Distribution size: ~5MB (end-user files only)
echo.

pause
