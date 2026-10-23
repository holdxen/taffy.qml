# taffy

C++ / Qt Quick 库骨架：CMake + FetchContent 分发，Rust（cxx-qt）做逻辑层，C++ 做 `QQuickItem` 壳。

## 结构

```text
taffy/
├── CMakeLists.txt           # 导出 taffy::taffy
├── include/taffy/           # 公共 C++ 头
│   ├── Taffy.h              # registerQmlTypes()
│   └── TaffyItem.h          # QQuickItem 子类
├── cpp/
│   ├── Taffy.cpp
│   └── TaffyItem.cpp
├── rust/                    # cxx-qt 逻辑层
│   ├── Cargo.toml           # crate-type = staticlib
│   ├── build.rs
│   └── src/layout_engine.rs # QObject + #[qenum] LayoutMode
└── examples/minimal/        # 演示如何消费本库
```

分工：

| 层 | 职责 |
|---|---|
| Rust `LayoutEngine` | 布局计算、`LayoutMode` 枚举（`#[qenum]` → `Q_ENUM`） |
| C++ `TaffyItem` | `QQuickItem` 子类，场景图 `updatePaintNode`、属性、信号 |
| CMake | 用 Corrosion 把 Rust 编成 staticlib，再链进 `taffy::taffy` |

## 构建（本仓库）

依赖：CMake ≥ 3.24、Qt 6、Rust 工具链（cargo）。**不支持 Qt 5。**

```bash
cmake -S . -B build
cmake --build build
./build/examples/minimal/taffy_example_minimal
```

## 作为依赖引入（FetchContent）

```cmake
cmake_minimum_required(VERSION 3.24)
project(my_app LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

include(FetchContent)
FetchContent_Declare(taffy
    GIT_REPOSITORY https://github.com/<you>/taffy.git
    GIT_TAG main
)
FetchContent_MakeAvailable(taffy)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE
    taffy::taffy
    Qt6::Quick
)
```

```cpp
#include <taffy/Taffy.h>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    taffy::registerQmlTypes();  // 注册 Taffy.TaffyItem 等
    // ... load QML
}
```

```qml
import Taffy 1.0

TaffyItem {
    width: 200; height: 80
    mode: LayoutEngine.Flex
    spacing: 8
    color: "#3b82f6"
}
```

> 消费方机器上需要安装 Rust 工具链（Corrosion 会调用 cargo 编译 `rust/`）。

## 扩展：Rust 侧 `QQuickItem` 子类

若要在 Rust 里直接写 item（而不是 C++ 壳），用 `#[base = QQuickItem]` + `#[cxx_override]`：

```rust
unsafe extern "C++Qt" {
    include!(<QtQuick/QQuickItem>);
    #[qobject]
    type QQuickItem;
}

extern "RustQt" {
    #[qobject]
    #[base = QQuickItem]
    #[qml_element]
    type RustItem = super::RustItemRust;
}

extern "RustQt" {
    #[qinvokable]
    #[cxx_override]
    fn update_paint_node(self: Pin<&mut RustItem>, old: *mut QSGNode) -> *mut QSGNode;
}
```

`QSG*` 场景图 API 需要自行用 `extern "C++"` 绑定，成本较高；业务布局逻辑放在 `LayoutEngine` 更合适。

## 许可

MIT OR Apache-2.0
