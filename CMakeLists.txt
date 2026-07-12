cmake_minimum_required(VERSION 3.16)

# Define project metadata and force native C++ compilation tracking
project(ClassicJems VERSION 1.0.0 LANGUAGES CXX)

# Enforce the C++17 standard (strictly required by modern Qt6 core architectures) [cite: 275, 278]
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic Qt Meta-Object, UI, and Resource compiler automation pipelines
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ==========================================
# AUTOMATIC SUBMODULE CORE INITIALIZATION
# ==========================================
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    # Only run submodule update if target engine directories are empty/missing
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/eka2l1/CMakeLists.txt" OR 
       NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/freej2me/CMakeLists.txt")
        message(STATUS "Third-party engine submodules are missing. Initializing submodules...")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMODULE_RESULT
        )
        if(NOT GIT_SUBMODULE_RESULT EQUAL 0)
            message(WARNING "Failed to automatically initialize git submodules. Please run 'git submodule update --init --recursive' manually.")
        else()
            message(STATUS "Submodules initialized successfully.")
        endif()
    endif()
endif()

# ==========================================
# 1. PLATFORM DEPENDENCY LOOKUPS
# ==========================================
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets Qml)
find_package(SDL2 REQUIRED)
find_package(PkgConfig REQUIRED)

# Optional desktop-specific lookups
find_package(LxQtPanel QUIET)
find_package(KF6Plasma QUIET)
if(NOT KF6Plasma_FOUND)
    find_package(KF5Plasma QUIET)
endif()

pkg_check_modules(XFCE4_PANEL QUIET libxfce4panel-2.0)
pkg_check_modules(GTK3 QUIET gtk+-3.0)

# Shared sources for Qt6 widgets and player
set(SHARED_SOURCES
    src/gamelibrarywidget.cpp
    src/gamelibrarywidget.h
    src/emulatorwindow.cpp
    src/emulatorwindow.h
)

# ==========================================
# 2. STANDALONE PLAYER TARGET
# ==========================================
add_executable(classicjems-player src/player/main.cpp ${SHARED_SOURCES})
target_link_libraries(classicjems-player PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::OpenGLWidgets
)

if(TARGET SDL2::SDL2)
    target_link_libraries(classicjems-player PRIVATE SDL2::SDL2)
else()
    target_include_directories(classicjems-player PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(classicjems-player PRIVATE ${SDL2_LIBRARIES})
endif()

if(UNIX AND NOT APPLE)
    target_compile_options(classicjems-player PRIVATE -Wall -Wextra -O3)
endif()

# ==========================================
# 3. LXQT PANEL PLUGIN TARGET
# ==========================================
if(LxQtPanel_FOUND)
    message(STATUS "LXQt Panel development files found. Building LXQt plugin...")
    add_library(plugin-classicjems SHARED
        src/lxqt-plugin/plugin-classicjems.cpp
        src/lxqt-plugin/plugin-classicjems.h
        ${SHARED_SOURCES}
    )
    target_link_libraries(plugin-classicjems PRIVATE
        LxQt::Panel
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
        Qt6::OpenGLWidgets
    )
    if(TARGET SDL2::SDL2)
        target_link_libraries(plugin-classicjems PRIVATE SDL2::SDL2)
    endif()

    configure_file(
        src/lxqt-plugin/classicjems.desktop.in
        ${CMAKE_CURRENT_BINARY_DIR}/classicjems.desktop
        @ONLY
    )
endif()

# ==========================================
# 4. KDE PLASMA PLASMOID TARGET
# ==========================================
if(KF6Plasma_FOUND OR KF5Plasma_FOUND)
    message(STATUS "KDE Plasma development files found. Building Plasmoid plugin...")
    add_library(classicjems_qmlplugin SHARED
        src/kde-plasmoid/classicjems_qmlplugin.cpp
        src/kde-plasmoid/classicjems_qmlplugin.h
        ${SHARED_SOURCES}
    )
    target_link_libraries(classicjems_qmlplugin PRIVATE
        Qt6::Core
        Qt6::Qml
        Qt6::Widgets
    )
endif()

# ==========================================
# 5. XFCE PANEL PLUGIN TARGET
# ==========================================
if(XFCE4_PANEL_FOUND AND GTK3_FOUND)
    message(STATUS "XFCE Panel & GTK3 development files found. Building XFCE plugin...")
    add_library(classicjems-xfce SHARED
        src/xfce-plugin/xfce_plugin.c
    )
    target_include_directories(classicjems-xfce PRIVATE
        ${XFCE4_PANEL_INCLUDE_DIRS}
        ${GTK3_INCLUDE_DIRS}
    )
    target_link_libraries(classicjems-xfce PRIVATE
        ${XFCE4_PANEL_LIBRARIES}
        ${GTK3_LIBRARIES}
    )

    configure_file(
        src/xfce-plugin/classicjems-xfce.desktop.in
        ${CMAKE_CURRENT_BINARY_DIR}/classicjems-xfce.desktop
        @ONLY
    )
endif()

# ==========================================
# 6. SUBMODULE COMPILATION TARGET HOOKS
# ==========================================

# A. Compile and Link EKA2L1 (Symbian Engine)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/eka2l1/CMakeLists.txt")
    add_subdirectory(third_party/eka2l1)
    target_link_libraries(classicjems-player PRIVATE eka2l1_core)
    target_include_directories(classicjems-player PRIVATE third_party/eka2l1/include)
endif()

# B. Build Configuration for FreeJ2ME (Java Engine Component)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/freej2me/CMakeLists.txt")
    add_subdirectory(third_party/freej2me)
    target_link_libraries(classicjems-player PRIVATE freej2me_bridge)
endif()