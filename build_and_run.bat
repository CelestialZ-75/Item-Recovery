@echo off
REM ItemRecovery Windows 自动化构建和运行脚本
REM 使用方法: build_and_run.bat [CLI|GUI] [Qt路径]
REM 示例: build_and_run.bat GUI "C:\Qt\5.15.2\msvc2019_64"

setlocal enabledelayedexpansion

REM 默认参数
set BUILD_TYPE=GUI
set QT_PATH=
set COMPILER=MSVC

REM 解析命令行参数 - 使用更安全的方法
set ARG1=%~1
set ARG2=%~2

if not "%ARG1%"=="" (
    set BUILD_TYPE=%ARG1%
)
if not "%ARG2%"=="" (
    set QT_PATH=%ARG2%
)

REM 检查CMake是否安装
where cmake >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake not found. Please install CMake and add it to PATH
    echo Download: https://cmake.org/download/
    pause
    exit /b 1
)

REM 如果没有提供Qt路径，尝试自动检测
if "%QT_PATH%"=="" (
    echo [INFO] Qt path not specified, attempting auto-detection...
    
    REM 检查常见Qt安装位置
    if exist "C:\Qt\5.15.2\msvc2019_64" (
        set QT_PATH=C:\Qt\5.15.2\msvc2019_64
        set COMPILER=MSVC
        echo [INFO] Found Qt (MSVC): %QT_PATH%
    ) else if exist "C:\Qt\5.15.2\mingw81_64" (
        set QT_PATH=C:\Qt\5.15.2\mingw81_64
        set COMPILER=MinGW
        echo [INFO] Found Qt (MinGW): %QT_PATH%
    ) else if exist "C:\Qt\5.14.2\msvc2019_64" (
        set QT_PATH=C:\Qt\5.14.2\msvc2019_64
        set COMPILER=MSVC
        echo [INFO] Found Qt (MSVC): %QT_PATH%
    ) else (
        echo [ERROR] Qt installation not found. Please specify Qt path manually
        echo Usage: build_and_run.bat %BUILD_TYPE% "C:\Qt\5.15.2\msvc2019_64"
        pause
        exit /b 1
    )
)

REM 检查Qt路径是否有效
if "%QT_PATH%"=="" (
    echo [ERROR] Qt path is empty
    pause
    exit /b 1
)
if not exist "%QT_PATH%\bin\qmake.exe" (
    echo [ERROR] Invalid Qt path: %QT_PATH%
    echo Please ensure the path points to Qt installation directory (contains bin\qmake.exe)
    pause
    exit /b 1
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

REM 创建构建目录
if not exist "build" (
    echo [INFO] Creating build directory...
    mkdir build
)

cd build

REM 根据编译器选择生成器
if "%COMPILER%"=="MSVC" (
    REM 检测Visual Studio版本
    where cl >nul 2>&1
    if errorlevel 1 (
        echo [WARNING] MSVC compiler not found, trying MinGW...
        set COMPILER=MinGW
        set GENERATOR=MinGW Makefiles
    ) else (
        REM 检测VS版本 - 简化版本检测
        REM 默认使用VS 2022，如果需要可以手动指定
        set GENERATOR=Visual Studio 17 2022
    )
)

if "%COMPILER%"=="MinGW" (
    set GENERATOR=MinGW Makefiles
    REM 检查MinGW是否在PATH中
    where g++ >nul 2>&1
    if errorlevel 1 (
        echo [ERROR] MinGW compiler not found. Please add it to PATH
        pause
        exit /b 1
    )
)

echo [INFO] Using generator: %GENERATOR%
echo.

REM 配置CMake
echo [Step 1/4] Configuring CMake...
if "%COMPILER%"=="MSVC" (
    cmake .. -G "%GENERATOR%" -A x64 -DCMAKE_PREFIX_PATH="%QT_PATH%"
) else (
    cmake .. -G "%GENERATOR%" -DCMAKE_PREFIX_PATH="%QT_PATH%"
)

if errorlevel 1 (
    echo [ERROR] CMake configuration failed
    pause
    exit /b 1
)

echo [SUCCESS] CMake configuration completed
echo.

REM 编译项目
echo [Step 2/4] Building project...
if "%COMPILER%"=="MSVC" (
    cmake --build . --config Release
) else (
    cmake --build . --config Release
)

if errorlevel 1 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo [SUCCESS] Build completed
echo.

REM 部署Qt库（仅GUI版本）
if "%BUILD_TYPE%"=="GUI" (
    echo [Step 3/4] Deploying Qt libraries...
    
    set WINDEPLOYQT=%QT_PATH%\bin\windeployqt.exe
    if exist "%WINDEPLOYQT%" (
        if "%COMPILER%"=="MSVC" (
            "%WINDEPLOYQT%" bin\Release\ItemRecovery_GUI.exe
        ) else (
            "%WINDEPLOYQT%" bin\Release\ItemRecovery_GUI.exe
        )
        echo [SUCCESS] Qt libraries deployed
    ) else (
        echo [WARNING] windeployqt tool not found, may need to deploy Qt DLLs manually
        echo Please refer to WINDOWS_DEPLOYMENT.md for details
    )
    echo.
)

REM 运行程序
echo [Step 4/4] Running program...
echo.

if "%BUILD_TYPE%"=="CLI" (
    if "%COMPILER%"=="MSVC" (
        if exist "bin\Release\ItemRecovery_CLI.exe" (
            cd bin\Release
            echo ========================================
            echo Starting CLI version...
            echo ========================================
            echo.
            ItemRecovery_CLI.exe
        ) else (
            echo [ERROR] Executable not found: bin\Release\ItemRecovery_CLI.exe
            pause
            exit /b 1
        )
    ) else (
        if exist "bin\ItemRecovery_CLI.exe" (
            cd bin
            echo ========================================
            echo Starting CLI version...
            echo ========================================
            echo.
            ItemRecovery_CLI.exe
        ) else (
            echo [ERROR] Executable not found: bin\ItemRecovery_CLI.exe
            pause
            exit /b 1
        )
    )
) else (
    if "%COMPILER%"=="MSVC" (
        if exist "bin\Release\ItemRecovery_GUI.exe" (
            cd bin\Release
            echo ========================================
            echo Starting GUI version...
            echo ========================================
            echo.
            start ItemRecovery_GUI.exe
        ) else (
            echo [ERROR] Executable not found: bin\Release\ItemRecovery_GUI.exe
            pause
            exit /b 1
        )
    ) else (
        if exist "bin\ItemRecovery_GUI.exe" (
            cd bin
            echo ========================================
            echo Starting GUI version...
            echo ========================================
            echo.
            start ItemRecovery_GUI.exe
        ) else (
            echo [ERROR] Executable not found: bin\ItemRecovery_GUI.exe
            pause
            exit /b 1
        )
    )
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

