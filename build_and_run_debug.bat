@echo off
REM Debug version of build_and_run.bat
REM This script shows detailed execution information

REM Enable echo to see each command
if "%1"=="DEBUG" (
    echo on
    shift
) else (
    echo off
)

setlocal enabledelayedexpansion

echo ========================================
echo DEBUG MODE - ItemRecovery Build Script
echo ========================================
echo.

REM Default parameters
set BUILD_TYPE=GUI
set QT_PATH=
set COMPILER=MSVC

echo [DEBUG] Initial parameters:
echo   BUILD_TYPE=%BUILD_TYPE%
echo   QT_PATH=%QT_PATH%
echo   COMPILER=%COMPILER%
echo   Command line args: %*
echo.

REM Parse command line arguments - safer method
set ARG1=%~1
set ARG2=%~2

echo [DEBUG] Parsed arguments:
echo   ARG1=%ARG1%
echo   ARG2=%ARG2%
echo.

if not "%ARG1%"=="" (
    echo [DEBUG] Setting BUILD_TYPE to %ARG1%
    set BUILD_TYPE=%ARG1%
)

if not "%ARG2%"=="" (
    echo [DEBUG] Setting QT_PATH to %ARG2%
    set QT_PATH=%ARG2%
)

echo [DEBUG] After parsing:
echo   BUILD_TYPE=%BUILD_TYPE%
echo   QT_PATH=%QT_PATH%
echo.

REM Check CMake
echo [DEBUG] Checking for CMake...
where cmake >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake not found. Please install CMake and add it to PATH
    echo Download: https://cmake.org/download/
    pause
    exit /b 1
) else (
    echo [DEBUG] CMake found
)

REM Auto-detect Qt if not provided
if "%QT_PATH%"=="" (
    echo [DEBUG] Qt path not specified, attempting auto-detection...
    
    if exist "C:\Qt\5.15.2\msvc2019_64" (
        set QT_PATH=C:\Qt\5.15.2\msvc2019_64
        set COMPILER=MSVC
        echo [DEBUG] Found Qt (MSVC): %QT_PATH%
    ) else if exist "C:\Qt\5.15.2\mingw81_64" (
        set QT_PATH=C:\Qt\5.15.2\mingw81_64
        set COMPILER=MinGW
        echo [DEBUG] Found Qt (MinGW): %QT_PATH%
    ) else if exist "C:\Qt\5.14.2\msvc2019_64" (
        set QT_PATH=C:\Qt\5.14.2\msvc2019_64
        set COMPILER=MSVC
        echo [DEBUG] Found Qt (MSVC): %QT_PATH%
    ) else (
        echo [ERROR] Qt installation not found. Please specify Qt path manually
        echo Usage: build_and_run_debug.bat [DEBUG] [CLI^|GUI] "C:\Qt\5.15.2\msvc2019_64"
        pause
        exit /b 1
    )
)

REM Validate Qt path
if "%QT_PATH%"=="" (
    echo [ERROR] Qt path is empty
    pause
    exit /b 1
)

echo [DEBUG] Checking Qt path: %QT_PATH%
if not exist "%QT_PATH%\bin\qmake.exe" (
    echo [ERROR] Invalid Qt path: %QT_PATH%
    echo Please ensure the path points to Qt installation directory
    pause
    exit /b 1
) else (
    echo [DEBUG] Qt path validated
)

echo.
echo ========================================
echo ItemRecovery Build Script
echo ========================================
echo Build Type: %BUILD_TYPE%
echo Qt Path: %QT_PATH%
echo Compiler: %COMPILER%
echo ========================================
echo.

REM Create build directory
if not exist "build" (
    echo [DEBUG] Creating build directory...
    mkdir build
    if errorlevel 1 (
        echo [ERROR] Failed to create build directory
        pause
        exit /b 1
    )
) else (
    echo [DEBUG] Build directory already exists
)

echo [DEBUG] Changing to build directory...
cd build
if errorlevel 1 (
    echo [ERROR] Failed to change to build directory
    pause
    exit /b 1
)

REM Determine generator
set GENERATOR=
if "%COMPILER%"=="MSVC" (
    echo [DEBUG] Detecting Visual Studio version...
    where cl >nul 2>&1
    if errorlevel 1 (
        echo [WARNING] MSVC compiler not found, trying MinGW...
        set COMPILER=MinGW
        set GENERATOR=MinGW Makefiles
    ) else (
        echo [DEBUG] MSVC compiler found, detecting version...
        REM Simplified version detection
        set GENERATOR=Visual Studio 17 2022
    )
)

if "%COMPILER%"=="MinGW" (
    if "%GENERATOR%"=="" (
        set GENERATOR=MinGW Makefiles
    )
    where g++ >nul 2>&1
    if errorlevel 1 (
        echo [ERROR] MinGW compiler not found. Please add it to PATH
        pause
        exit /b 1
    )
)

if "%GENERATOR%"=="" (
    echo [ERROR] Failed to determine generator
    pause
    exit /b 1
)

echo [INFO] Using generator: %GENERATOR%
echo.

REM Configure CMake
echo [Step 1/4] Configuring CMake...
echo [DEBUG] CMake command:
if "%COMPILER%"=="MSVC" (
    echo   cmake .. -G "%GENERATOR%" -A x64 -DCMAKE_PREFIX_PATH="%QT_PATH%"
    cmake .. -G "%GENERATOR%" -A x64 -DCMAKE_PREFIX_PATH="%QT_PATH%"
) else (
    echo   cmake .. -G "%GENERATOR%" -DCMAKE_PREFIX_PATH="%QT_PATH%"
    cmake .. -G "%GENERATOR%" -DCMAKE_PREFIX_PATH="%QT_PATH%"
)

if errorlevel 1 (
    echo [ERROR] CMake configuration failed at line above
    pause
    exit /b 1
)

echo [SUCCESS] CMake configuration completed
echo.

REM Build project
echo [Step 2/4] Building project...
cmake --build . --config Release

if errorlevel 1 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo [SUCCESS] Build completed
echo.

REM Deploy Qt libraries (GUI only)
if "%BUILD_TYPE%"=="GUI" (
    echo [Step 3/4] Deploying Qt libraries...
    set WINDEPLOYQT=%QT_PATH%\bin\windeployqt.exe
    if exist "%WINDEPLOYQT%" (
        echo [DEBUG] Running windeployqt...
        "%WINDEPLOYQT%" bin\Release\ItemRecovery_GUI.exe
        echo [SUCCESS] Qt libraries deployed
    ) else (
        echo [WARNING] windeployqt tool not found
        echo Please refer to WINDOWS_DEPLOYMENT.md for details
    )
    echo.
)

REM Run program
echo [Step 4/4] Running program...
echo.

set EXE_PATH=
if "%BUILD_TYPE%"=="CLI" (
    if "%COMPILER%"=="MSVC" (
        set EXE_PATH=bin\Release\ItemRecovery_CLI.exe
    ) else (
        set EXE_PATH=bin\ItemRecovery_CLI.exe
    )
) else (
    if "%COMPILER%"=="MSVC" (
        set EXE_PATH=bin\Release\ItemRecovery_GUI.exe
    ) else (
        set EXE_PATH=bin\ItemRecovery_GUI.exe
    )
)

echo [DEBUG] Looking for executable: %EXE_PATH%
if exist "%EXE_PATH%" (
    echo [DEBUG] Executable found, starting...
    if "%BUILD_TYPE%"=="CLI" (
        for %%F in ("%EXE_PATH%") do cd /d "%%~dpF"
        "%EXE_PATH%"
    ) else (
        start "" "%EXE_PATH%"
    )
) else (
    echo [ERROR] Executable not found: %EXE_PATH%
    echo [DEBUG] Current directory: %CD%
    dir /b bin 2>nul
    dir /b bin\Release 2>nul
    pause
    exit /b 1
)

echo.
echo [DONE] Program started
echo.
echo Tips: If the program fails to run, please check:
echo   1. Qt DLLs are properly deployed
echo   2. Data directory exists
echo   3. Refer to WINDOWS_DEPLOYMENT.md for more help
echo.

pause

