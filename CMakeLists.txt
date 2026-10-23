cmake_minimum_required(VERSION 3.24)

project(taffy
    VERSION 0.1.0
    DESCRIPTION "Qt Quick item library; layout math in Rust (taffy), enums via cxx-qt qenum"
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
# Qt 6
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
        Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2 Qt6::QuickPrivate
        PROPERTY MAP_IMPORTED_CONFIG_DEBUG "RELEASE")
endif()

# --------------------------------------------------------------------
# cxx-qt (enums / qenum) + Corrosion
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

# Public Qt modules only — private modules have no .prl (link them on C++ targets).
cxx_qt_import_crate(
    MANIFEST_PATH rust/Cargo.toml
    CRATES taffy_rust
    LOCKED
    QT_MODULES
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
)

# --------------------------------------------------------------------
# C++ library
# --------------------------------------------------------------------
add_library(taffy
    cpp/Taffy.cpp
    cpp/TaffyEngine.cpp
    cpp/TaffyItem.cpp
    cpp/TaffyLayout.cpp
    include/taffy/Taffy.h
    include/taffy/TaffyEngine.h
    include/taffy/TaffyItem.h
    include/taffy/TaffyLayout.h
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
        Qt6::QuickLayouts
        Qt6::QuickLayoutsPrivate
        taffy_rust
)

if(TAFFY_IS_TOP_LEVEL)
    add_subdirectory(examples/minimal)
endif()
