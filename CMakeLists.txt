cmake_minimum_required(VERSION 3.24)

project(taffy
    VERSION 0.1.0
    DESCRIPTION "Qt Quick item library with Rust (cxx-qt) layout engine"
    LANGUAGES CXX
)

# When consumed via FetchContent / add_subdirectory, don't pollute the parent.
if(NOT DEFINED PROJECT_NAME OR CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(TAFFY_IS_TOP_LEVEL ON)
else()
    set(TAFFY_IS_TOP_LEVEL OFF)
endif()

# Rust always links the non-debug Windows runtime on *-msvc.
# Must be consistent across the whole program.
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
find_package(Qt6 QUIET COMPONENTS Core Gui Qml Quick QuickControls2)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)
    set(TAFFY_QT_PACKAGE Qt5)
else()
    set(TAFFY_QT_PACKAGE Qt6)
endif()

if(MSVC)
    set_property(TARGET
        ${TAFFY_QT_PACKAGE}::Core
        ${TAFFY_QT_PACKAGE}::Gui
        ${TAFFY_QT_PACKAGE}::Qml
        ${TAFFY_QT_PACKAGE}::Quick
        ${TAFFY_QT_PACKAGE}::QuickControls2
        PROPERTY MAP_IMPORTED_CONFIG_DEBUG "RELEASE")
endif()

# --------------------------------------------------------------------
# CXX-Qt (Corrosion wrappers)
# Prefer an installed package; fall back to FetchContent.
# --------------------------------------------------------------------
find_package(CxxQt QUIET)
if(NOT CxxQt_FOUND)
    include(FetchContent)
    FetchContent_Declare(CxxQt
        GIT_REPOSITORY https://github.com/kdab/cxx-qt-cmake.git
        GIT_TAG 0.10.0
    )
    FetchContent_MakeAvailable(CxxQt)
endif()

# --------------------------------------------------------------------
# Rust crate (staticlib + generated C++/headers via cxx-qt)
# --------------------------------------------------------------------
cxx_qt_import_crate(
    MANIFEST_PATH rust/Cargo.toml
    CRATES taffy_rust
    LOCKED
    QT_MODULES
        ${TAFFY_QT_PACKAGE}::Core
        ${TAFFY_QT_PACKAGE}::Gui
        ${TAFFY_QT_PACKAGE}::Qml
        ${TAFFY_QT_PACKAGE}::Quick
        ${TAFFY_QT_PACKAGE}::QuickControls2
)

# --------------------------------------------------------------------
# C++ library: QQuickItem shell + public headers
# --------------------------------------------------------------------
add_library(taffy
    cpp/Taffy.cpp
    cpp/TaffyItem.cpp
    include/taffy/Taffy.h
    include/taffy/TaffyItem.h
)
add_library(taffy::taffy ALIAS taffy)

target_include_directories(taffy
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(taffy
    PUBLIC
        ${TAFFY_QT_PACKAGE}::Core
        ${TAFFY_QT_PACKAGE}::Gui
        ${TAFFY_QT_PACKAGE}::Qml
        ${TAFFY_QT_PACKAGE}::Quick
    PRIVATE
        taffy_rust
)

# Generated cxx-qt headers live under the rust crate export dir.
# Expose them only to this library's sources (not the public API).
target_include_directories(taffy
    PRIVATE
        $<TARGET_PROPERTY:taffy_rust,INTERFACE_INCLUDE_DIRECTORIES>
)

# --------------------------------------------------------------------
# Example (only when built as top-level)
# --------------------------------------------------------------------
if(TAFFY_IS_TOP_LEVEL)
    add_subdirectory(examples/minimal)
endif()
