# Windows 部署指南

本指南将帮助您在Windows系统上部署和运行ItemRecovery项目。

## 目录

1. [前置要求](#前置要求)
2. [安装步骤](#安装步骤)
3. [编译项目](#编译项目)
4. [运行程序](#运行程序)
5. [常见问题](#常见问题)
6. [快速开始](#快速开始)

---

## 前置要求

在开始之前，您需要安装以下软件：

1. **Qt 5.x** - GUI框架（GUI版本需要）
2. **CMake 3.16或更高版本** - 构建系统
3. **C++编译器** - 选择以下之一：
   - **Visual Studio 2019/2022** (推荐，包含MSVC编译器)
   - **MinGW-w64** (GCC编译器)

---

## 安装步骤

### 1. 安装Qt

#### 方法A：使用Qt官方安装程序（推荐）

1. 访问 [Qt官网下载页面](https://www.qt.io/download)
2. 下载Qt Online Installer
3. 运行安装程序，选择以下组件：
   - **Qt 5.15.x** 或 **Qt 5.14.x**（选择最新稳定版本）
   - **MSVC 2019 64-bit**（如果使用Visual Studio）
   - **MinGW 8.1.0 64-bit**（如果使用MinGW）
   - **Qt Creator**（可选，但推荐用于开发）

4. 安装完成后，记录Qt的安装路径（例如：`C:\Qt\5.15.2\msvc2019_64`）

#### 方法B：使用包管理器（高级用户）

```powershell
# 使用 Chocolatey
choco install qt5

# 或使用 vcpkg
vcpkg install qt5-base qt5-widgets qt5-gui
```

### 2. 安装CMake

#### 方法A：使用官方安装程序

1. 访问 [CMake官网](https://cmake.org/download/)
2. 下载Windows x64 Installer
3. 运行安装程序，**重要**：选择"Add CMake to system PATH"
4. 验证安装：
   ```cmd
   cmake --version
   ```

#### 方法B：使用包管理器

```powershell
# 使用 Chocolatey
choco install cmake

# 或使用 winget
winget install Kitware.CMake
```

### 3. 安装C++编译器

#### 选项1：Visual Studio（推荐）

1. 下载 [Visual Studio Community](https://visualstudio.microsoft.com/downloads/)
2. 安装时选择"使用C++的桌面开发"工作负载
3. 确保包含：
   - MSVC v142或v143编译器工具集
   - Windows 10/11 SDK

#### 选项2：MinGW-w64

1. 访问 [MinGW-w64官网](https://www.mingw-w64.org/downloads/)
2. 或使用 [MSYS2](https://www.msys2.org/)（推荐）：
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-cmake
   ```

---

## 编译项目

### 方法1：使用命令行（推荐）

#### 使用Visual Studio (MSVC)

1. 打开 **"x64 Native Tools Command Prompt for VS 2022"**（或对应版本的命令提示符）

2. 导航到项目目录：
   ```cmd
   cd C:\path\to\ItemRecovery
   ```

3. 创建构建目录并配置：
   ```cmd
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64
   ```
   > **注意**：将 `C:\Qt\5.15.2\msvc2019_64` 替换为您的Qt安装路径

4. 编译项目：
   ```cmd
   cmake --build . --config Release
   ```

#### 使用MinGW

1. 打开命令提示符（确保MinGW在PATH中）

2. 导航到项目目录：
   ```cmd
   cd C:\path\to\ItemRecovery
   ```

3. 创建构建目录并配置：
   ```cmd
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\mingw81_64
   ```
   > **注意**：将 `C:\Qt\5.15.2\mingw81_64` 替换为您的Qt安装路径

4. 编译项目：
   ```cmd
   cmake --build . --config Release
   ```

### 方法2：使用CMake GUI

1. 打开CMake GUI

2. 设置源目录：`C:\path\to\ItemRecovery`
3. 设置构建目录：`C:\path\to\ItemRecovery\build`
4. 点击"Configure"
5. 选择生成器（Visual Studio 17 2022 或 MinGW Makefiles）
6. 设置 `CMAKE_PREFIX_PATH` 为Qt安装路径（例如：`C:\Qt\5.15.2\msvc2019_64`）
7. 再次点击"Configure"
8. 点击"Generate"
9. 点击"Open Project"（Visual Studio）或在构建目录运行 `mingw32-make`

### 编译输出

编译成功后，可执行文件位于：
- **命令行版本**：`build\bin\Release\ItemRecovery_CLI.exe`
- **GUI版本**：`build\bin\Release\ItemRecovery_GUI.exe`

数据文件已自动复制到 `build\bin\data\` 目录。

---

## 运行程序

### 运行命令行版本

1. 打开命令提示符
2. 导航到构建目录的bin文件夹：
   ```cmd
   cd build\bin\Release
   ```
3. 运行程序：
   ```cmd
   ItemRecovery_CLI.exe
   ```

### 运行GUI版本

#### 方法1：直接运行（如果Qt DLL在PATH中）

1. 导航到 `build\bin\Release`
2. 双击 `ItemRecovery_GUI.exe` 或从命令行运行

#### 方法2：部署Qt库（推荐）

Windows上运行Qt程序需要Qt的DLL文件。有两种方法：

**方法A：使用windeployqt工具（推荐）**

1. 找到Qt的windeployqt工具（通常在 `C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe`）

2. 在命令提示符中运行：
   ```cmd
   cd build\bin\Release
   C:\Qt\5.15.2\msvc2019_64\bin\windeployqt.exe ItemRecovery_GUI.exe
   ```

3. 这将自动复制所需的Qt DLL到可执行文件目录

**方法B：手动复制DLL**

1. 从Qt安装目录复制以下DLL到 `build\bin\Release`：
   - `Qt5Core.dll`
   - `Qt5Gui.dll`
   - `Qt5Widgets.dll`
   - 以及相应的平台插件（`platforms\qwindows.dll`）

2. 创建 `platforms` 文件夹并复制 `qwindows.dll`

### 默认管理员账户

首次运行时，使用以下凭据登录：
- **用户名**：`admin`
- **密码**：`admin123`

---

## 常见问题

### 1. CMake找不到Qt

**错误信息**：
```
CMake Error: Could not find Qt5
```

**解决方法**：
- 确保设置了 `CMAKE_PREFIX_PATH` 指向Qt安装目录
- 检查Qt版本是否正确安装
- 使用完整路径：`cmake .. -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64`

### 2. 编译错误：找不到Qt头文件

**解决方法**：
- 检查 `CMAKE_PREFIX_PATH` 是否正确设置
- 确保安装了Qt的开发组件（不仅仅是运行时）

### 3. 运行时错误：找不到Qt DLL

**错误信息**：
```
The program can't start because Qt5Core.dll is missing
```

**解决方法**：
- 使用 `windeployqt` 工具部署Qt库
- 或将Qt的bin目录添加到系统PATH环境变量

### 4. MOC文件生成错误

**解决方法**：
- 确保 `CMAKE_AUTOMOC` 已启用（CMakeLists.txt中已设置）
- 清理构建目录并重新配置

### 5. 数据文件找不到

**解决方法**：
- 确保 `data` 目录存在于可执行文件同级目录
- CMakeLists.txt已配置自动复制数据目录
- 如果仍有问题，手动复制 `data` 文件夹到可执行文件目录

### 6. 使用MinGW时链接错误

**解决方法**：
- 确保Qt是用MinGW编译的版本
- 检查编译器版本兼容性
- 尝试使用Visual Studio编译器

---

## 快速开始

如果您已经安装了所有依赖，可以使用以下一键命令：

### Visual Studio用户

```cmd
# 在项目根目录执行
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64
cmake --build . --config Release
cd bin\Release
windeployqt ItemRecovery_GUI.exe
ItemRecovery_GUI.exe
```

### MinGW用户

```cmd
# 在项目根目录执行
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:\Qt\5.15.2\mingw81_64
cmake --build . --config Release
cd bin\Release
C:\Qt\5.15.2\mingw81_64\bin\windeployqt.exe ItemRecovery_GUI.exe
ItemRecovery_GUI.exe
```

---

## 额外提示

1. **开发环境**：推荐使用Qt Creator或Visual Studio作为IDE
2. **调试**：在Visual Studio中可以直接打开生成的 `.sln` 文件进行调试
3. **版本选择**：建议使用Release配置进行最终部署，Debug配置用于开发调试
4. **数据备份**：程序数据保存在 `data` 目录，建议定期备份

---

## 技术支持

如果遇到其他问题，请检查：
1. Qt版本是否与编译器匹配
2. CMake版本是否 >= 3.16
3. 编译器是否支持C++17标准
4. 所有依赖是否正确安装

祝您使用愉快！

