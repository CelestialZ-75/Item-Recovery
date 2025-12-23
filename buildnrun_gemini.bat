@echo off
REM ItemRecovery Windows Build and Run Script
setlocal enabledelayedexpansion

REM Default parameters
set "BUILD_TYPE=GUI"
set "QT_PATH="
set "COMPILER=MSVC"

REM Parse command line arguments
if not "%~1"=="" set "BUILD_TYPE=%~1"
if not "%~2"=="" set "QT_PATH=%~2"

REM Check if CMake is installed
where cmake >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake not found.
    pause
    exit /b 1
)

REM If Qt path not provided, try auto-detection
if "%QT_PATH%"=="" (
    echo [INFO] Qt path not specified, attempting auto-detection...
    set "QT_FOUND=0"
    
    REM Check MinGW versions
    if exist "C:\Qt\6.9.3\mingw_64" (
        set "QT_PATH=C:\Qt\6.9.3\mingw_64"
        set "COMPILER=MinGW"
        set "QT_FOUND=1"
    ) else if exist "C:\Qt\6.5.3\mingw_64" (
        set "QT_PATH=C:\Qt\6.5.3\mingw_64"
        set "COMPILER=MinGW"
        set "QT_FOUND=1"
    ) else if exist "C:\Qt\6.5.0\mingw_64" (
        set "QT_PATH=C:\Qt\6.5.0\mingw_64"
        set "COMPILER=MinGW"
        set "QT_FOUND=1"
    ) else if exist "C:\Qt\5.15.2\mingw81_64" (
        set "QT_PATH=C:\Qt\5.15.2\mingw81_64"
        set "COMPILER=MinGW"
        set "QT_FOUND=1"
    )
    
    REM Check MSVC versions if MinGW not found
    if "!QT_FOUND!"=="0" (
        if exist "C:\Qt\5.15.2\msvc2022_64" (
            set "QT_PATH=C:\Qt\5.15.2\msvc2022_64"
            set "COMPILER=MSVC"
            set "QT_FOUND=1"
        ) else if exist "C:\Qt\6.5.0\msvc2022_64" (
            set "QT_PATH=C:\Qt\6.5.0\msvc2022_64"
            set "COMPILER=MSVC"
            set "QT_FOUND=1"
        )
    )

    if "!QT_FOUND!"=="0" (
        echo [ERROR] Qt installation not found. Please specify manually.
        pause
        exit /b 1
    )
)

echo [INFO] Using Qt Path: "%QT_PATH%"

REM --- Auto-setup MinGW Toolchain from Qt Tools if using MinGW ---
if "%COMPILER%"=="MinGW" (
    echo [INFO] Checking for Qt MinGW compiler...
    set "MINGW_TOOL_FOUND=0"
    
    if exist "C:\Qt\Tools" (
        for /d %%D in ("C:\Qt\Tools\mingw*") do (
             if exist "%%D\bin\g++.exe" (
                 set "MINGW_BIN=%%D\bin"
                 set "MINGW_TOOL_FOUND=1"
             )
        )
    )

    if "!MINGW_TOOL_FOUND!"=="1" (
        echo [INFO] Found Qt MinGW tools at: !MINGW_BIN!
        echo [INFO] Prepending to PATH to override system MinGW...
        set "PATH=!MINGW_BIN!;%PATH%"
    ) else (
        echo [WARNING] Could not find MinGW in C:\Qt\Tools.
    )
)
REM --------------------------------------------------------------------

REM Create build directory
if not exist "build" mkdir build
cd build

REM Clean old CMake cache if exists
if exist "CMakeCache.txt" (
    echo [INFO] Removing old CMake cache...
    del /q CMakeCache.txt 2>nul
    if exist "CMakeFiles" rmdir /s /q CMakeFiles 2>nul
)

REM Select generator
if "%COMPILER%"=="MSVC" (
    where cl >nul 2>&1
    if errorlevel 1 (
        set "GENERATOR=Visual Studio 17 2022"
    ) else (
        set "GENERATOR=Visual Studio 17 2022"
    )
)

if "%COMPILER%"=="MinGW" (
    set "GENERATOR=MinGW Makefiles"
)

echo [INFO] Using generator: "%GENERATOR%"

REM Configure CMake
echo [Step 1/4] Configuring...
echo [DEBUG] Qt Path: "%QT_PATH%"

if "%COMPILER%"=="MSVC" (
    cmake .. -G "%GENERATOR%" -A x64 -DCMAKE_PREFIX_PATH="%QT_PATH%"
) else (
    cmake .. -G "%GENERATOR%" -DCMAKE_PREFIX_PATH="%QT_PATH%"
)

if errorlevel 1 (
    echo [ERROR] CMake failed
    pause
    exit /b 1
)

REM Build
echo [Step 2/4] Building...
cmake --build . --config Release
if errorlevel 1 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

REM Deployment
if "%BUILD_TYPE%"=="GUI" (
    echo [Step 3/4] Deploying...
    set "WINDEPLOYQT=%QT_PATH%\bin\windeployqt.exe"
    if exist "!WINDEPLOYQT!" (
        if exist "bin\Release\ItemRecovery_GUI.exe" (
            echo [INFO] Running windeployqt...
            "!WINDEPLOYQT!" "bin\Release\ItemRecovery_GUI.exe" --dir "bin\Release" --no-translations
        ) else if exist "bin\ItemRecovery_GUI.exe" (
            echo [INFO] Running windeployqt...
            "!WINDEPLOYQT!" "bin\ItemRecovery_GUI.exe" --dir "bin" --no-translations
        )
    )
)

echo [Step 4/4] Done.
pause
