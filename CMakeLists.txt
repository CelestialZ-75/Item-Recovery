cmake_minimum_required(VERSION 3.16)

# Windows上检查生成器
if(WIN32)
    if(CMAKE_GENERATOR MATCHES "NMake")
        message(FATAL_ERROR 
            "NMake generator requires Visual Studio environment setup.\n"
            "Please use the provided build_and_run.bat script or setup VS environment."
        )
    endif()
endif()

project(ItemRecovery VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

# 查找Qt6
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

if(CMAKE_PREFIX_PATH)
    # 强制刷新缓存中的 Qt6_DIR，以防残留旧路径
    unset(Qt6_DIR CACHE)
    set(Qt6_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt6" CACHE PATH "Qt6 directory" FORCE)
endif()

# 查找 Qt6
find_package(Qt6 COMPONENTS Core Widgets Gui)

if(Qt6_FOUND)
    message(STATUS "Found Qt6 Version: ${Qt6_VERSION}")
    set(QT_VERSION_MAJOR 6)
else()
    message(WARNING "Qt6 not found or check failed. Trying Qt5...")
    # 只有当Qt6真的没找到时才找Qt5
    if(CMAKE_PREFIX_PATH)
        set(Qt5_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5" CACHE PATH "Qt5 directory")
    endif()
    find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
    message(STATUS "Found Qt5")
    set(QT_VERSION_MAJOR 5)
endif()

# 启用Qt的MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(COMMON_SOURCES
    ${SRC_DIR}/interface.cpp
    ${SRC_DIR}/inventory.cpp
    ${SRC_DIR}/item.cpp
    ${SRC_DIR}/itemtype.cpp
    ${SRC_DIR}/user.cpp
)

set(GUI_SOURCES
    ${SRC_DIR}/mainwindow.cpp
    ${INCLUDE_DIR}/mainwindow.h
)

# CLI版本
add_executable(ItemRecovery_CLI
    ${COMMON_SOURCES}
    ${SRC_DIR}/main.cpp
)

target_compile_options(ItemRecovery_CLI PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# GUI版本
add_executable(ItemRecovery_GUI
    ${COMMON_SOURCES}
    ${GUI_SOURCES}
    ${SRC_DIR}/main_gui.cpp
)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(ItemRecovery_GUI
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
    )
else()
    target_link_libraries(ItemRecovery_GUI
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
    )
endif()

target_compile_options(ItemRecovery_GUI PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# 复制数据文件
if(WIN32)
    add_custom_command(TARGET ItemRecovery_CLI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_CLI>/data
        COMMENT "Copying data for CLI"
    )
    add_custom_command(TARGET ItemRecovery_GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_GUI>/data
        COMMENT "Copying data for GUI"
    )
elseif(APPLE OR UNIX)
    add_custom_command(TARGET ItemRecovery_CLI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_CLI>/data
        COMMENT "Copying data for CLI"
    )
    add_custom_command(TARGET ItemRecovery_GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:ItemRecovery_GUI>/data
        COMMENT "Copying data for GUI"
    )
endif()