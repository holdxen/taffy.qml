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

## Property reference

### TaffyLayout (container)

How children are arranged. All properties notify via `styleChanged`.

| Property | QML type | Values / notes |
|---|---|---|
| `display` | `string` | `block`, `flow-root`, `flex`, `grid`, `none` |
| `flexDirection` | `string` | `row`, `column`, `row-reverse`, `column-reverse` |
| `flexWrap` | `string` | `nowrap`, `wrap`, `wrap-reverse`, `balance`, `balance-reverse` |
| `flexLineCount` | `int` | ≥ 1 (used with balance wraps) |
| `alignItems` | `string` | `normal`, `start`, `end`, `flex-start`, `flex-end`, `self-start`, `self-end`, `center`, `baseline`, `stretch` |
| `alignContent` | `string` | `normal`, `start`, `end`, `flex-start`, `flex-end`, `center`, `stretch`, `space-between`, `space-evenly`, `space-around` |
| `justifyContent` | `string` | same set as `alignContent` |
| `gapWidth` | `real` | px; `RESET` → 0 |
| `gapHeight` | `real` | px; `RESET` → 0 |
| `gridAutoFlow` | `string` | `row`, `column`, `row-dense`, `column-dense` |
| `gridTemplateRows` | `string` | CSS track list, e.g. `"48px 1fr"`, `"repeat(2, 100px)"`, `"minmax(80px, 1fr)"` |
| `gridTemplateColumns` | `string` | same form as rows |
| `gridAutoRows` | `string` | track sizes, e.g. `"auto 100px"` |
| `gridAutoColumns` | `string` | same form as auto rows |
| `gridTemplateAreas` | `string` | `"hd hd" "main side"` or `hd hd / main side` |
| `gridTemplateRowNames` | `stringlist` | one entry per track, e.g. `["start end", "line1"]` |
| `gridTemplateColumnNames` | `stringlist` | same as row names |
| `boxSizing` | `string` | `border-box`, `content-box` |
| `overflowX` / `overflowY` | `string` | `visible`, `clip`, `hidden`, `scroll` |
| `scrollbarWidth` | `real` | px |
| `paddingTop` / `Right` / `Bottom` / `Left` | `real` | px |
| `direction` | `string` | `ltr`, `rtl` |
| `taffyStyle` | `var` (`QVariantMap`) | read-only debug snapshot |

### TaffyLayout.<var> attached (item)

Set on **children** of `TaffyLayout`. Notify via `itemStyleChanged`.

| Property | QML type | Values / notes |
|---|---|---|
| `width` / `height` | `string` | length: `"100"`, `"100px"`, `"50%"`, `"auto"`, `"min-content"`, `"max-content"` |
| `minWidth` / `minHeight` | `string` | length (same syntax) |
| `maxWidth` / `maxHeight` | `string` | length |
| `aspectRatio` | `real` | width/height ratio; `NaN` = unset |
| `flexBasis` | `string` | length: `"0"`, `"0%"`, `"auto"`, `"100px"` |
| `flexGrow` | `real` | default `0` |
| `flexShrink` | `real` | default `1` |
| `alignSelf` | `string` | empty/`auto` = unset; else `normal`, `start`, `center`, `stretch`, … (same as `alignItems`) |
| `justifySelf` | `string` | empty/`auto` = unset; same keywords as `alignSelf` |
| `marginTop` / `Right` / `Bottom` / `Left` | `real` | px |
| `paddingTop` / `Right` / `Bottom` / `Left` | `real` | px |
| `borderTop` / `Right` / `Bottom` / `Left` | `real` | px |
| `position` | `string` | `static`, `relative`, `absolute`, `fixed`, `sticky` |
| `insetTop` / `Right` / `Bottom` / `Left` | `string` | length: `"auto"`, `"8px"`, `"10%"` |
| `gridRow` | `string` | `"auto"`, `"2"`, `"span 2"`, `"1 / 3"` |
| `gridColumn` | `string` | same form as `gridRow` |
| `boxSizing` | `string` | `border-box`, `content-box` |
| `overflowX` / `overflowY` | `string` | `visible`, `clip`, `hidden`, `scroll` |
| `cssFloat` | `string` | `none`, `left`, `right` |
| `clear` | `string` | `none`, `left`, `right`, `both` |
| `textAlign` | `string` | `auto`, `left`, `right`, `center` (and `legacy-left` etc.) |
| `itemIsTable` | `bool` | default `false` |
| `itemIsReplaced` | `bool` | default `false` |
| `itemIsCompressibleReplaced` | `bool` | default `false` |
| `taffyStyle` | `var` (`QVariantMap`) | read-only debug snapshot |

### Length syntax

| Example | Meaning |
|---|---|
| `"100"` / `"100px"` | 100 pixels (bare number = px) |
| `"50%"` | percentage |
| `"auto"` / unset | automatic (stretch / flex may expand) |
| `"min-content"` / `"max-content"` | intrinsic keyword |
| `"1fr"` / `"repeat(2, 100px)"` | grid tracks only |

### Debug

```qml
layout.dump()                      // container property dump (string)
layout.taffyStyle                  // QVariantMap
item.TaffyLayout.dump()            // attached property dump
layout.dumpTree()                  // container + all children
```

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

## License

MIT OR Apache-2.0
