@echo off
REM ItemRecovery Windows 自动化构建和运行脚本
REM 使用方法: build_and_run.bat [CLI|GUI] [Qt路径]
REM 示例: build_and_run.bat GUI "C:\Qt\5.15.2\msvc2019_64"

setlocal enabledelayedexpansion

REM 默认参数
set BUILD_TYPE=GUI
set QT_PATH=
set COMPILER=MSVC

REM 解析命令行参数
if not "%~1"=="" set BUILD_TYPE=%~1
if not "%~2"=="" set QT_PATH=%~2

REM 检查CMake是否安装
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到CMake，请先安装CMake并添加到PATH
    echo 下载地址: https://cmake.org/download/
    pause
    exit /b 1
)

REM 如果没有提供Qt路径，尝试自动检测
if "%QT_PATH%"=="" (
    echo [信息] 未指定Qt路径，尝试自动检测...
    
    REM 检查常见Qt安装位置
    if exist "C:\Qt\5.15.2\msvc2019_64" (
        set QT_PATH=C:\Qt\5.15.2\msvc2019_64
        set COMPILER=MSVC
        echo [信息] 找到Qt (MSVC): %QT_PATH%
    ) else if exist "C:\Qt\5.15.2\mingw81_64" (
        set QT_PATH=C:\Qt\5.15.2\mingw81_64
        set COMPILER=MinGW
        echo [信息] 找到Qt (MinGW): %QT_PATH%
    ) else if exist "C:\Qt\5.14.2\msvc2019_64" (
        set QT_PATH=C:\Qt\5.14.2\msvc2019_64
        set COMPILER=MSVC
        echo [信息] 找到Qt (MSVC): %QT_PATH%
    ) else (
        echo [错误] 未找到Qt安装，请手动指定Qt路径
        echo 使用方法: build_and_run.bat %BUILD_TYPE% "C:\Qt\5.15.2\msvc2019_64"
        pause
        exit /b 1
    )
)

REM 检查Qt路径是否有效
if not exist "%QT_PATH%\bin\qmake.exe" (
    echo [错误] Qt路径无效: %QT_PATH%
    echo 请确保路径指向Qt安装目录（包含bin\qmake.exe）
    pause
    exit /b 1
)

echo.
echo ========================================
echo ItemRecovery 构建脚本
echo ========================================
echo 构建类型: %BUILD_TYPE%
echo Qt路径: %QT_PATH%
echo 编译器: %COMPILER%
echo ========================================
echo.

REM 创建构建目录
if not exist "build" (
    echo [信息] 创建构建目录...
    mkdir build
)

cd build

REM 根据编译器选择生成器
if "%COMPILER%"=="MSVC" (
    REM 检测Visual Studio版本
    where cl >nul 2>&1
    if %errorlevel% neq 0 (
        echo [警告] 未找到MSVC编译器，尝试使用MinGW...
        set COMPILER=MinGW
        set GENERATOR=MinGW Makefiles
    ) else (
        REM 检测VS版本
        for /f "tokens=*" %%i in ('where cl') do set CL_PATH=%%i
        echo %CL_PATH% | findstr /i "2022" >nul
        if %errorlevel% equ 0 (
            set GENERATOR=Visual Studio 17 2022
        ) else (
            echo %CL_PATH% | findstr /i "2019" >nul
            if %errorlevel% equ 0 (
                set GENERATOR=Visual Studio 16 2019
            ) else (
                set GENERATOR=Visual Studio 17 2022
            )
        )
    )
)

if "%COMPILER%"=="MinGW" (
    set GENERATOR=MinGW Makefiles
    REM 检查MinGW是否在PATH中
    where g++ >nul 2>&1
    if %errorlevel% neq 0 (
        echo [错误] 未找到MinGW编译器，请添加到PATH
        pause
        exit /b 1
    )
)

echo [信息] 使用生成器: %GENERATOR%
echo.

REM 配置CMake
echo [步骤 1/4] 配置CMake...
if "%COMPILER%"=="MSVC" (
    cmake .. -G "%GENERATOR%" -A x64 -DCMAKE_PREFIX_PATH="%QT_PATH%"
) else (
    cmake .. -G "%GENERATOR%" -DCMAKE_PREFIX_PATH="%QT_PATH%"
)

if %errorlevel% neq 0 (
    echo [错误] CMake配置失败
    pause
    exit /b 1
)

echo [成功] CMake配置完成
echo.

REM 编译项目
echo [步骤 2/4] 编译项目...
if "%COMPILER%"=="MSVC" (
    cmake --build . --config Release
) else (
    cmake --build . --config Release
)

if %errorlevel% neq 0 (
    echo [错误] 编译失败
    pause
    exit /b 1
)

echo [成功] 编译完成
echo.

REM 部署Qt库（仅GUI版本）
if "%BUILD_TYPE%"=="GUI" (
    echo [步骤 3/4] 部署Qt库...
    
    set WINDEPLOYQT=%QT_PATH%\bin\windeployqt.exe
    if exist "%WINDEPLOYQT%" (
        if "%COMPILER%"=="MSVC" (
            "%WINDEPLOYQT%" bin\Release\ItemRecovery_GUI.exe
        ) else (
            "%WINDEPLOYQT%" bin\Release\ItemRecovery_GUI.exe
        )
        echo [成功] Qt库部署完成
    ) else (
        echo [警告] 未找到windeployqt工具，可能需要手动部署Qt DLL
        echo 请参考 WINDOWS_DEPLOYMENT.md 文档
    )
    echo.
)

REM 运行程序
echo [步骤 4/4] 运行程序...
echo.

if "%BUILD_TYPE%"=="CLI" (
    if "%COMPILER%"=="MSVC" (
        if exist "bin\Release\ItemRecovery_CLI.exe" (
            cd bin\Release
            echo ========================================
            echo 启动命令行版本...
            echo ========================================
            echo.
            ItemRecovery_CLI.exe
        ) else (
            echo [错误] 未找到可执行文件: bin\Release\ItemRecovery_CLI.exe
            pause
            exit /b 1
        )
    ) else (
        if exist "bin\ItemRecovery_CLI.exe" (
            cd bin
            echo ========================================
            echo 启动命令行版本...
            echo ========================================
            echo.
            ItemRecovery_CLI.exe
        ) else (
            echo [错误] 未找到可执行文件: bin\ItemRecovery_CLI.exe
            pause
            exit /b 1
        )
    )
) else (
    if "%COMPILER%"=="MSVC" (
        if exist "bin\Release\ItemRecovery_GUI.exe" (
            cd bin\Release
            echo ========================================
            echo 启动GUI版本...
            echo ========================================
            echo.
            start ItemRecovery_GUI.exe
        ) else (
            echo [错误] 未找到可执行文件: bin\Release\ItemRecovery_GUI.exe
            pause
            exit /b 1
        )
    ) else (
        if exist "bin\ItemRecovery_GUI.exe" (
            cd bin
            echo ========================================
            echo 启动GUI版本...
            echo ========================================
            echo.
            start ItemRecovery_GUI.exe
        ) else (
            echo [错误] 未找到可执行文件: bin\ItemRecovery_GUI.exe
            pause
            exit /b 1
        )
    )
)

echo.
echo [完成] 程序已启动
echo.
echo 提示: 如果程序无法运行，请检查:
echo   1. Qt DLL是否正确部署
echo   2. 数据目录是否存在
echo   3. 参考 WINDOWS_DEPLOYMENT.md 获取更多帮助
echo.

pause

