cmake_minimum_required(VERSION 3.16)

# Define project metadata and force native C++ compilation tracking
project(RetroFuseQT VERSION 1.0.0 LANGUAGES CXX)

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
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "Checking dependencies inside submodule repositories...")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMODULE_RESULT
    )
endif()

# ==========================================
# 1. PLATFORM DEPENDENCY LOOKUPS
# ==========================================
# Locate target Qt6 components required for GUI windows and hardware rendering
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets)

# Locate SDL2 for native, low-latency physical desktop gamepad tracking [cite: 270]
find_package(SDL2 REQUIRED)

# ==========================================
# 2. SOURCE SPECIFICATION MATRICES
# ==========================================
# Collect implementation elements matching our established folder architecture
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
)

# Define the primary compilation build target payload
add_executable(${PROJECT_NAME} ${SOURCES})

# ==========================================
# 3. COMPILER ADJUSTMENTS & OPTIMIZATIONS
# ==========================================
if(WIN32)
    # Windows specific optimizations: Trim down legacy win32 headers
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
    
    # Hide the standard CMD debugging prompt terminal behind the Qt window on Windows launch [cite: 289]
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    
elseif(UNIX AND NOT APPLE)
    # Linux specific optimizations: Activate full warning flags and level-3 speed compiling
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O3)
endif()

# ==========================================
# 4. LIBRARY MOUNT LINKS
# ==========================================
# Statically couple libraries to the target executable runtime bubble
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::OpenGLWidgets
    SDL2::SDL2
)

# ==========================================
# 5. SUBMODULE COMPILATION TARGET HOOKS
# ==========================================

# A. Compile and Link EKA2L1 (Symbian Engine)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/eka2l1/CMakeLists.txt")
    # Tells CMake to enter the submodule folder and process its source targets
    add_subdirectory(third_party/eka2l1)
    
    # Statically embed the compiled EKA2L1 translation library engine directly 
    # inside your main executable target footprint
    target_link_libraries(${PROJECT_NAME} PRIVATE eka2l1_core)
    target_include_directories(${PROJECT_NAME} PRIVATE third_party/eka2l1/include)
endif()

# B. Build Configuration for FreeJ2ME (Java Engine Component)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/freej2me/CMakeLists.txt")
    # FreeJ2ME contains a combination of native elements and compiled Java code.
    # We add its sub-build tree here to generate its native library bridges.
    add_subdirectory(third_party/freej2me)
    target_link_libraries(${PROJECT_NAME} PRIVATE freej2me_bridge)
endif()

# ==========================================
# 6. SUBMODULE BACKEND MAPPINGS
# ==========================================
# When ready to compile the backend cores from the third_party directory, 
# uncomment the structural directives below to tie engine submodules together[cite: 289]:
#
# add_subdirectory(third_party/eka2l1)
# target_link_libraries(${PROJECT_NAME} PRIVATE eka2l1_core)