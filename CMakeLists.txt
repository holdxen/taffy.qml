cmake_minimum_required(VERSION 3.24)

project(taffy
    VERSION 0.1.0
    DESCRIPTION "Qt Quick item library; layout math in Rust (taffy + cxx), CSS-string style"
    LANGUAGES CXX
)

if(NOT DEFINED PROJECT_NAME OR CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(TAFFY_IS_TOP_LEVEL ON)
else()
    set(TAFFY_IS_TOP_LEVEL OFF)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# --------------------------------------------------------------------
# Qt 6 (C++ only)
# --------------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    QuickPrivate
    QuickLayouts
    QuickLayoutsPrivate
)

if(MSVC)
    set_property(TARGET
        Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2
        Qt6::QuickPrivate Qt6::QuickLayouts Qt6::QuickLayoutsPrivate
        PROPERTY MAP_IMPORTED_CONFIG_DEBUG "RELEASE")
endif()

# --------------------------------------------------------------------
# Rust staticlib via Corrosion + cxx (no cxx-qt)
# --------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.5.2
)
FetchContent_MakeAvailable(Corrosion)

corrosion_import_crate(
    MANIFEST_PATH rust/Cargo.toml
    CRATES taffy_rust
    LOCKED
)

if(COMMAND corrosion_add_cxxbridge)
    corrosion_add_cxxbridge(taffy_cxxbridge
        CRATE taffy_rust
        FILES lib.rs
    )
endif()

# --------------------------------------------------------------------
# C++ library
# --------------------------------------------------------------------
add_library(taffy
    cpp/Taffy.cpp
    cpp/TaffyEngine.cpp
    cpp/TaffyLayout.cpp
    cpp/TaffyLayoutAttached.cpp
    include/taffy/Taffy.h
    include/taffy/TaffyEngine.h
    include/taffy/TaffyLayout.h
    include/taffy/TaffyLayoutAttached.h
)
add_library(taffy::taffy ALIAS taffy)

target_include_directories(taffy
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(taffy
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
    PRIVATE
        Qt6::QuickPrivate
        Qt6::QuickLayouts
        Qt6::QuickLayoutsPrivate
        taffy_rust
)

if(TARGET taffy_cxxbridge)
    target_link_libraries(taffy PRIVATE taffy_cxxbridge)
endif()

if(TAFFY_IS_TOP_LEVEL)
    add_subdirectory(examples/minimal)
    add_subdirectory(examples/dump)
    add_subdirectory(examples/grid)
    add_subdirectory(examples/nested)
endif()
