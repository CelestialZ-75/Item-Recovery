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
    
    REM 检查常见Qt安装位置（按优先级排序）
    if exist "C:\Qt\5.15.2\msvc2019_64" (
        set QT_PATH=C:\Qt\5.15.2\msvc2019_64
        set COMPILER=MSVC
        echo [DEBUG] Found Qt (MSVC): %QT_PATH%
    ) else if exist "C:\Qt\5.15.2\msvc2022_64" (
        set QT_PATH=C:\Qt\5.15.2\msvc2022_64
        set COMPILER=MSVC
        echo [DEBUG] Found Qt (MSVC): %QT_PATH%
    ) else if exist "C:\Qt\6.5.0\msvc2019_64" (
        set QT_PATH=C:\Qt\6.5.0\msvc2019_64
        set COMPILER=MSVC
        echo [DEBUG] Found Qt (MSVC): %QT_PATH%
    ) else if exist "C:\Qt\6.5.0\msvc2022_64" (
        set QT_PATH=C:\Qt\6.5.0\msvc2022_64
        set COMPILER=MSVC
        echo [DEBUG] Found Qt (MSVC): %QT_PATH%
    ) else if exist "C:\Qt\5.14.2\msvc2019_64" (
        set QT_PATH=C:\Qt\5.14.2\msvc2019_64
        set COMPILER=MSVC
        echo [DEBUG] Found Qt (MSVC): %QT_PATH%
    ) else if exist "C:\Qt\5.15.2\mingw81_64" (
        set QT_PATH=C:\Qt\5.15.2\mingw81_64
        set COMPILER=MinGW
        echo [DEBUG] Found Qt (MinGW): %QT_PATH%
    ) else if exist "C:\Qt\6.5.0\mingw_64" (
        set QT_PATH=C:\Qt\6.5.0\mingw_64
        set COMPILER=MinGW
        echo [DEBUG] Found Qt (MinGW): %QT_PATH%
    ) else (
        echo [ERROR] Qt installation not found. Please specify Qt path manually
        echo.
        echo Usage: build_and_run_debug.bat [DEBUG] [CLI^|GUI] "C:\Qt\5.15.2\msvc2019_64"
        echo.
        echo Common Qt installation paths:
        echo   - C:\Qt\5.15.2\msvc2019_64
        echo   - C:\Qt\5.15.2\mingw81_64
        echo   - C:\Qt\6.5.0\msvc2022_64
        echo.
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
        echo [WARNING] MSVC compiler not found in PATH
        echo [DEBUG] Attempting to setup MSVC environment...
        
        REM 尝试查找并运行vcvarsall.bat
        set VS_FOUND=0
        if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
            echo [DEBUG] Found VS 2022 Community, setting up environment...
            call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
            set VS_FOUND=1
            set GENERATOR=Visual Studio 17 2022
        ) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
            echo [DEBUG] Found VS 2022 Professional, setting up environment...
            call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
            set VS_FOUND=1
            set GENERATOR=Visual Studio 17 2022
        ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
            echo [DEBUG] Found VS 2019 Community, setting up environment...
            call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
            set VS_FOUND=1
            set GENERATOR=Visual Studio 16 2019
        ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
            echo [DEBUG] Found VS 2019 Professional, setting up environment...
            call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
            set VS_FOUND=1
            set GENERATOR=Visual Studio 16 2019
        )
        
        if !VS_FOUND!==0 (
            echo [WARNING] Could not setup MSVC environment, trying MinGW...
            set COMPILER=MinGW
            set GENERATOR=MinGW Makefiles
        ) else (
            echo [DEBUG] MSVC environment setup successful
        )
    ) else (
        echo [DEBUG] MSVC compiler found, detecting version...
        if exist "C:\Program Files\Microsoft Visual Studio\2022" (
            set GENERATOR=Visual Studio 17 2022
        ) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019" (
            set GENERATOR=Visual Studio 16 2019
        ) else (
            set GENERATOR=Visual Studio 17 2022
        )
        echo [DEBUG] Using generator: %GENERATOR%
    )
)

if "%COMPILER%"=="MinGW" (
    if "%GENERATOR%"=="" (
        set GENERATOR=MinGW Makefiles
    )
    where g++ >nul 2>&1
    if errorlevel 1 (
        echo [ERROR] MinGW compiler not found. Please add it to PATH
        echo Or run this script from Visual Studio Developer Command Prompt
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
    
    REM 确定可执行文件路径
    set EXE_PATH=
    if "%COMPILER%"=="MSVC" (
        set EXE_PATH=bin\Release\ItemRecovery_GUI.exe
        if not exist "!EXE_PATH!" (
            set EXE_PATH=bin\Debug\ItemRecovery_GUI.exe
        )
    ) else (
        set EXE_PATH=bin\ItemRecovery_GUI.exe
    )
    
    echo [DEBUG] Looking for executable: !EXE_PATH!
    if exist "!EXE_PATH!" (
        set WINDEPLOYQT=%QT_PATH%\bin\windeployqt.exe
        echo [DEBUG] windeployqt path: %WINDEPLOYQT%
        if exist "%WINDEPLOYQT%" (
            echo [DEBUG] Running windeployqt for: !EXE_PATH!
            "%WINDEPLOYQT%" "!EXE_PATH!"
            if errorlevel 1 (
                echo [WARNING] windeployqt failed, but continuing...
            ) else (
                echo [SUCCESS] Qt libraries deployed
            )
        ) else (
            echo [WARNING] windeployqt tool not found at: %WINDEPLOYQT%
            echo Please refer to WINDOWS_DEPLOYMENT.md for details
        )
    ) else (
        echo [WARNING] Executable not found for deployment: !EXE_PATH!
    )
    echo.
)

REM Run program
echo [Step 4/4] Running program...
echo.

REM 确定可执行文件路径和名称
set EXE_NAME=
set EXE_DIR=
if "%BUILD_TYPE%"=="CLI" (
    set EXE_NAME=ItemRecovery_CLI.exe
) else (
    set EXE_NAME=ItemRecovery_GUI.exe
)

REM 根据编译器确定路径
if "%COMPILER%"=="MSVC" (
    REM MSVC使用多配置生成器，先尝试Release，再尝试Debug
    if exist "bin\Release\%EXE_NAME%" (
        set EXE_DIR=bin\Release
    ) else if exist "bin\Debug\%EXE_NAME%" (
        set EXE_DIR=bin\Debug
    ) else (
        echo [ERROR] Executable not found in bin\Release\ or bin\Debug\
        echo [DEBUG] Current directory: %CD%
        echo [DEBUG] Searching for executable...
        dir /s /b %EXE_NAME% 2>nul
        echo [DEBUG] Available files in bin:
        if exist "bin" dir /b /s bin 2>nul
        pause
        exit /b 1
    )
) else (
    REM MinGW使用单配置生成器
    if exist "bin\%EXE_NAME%" (
        set EXE_DIR=bin
    ) else (
        echo [ERROR] Executable not found: bin\%EXE_NAME%
        echo [DEBUG] Current directory: %CD%
        echo [DEBUG] Searching for executable...
        dir /s /b %EXE_NAME% 2>nul
        echo [DEBUG] Available files in bin:
        if exist "bin" dir /b /s bin 2>nul
        pause
        exit /b 1
    )
)

echo [DEBUG] Executable directory: %EXE_DIR%
echo [DEBUG] Executable name: %EXE_NAME%

REM 切换到可执行文件目录并运行
cd /d "%EXE_DIR%"
if errorlevel 1 (
    echo [ERROR] Failed to change to directory: %EXE_DIR%
    pause
    exit /b 1
)

echo ========================================
if "%BUILD_TYPE%"=="CLI" (
    echo Starting CLI version...
) else (
    echo Starting GUI version...
)
echo ========================================
echo Executable: %CD%\%EXE_NAME%
echo ========================================
echo.

if "%BUILD_TYPE%"=="CLI" (
    echo [DEBUG] Running CLI executable...
    %EXE_NAME%
) else (
    echo [DEBUG] Starting GUI executable in new window...
    start "" "%EXE_NAME%"
    echo [INFO] GUI application started in new window
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

