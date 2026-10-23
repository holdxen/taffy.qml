[English](README.md) | [简体中文](README.zh-CN.md)

# taffy.qml

CSS-style **Flexbox / Grid** layout for Qt Quick. Layout is computed by the Rust [taffy](https://github.com/DioxusLabs/taffy) engine; the QML/C++ side exposes `TaffyLayout` and attached properties close to CSS. Compared with Qt’s `RowLayout` / `GridLayout`, it is more expressive, and compared with `FlexboxLayout` it covers more of taffy’s style surface.

## ⚠️ Qt private API notice

This library depends on **Qt private modules / headers** (for example `QtQuickLayoutsPrivate`, `QQuickLayout`) in order to:

- Subclass `QQuickLayout` and join the Qt Quick layout system
- Use size hints, item change listeners, and other layout integration APIs

Please note:

1. **Some package builds omit private modules**  
   Channels such as **Homebrew** may ship Qt without `*Private` headers or CMake config, which breaks finding `QtQuickLayoutsPrivate` / private headers. Use a Qt install that includes private headers (official installer, full source build, or install `qtdeclarative` private components yourself).

2. **Qt version differences may break the build**  
   Private APIs have **no compatibility guarantee**. Signatures, headers, and layout behavior can change between Qt patch releases. This project is developed against **Qt 6.11**; other 6.x versions are not guaranteed to compile.

3. **Compatibility issues**  
   If the project fails to build, please open an Issue with: Qt version, how Qt was installed (brew / aqt / source / …), and the full error log. We will try to adapt where possible; otherwise the supported range will be documented.

## Quick start

```qml
import QtQuick
import Taffy 1.0

TaffyLayout {
    anchors.fill: parent
    display: "flex"
    flexDirection: "row"
    gapWidth: 8

    Rectangle {
        color: "salmon"
        TaffyLayout.height: "100px"
        TaffyLayout.flexShrink: 0
    }
    Rectangle {
        color: "steelblue"
        TaffyLayout.flexGrow: 1      // take remaining width
        TaffyLayout.height: "100px"
    }
}
```

Grid:

```qml
TaffyLayout {
    display: "grid"
    gridTemplateRows: "48px 1fr 48px"
    gridTemplateColumns: "120px 1fr 120px"

    Rectangle {
        TaffyLayout.gridRow: "1"
        TaffyLayout.gridColumn: "1 / 4"   // span 3 columns
    }
}
```

C++ entry:

```cpp
#include <taffy/Taffy.h>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    taffy::registerQmlTypes();   // registers Taffy.TaffyLayout
    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/main.qml"));
    return app.exec();
}
```

## Property model

```text
TaffyLayout (container — how children are arranged)
  display / flexDirection / flexWrap / flexLineCount
  alignItems / alignContent / justifyContent
  gapWidth / gapHeight
  gridTemplate* / gridAuto* / gridAutoFlow
  boxSizing / overflowX/Y / scrollbarWidth
  paddingTop/Right/Bottom/Left / direction

TaffyLayout attached (item — how a child sits in its parent)
  width / height / min* / max* (QString, CSS lengths)
  flexGrow / flexShrink / flexBasis
  alignSelf / justifySelf
  margin* / padding* / border*
  position / inset*
  gridRow / gridColumn
  cssFloat / clear / textAlign / …
```

When width/height are omitted, behavior matches CSS: `auto`, and may be expanded by `stretch` / `flex-grow`.

## Architecture

```text
QML / C++ properties (QString / qreal)
        ↓
taffy::Style (std::string, CSS)
        ↓  cxx
Rust: parse CSS → taffy::TaffyTree::compute_layout
        ↓
TaffyLayout::rearrange → setX/Y/Width/Height
```

| Path | Role |
|---|---|
| `include/taffy/`, `cpp/` | Qt/QML API, `QQuickLayout`, attached properties, debug dump |
| `rust/` | Pure **cxx** (no Qt), engine + CSS-string style |
| `examples/` | minimal / dump / grid |

Requirements: CMake ≥ 3.24, **Qt 6** (Qt 5 is not supported), Rust toolchain (to build `rust/`).

## Build examples

```bash
cmake -S . -B build
cmake --build build

./build/examples/minimal/taffy_example_minimal
./build/examples/grid/taffy_example_grid
./build/examples/dump/taffy_example_dump
```

## Use as a dependency (FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(taffy
    GIT_REPOSITORY https://github.com/holdxen/taffy.qml.git
    GIT_TAG main)
FetchContent_MakeAvailable(taffy)

target_link_libraries(app PRIVATE taffy::taffy Qt6::Quick)
```

```cpp
#include <taffy/Taffy.h>
taffy::registerQmlTypes();
```

> Consumers need Rust/cargo on the machine; Corrosion builds the layout engine during the CMake build.

## Compared with Qt FlexboxLayout

| | Qt `FlexboxLayout` | **taffy.qml** |
|---|---|---|
| Engine | Yoga (flex only) | taffy (flex + grid + …) |
| Style API | enums + qreal | **CSS strings** |
| Grid | no | templates / areas / span |
| Attached props | mostly `alignSelf` | full item style |
| Dependency | in Qt | Rust taffy + cxx |

Layout timing (`invalidate` → polish → rearrange) follows the `QQuickLayout` family, with extra hardening for invalid sizes and nested layouts.

## Debugging

```qml
Component.onCompleted: {
    console.log(layout.dump())                 // container properties
    console.log(JSON.stringify(item.TaffyLayout.taffyStyle))
    console.log(layout.dumpTree())             // container + all children attached
}
```

## License

MIT OR Apache-2.0
