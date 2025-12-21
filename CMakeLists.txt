cmake_minimum_required(VERSION 3.16)
project(ItemRecovery VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 确保数据目录存在
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

# 查找Qt5
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

# 启用Qt的MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 源文件目录
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# 公共源文件（不包含main.cpp和main_gui.cpp）
set(COMMON_SOURCES
    ${SRC_DIR}/interface.cpp
    ${SRC_DIR}/inventory.cpp
    ${SRC_DIR}/item.cpp
    ${SRC_DIR}/itemtype.cpp
    ${SRC_DIR}/user.cpp
)

# GUI特定源文件
set(GUI_SOURCES
    ${SRC_DIR}/mainwindow.cpp
    ${INCLUDE_DIR}/mainwindow.h
)

# 命令行版本可执行文件
add_executable(ItemRecovery_CLI
    ${COMMON_SOURCES}
    ${SRC_DIR}/main.cpp
)

target_compile_options(ItemRecovery_CLI PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# GUI版本可执行文件
add_executable(ItemRecovery_GUI
    ${COMMON_SOURCES}
    ${GUI_SOURCES}
    ${SRC_DIR}/main_gui.cpp
)

# 链接Qt库到GUI版本
target_link_libraries(ItemRecovery_GUI
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

target_compile_options(ItemRecovery_GUI PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# Windows特定设置
if(WIN32)
    # 复制数据目录到输出目录（考虑Release/Debug配置）
    add_custom_command(TARGET ItemRecovery_CLI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_CLI>/data
        COMMENT "复制数据目录到CLI输出目录"
    )
    
    add_custom_command(TARGET ItemRecovery_GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_GUI>/data
        COMMENT "复制数据目录到GUI输出目录"
    )
    
    # 对于Windows，可能需要部署Qt DLL
    # 可以使用windeployqt工具，但需要用户手动运行
    # 或者在这里添加自动部署（需要找到Qt的windeployqt工具）
endif()

# macOS特定设置
if(APPLE)
    # 复制数据目录
    add_custom_command(TARGET ItemRecovery_CLI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_CLI>/data
        COMMENT "复制数据目录到CLI输出目录"
    )
    
    add_custom_command(TARGET ItemRecovery_GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_GUI>/data
        COMMENT "复制数据目录到GUI输出目录"
    )
endif()

# Linux特定设置
if(UNIX AND NOT APPLE)
    # 复制数据目录
    add_custom_command(TARGET ItemRecovery_CLI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_CLI>/data
        COMMENT "复制数据目录到CLI输出目录"
    )
    
    add_custom_command(TARGET ItemRecovery_GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_GUI>/data
        COMMENT "复制数据目录到GUI输出目录"
    )
endif()

# 安装规则（可选）
install(TARGETS ItemRecovery_CLI ItemRecovery_GUI
    RUNTIME DESTINATION bin
)

install(DIRECTORY data/
    DESTINATION data
    FILES_MATCHING PATTERN "*.txt"
)

