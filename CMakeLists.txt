cmake_minimum_required(VERSION 3.24)

project(taffy
    VERSION 0.1.0
    DESCRIPTION "Qt Quick item library; layout math in Rust (cxx + taffy)"
    LANGUAGES CXX
)

set(CMAKE_PREFIX_PATH "/usr/local/qt6")

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
# Qt 6 (C++ only — the Rust crate does not use Qt)
# --------------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    QuickPrivate
)

if(MSVC)
    set_property(TARGET
        Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2 Qt6::QuickPrivate
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

# cxx generated headers (lib.rs.h / lib.rs.cc glue already in the staticlib)
# corrosion_add_cxxbridge creates an interface with the right include path.
if(COMMAND corrosion_add_cxxbridge)
    corrosion_add_cxxbridge(taffy_cxxbridge
        CRATE taffy_rust
        FILES lib.rs
    )
else()
    # Fallback: locate OUT_DIR headers after first build via CARGO_TARGET_DIR
    set_target_properties(taffy_rust PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_BINARY_DIR}/corrosion")
endif()

# --------------------------------------------------------------------
# C++ library — owns all Qt API
# --------------------------------------------------------------------
add_library(taffy
    cpp/Taffy.cpp
    cpp/TaffyEngine.cpp
    cpp/TaffyItem.cpp
    include/taffy/Taffy.h
    include/taffy/TaffyEngine.h
    include/taffy/TaffyItem.h
    include/taffy/TaffyTypes.h
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
        taffy_rust
)

if(TARGET taffy_cxxbridge)
    target_link_libraries(taffy PRIVATE taffy_cxxbridge)
endif()

if(TAFFY_IS_TOP_LEVEL)
    add_subdirectory(examples/minimal)
endif()
