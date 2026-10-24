[English](README.md) | 简体中文

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

3. **兼容性问题反馈**  
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

## 属性参考

### TaffyLayout（容器）

控制子项如何排列。变更通过 `styleChanged` 通知。

| 属性 | QML 类型 | 默认值 | 取值 / 说明 |
|---|---|---|---|
| `display` | `string` | `"flex"` | `block`, `flow-root`, `flex`, `grid`, `none` |
| `flexDirection` | `string` | `"row"` | `row`, `column`, `row-reverse`, `column-reverse` |
| `flexWrap` | `string` | `"nowrap"` | `nowrap`, `wrap`, `wrap-reverse`, `balance`, `balance-reverse` |
| `flexLineCount` | `int` | `1` | ≥ 1（配合 balance 换行） |
| `alignItems` | `string` | `"normal"` | `normal`, `start`, `end`, `flex-start`, `flex-end`, `self-start`, `self-end`, `center`, `baseline`, `stretch` |
| `alignContent` | `string` | `"normal"` | `normal`, `start`, `end`, `flex-start`, `flex-end`, `center`, `stretch`, `space-between`, `space-evenly`, `space-around` |
| `justifyContent` | `string` | `"normal"` | 与 `alignContent` 相同 |
| `gapWidth` | `real` | `0` | 像素；`RESET` 恢复 0 |
| `gapHeight` | `real` | `0` | 像素；`RESET` 恢复 0 |
| `gridAutoFlow` | `string` | `"row"` | `row`, `column`, `row-dense`, `column-dense` |
| `gridTemplateRows` | `string` | `""` | CSS 轨道列表，如 `"48px 1fr"`、`"repeat(2, 100px)"` |
| `gridTemplateColumns` | `string` | `""` | 同行轨道写法 |
| `gridAutoRows` | `string` | `""` | 轨道尺寸，如 `"auto 100px"` |
| `gridAutoColumns` | `string` | `""` | 同 `gridAutoRows` |
| `gridTemplateAreas` | `string` | `""` | `"hd hd" "main side"` 或 `hd hd / main side` |
| `gridTemplateRowNames` | `stringlist` | `[]` | 每条轨道一项，如 `["start end", "line1"]` |
| `gridTemplateColumnNames` | `stringlist` | `[]` | 同行命名 |
| `boxSizing` | `string` | `"border-box"` | `border-box`, `content-box` |
| `overflowX` / `overflowY` | `string` | `"visible"` | `visible`, `clip`, `hidden`, `scroll` |
| `scrollbarWidth` | `real` | `0` | 像素 |
| `paddingTop` / `Right` / `Bottom` / `Left` | `real` | `0` | 像素 |
| `direction` | `string` | `"ltr"` | `ltr`, `rtl` |
| `taffyStyle` | `var`（`QVariantMap`） | — | 只读调试快照 |

### TaffyLayout.附加属性（子项）

写在 **`TaffyLayout` 的子元素** 上。变更通过 `itemStyleChanged` 通知。

| 属性 | QML 类型 | 默认值 | 取值 / 说明 |
|---|---|---|---|
| `width` / `height` | `string` | `"auto"` | 长度：`"100"`、`"100px"`、`"50%"`、`"auto"`、`"min-content"`、`"max-content"` |
| `minWidth` / `minHeight` | `string` | `"auto"` | 长度（语法同上） |
| `maxWidth` / `maxHeight` | `string` | `"auto"` | 长度 |
| `aspectRatio` | `real` | `NaN` | 宽/高比；`NaN` = 未设置 |
| `flexBasis` | `string` | `"auto"` | 长度：`"0"`、`"0%"`、`"auto"`、`"100px"` |
| `flexGrow` | `real` | `0` | |
| `flexShrink` | `real` | `1` | |
| `alignSelf` | `string` | `""`（未设置） | 空/`auto` = 未设置；否则同 `alignItems` 关键字 |
| `justifySelf` | `string` | `""`（未设置） | 空/`auto` = 未设置；关键字同 `alignSelf` |
| `marginTop` / `Right` / `Bottom` / `Left` | `real` | `0` | 像素 |
| `paddingTop` / `Right` / `Bottom` / `Left` | `real` | `0` | 像素 |
| `borderTop` / `Right` / `Bottom` / `Left` | `real` | `0` | 像素 |
| `position` | `string` | `"static"` | `static`, `relative`, `absolute`, `fixed`, `sticky` |
| `insetTop` / `Right` / `Bottom` / `Left` | `string` | `"auto"` | 长度：`"auto"`、`"8px"`、`"10%"` |
| `gridRow` | `string` | `"auto"` | `"auto"`、`"2"`、`"span 2"`、`"1 / 3"` |
| `gridColumn` | `string` | `"auto"` | 同 `gridRow` |
| `boxSizing` | `string` | `"border-box"` | `border-box`, `content-box` |
| `overflowX` / `overflowY` | `string` | `"visible"` | `visible`, `clip`, `hidden`, `scroll` |
| `cssFloat` | `string` | `"none"` | `none`, `left`, `right` |
| `clear` | `string` | `"none"` | `none`, `left`, `right`, `both` |
| `textAlign` | `string` | `"auto"` | `auto`, `left`, `right`, `center`（及 `legacy-left` 等） |
| `itemIsTable` | `bool` | `false` | |
| `itemIsReplaced` | `bool` | `false` | |
| `itemIsCompressibleReplaced` | `bool` | `false` | |
| `taffyStyle` | `var`（`QVariantMap`） | — | 只读调试快照 |

### 长度写法

| 示例 | 含义 |
|---|---|
| `"100"` / `"100px"` | 100 像素（裸数字按 px） |
| `"50%"` | 百分比 |
| `"auto"` / 不写 | 自动（可被 stretch / flex 撑开） |
| `"min-content"` / `"max-content"` | 内在尺寸关键字 |
| `"1fr"` / `"repeat(2, 100px)"` | 仅 grid 轨道 |

### 调试

```qml
layout.dump()                      // 容器属性文本
layout.taffyStyle                  // QVariantMap
item.TaffyLayout.dump()            // 子项附加属性文本
layout.dumpTree()                  // 容器 + 所有子项
```

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

## 许可

MIT OR Apache-2.0
