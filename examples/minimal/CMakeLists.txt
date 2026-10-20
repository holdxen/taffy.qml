# Example app. Also documents how an external consumer FetchContent's taffy.
#
# External consumer equivalent:
#
#   include(FetchContent)
#   FetchContent_Declare(taffy
#       GIT_REPOSITORY <your-taffy-url>
#       GIT_TAG main)
#   FetchContent_MakeAvailable(taffy)
#
#   add_executable(app main.cpp)
#   target_link_libraries(app PRIVATE taffy::taffy)

cmake_minimum_required(VERSION 3.24)
project(taffy_example_minimal LANGUAGES CXX)

find_package(Qt6 QUIET COMPONENTS Core Gui Qml Quick QuickControls2)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)
    set(QT_PACKAGE Qt5)
else()
    set(QT_PACKAGE Qt6)
endif()

add_executable(taffy_example_minimal
    main.cpp
    main.qml
)

target_link_libraries(taffy_example_minimal PRIVATE
    taffy::taffy
    ${QT_PACKAGE}::Core
    ${QT_PACKAGE}::Gui
    ${QT_PACKAGE}::Qml
    ${QT_PACKAGE}::Quick
    ${QT_PACKAGE}::QuickControls2
)

# Make main.qml visible to the engine without a full qrc for the skeleton.
target_compile_definitions(taffy_example_minimal PRIVATE
    TAFFY_EXAMPLE_QML_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
