# taffy.qml

用 **CSS 语义** 给 Qt Quick 做 **Flexbox / Grid 布局** 的库。布局计算由 Rust 的 [taffy](https://github.com/DioxusLabs/taffy) 引擎完成，QML/C++ 侧提供 `TaffyLayout` 与附加属性，风格接近 CSS，比 Qt 自带的 `RowLayout` / `GridLayout` 更灵活，也比 `FlexboxLayout` 覆盖更完整的 taffy 样式能力。

## ⚠️ Qt 私有 API 说明

本库依赖 **Qt 私有模块 / 私有头文件**（例如 `QtQuickLayoutsPrivate`、`QQuickLayout` 等），主要用于：

- 继承 `QQuickLayout`，接入 Qt Quick 布局体系
- 读取 size hint、子项监听等与 Qt 布局协同的能力

请注意：

1. **包管理器可能不带私有模块**  
   例如 **Homebrew** 等发行渠道打包的 Qt 有时缺少 `*Private` 头或 CMake 配置，会导致找不到 `QtQuickLayoutsPrivate` / 私有头文件。  
   请使用带 private headers 的 Qt（官方安装器、完整源码构建，或自行安装 `qtdeclarative` 等私有组件）。

2. **Qt 版本差异可能导致编译失败**  
   私有 API **无兼容承诺**，不同 Qt 小版本都可能改签名、删头文件或改布局行为。当前以 **Qt 6.11** 为参考开发；其他 6.x 版本不保证能编过。

3. **编译不过欢迎提 Issue**  
   请附上：Qt 版本、安装方式（brew / aqt / 源码…）、完整报错。我们会尽量适配；无法适配时会在文档中标明支持范围。

## 快速上手

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
        TaffyLayout.flexGrow: 1      // 占满剩余宽度
        TaffyLayout.height: "100px"
    }
}
```

Grid：

```qml
TaffyLayout {
    display: "grid"
    gridTemplateRows: "48px 1fr 48px"
    gridTemplateColumns: "120px 1fr 120px"

    Rectangle {
        TaffyLayout.gridRow: "1"
        TaffyLayout.gridColumn: "1 / 4"   // 跨 3 列
    }
}
```

C++ 入口：

```cpp
#include <taffy/Taffy.h>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    taffy::registerQmlTypes();   // 注册 Taffy.TaffyLayout
    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/main.qml"));
    return app.exec();
}
```

## 属性模型

```text
TaffyLayout（容器，控制子项怎么排）
  display / flexDirection / flexWrap / flexLineCount
  alignItems / alignContent / justifyContent
  gapWidth / gapHeight
  gridTemplate* / gridAuto* / gridAutoFlow
  boxSizing / overflowX/Y / scrollbarWidth
  paddingTop/Right/Bottom/Left / direction

TaffyLayout.附加（子项，在父里怎么摆）
  width / height / min* / max*（QString，CSS 长度）
  flexGrow / flexShrink / flexBasis
  alignSelf / justifySelf
  margin* / padding* / border*
  position / inset*
  gridRow / gridColumn
  cssFloat / clear / textAlign / …
```

未设置宽高时行为与 CSS 一致：`auto`，可被 `stretch` / `flex-grow` 撑开。

## 架构

```text
QML / C++ 属性 (QString / qreal)
        ↓
taffy::Style (std::string, CSS)
        ↓  cxx
Rust: 解析 CSS → taffy::TaffyTree::compute_layout
        ↓
TaffyLayout::rearrange → setX/Y/Width/Height
```

| 目录 | 职责 |
|---|---|
| `include/taffy/` `cpp/` | Qt/QML API、`QQuickLayout`、附加属性、调试 dump |
| `rust/` | 纯 **cxx**（无 Qt），导出引擎与 CSS 字符串 Style |
| `examples/` | minimal / dump / grid |

依赖：CMake ≥ 3.24、**Qt 6**（不支持 Qt 5）、Rust 工具链（编译 `rust/`）。

## 构建示例

```bash
cmake -S . -B build
cmake --build build

./build/examples/minimal/taffy_example_minimal
./build/examples/grid/taffy_example_grid
./build/examples/dump/taffy_example_dump
```

## 作为依赖（FetchContent）

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

> 消费方需要本机有 Rust/cargo；Corrosion 会在构建时编译布局引擎。

## 与 Qt FlexboxLayout 对比

| | Qt `FlexboxLayout` | **taffy.qml** |
|---|---|---|
| 引擎 | Yoga（仅 Flex） | taffy（Flex + Grid + …） |
| 样式 API | 枚举 + qreal | **CSS 字符串** |
| Grid | 无 | 完整模板 / area / span |
| 附加属性 | 基本只有 `alignSelf` | 完整子项样式 |
| 依赖 | Qt 内置 | Rust taffy + cxx |

布局时机（`invalidate` → polish → rearrange）与 `QQuickLayout` 族一致，并对非法尺寸、嵌套 layout 等做了加固。

## 调试

```qml
Component.onCompleted: {
    console.log(layout.dump())                 // 容器属性
    console.log(JSON.stringify(item.TaffyLayout.taffyStyle))
    console.log(layout.dumpTree())             // 容器 + 所有子项附加属性
}
```

## 许可

MIT OR Apache-2.0
