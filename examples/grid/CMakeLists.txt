cmake_minimum_required(VERSION 3.24)
project(taffy_example_grid LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

add_executable(taffy_example_grid
    main.cpp
    main.qml
)

target_link_libraries(taffy_example_grid PRIVATE
    taffy::taffy
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)

target_compile_definitions(taffy_example_grid PRIVATE
    TAFFY_EXAMPLE_QML_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
