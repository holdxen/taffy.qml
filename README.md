# taffy

Qt Quick 组件库骨架：**主逻辑在 C++（Qt）**，布局计算在 **Rust（taffy + 纯 cxx）**。Rust 不依赖 Qt。

## 分层

```text
taffy/
├── CMakeLists.txt           # C++/Qt + Corrosion(cxx)，可 FetchContent
├── include/taffy/
│   ├── Taffy.h              # registerQmlTypes()
│   ├── TaffyTypes.h         # Qt/QML 枚举（Q_ENUM）
│   ├── TaffyEngine.h        # C++ 门面，调用 Rust 布局引擎
│   └── TaffyLayout.h        # QQuickLayout
├── cpp/                     # 全部 Qt 逻辑
├── rust/                    # 纯 cxx 导出 taffy 类型（无 Qt）
│   └── src/lib.rs
└── examples/minimal/
```

| 层 | 职责 |
|---|---|
| C++ | QObject / QQuickItem / Q_ENUM / QML 注册 |
| Rust | `taffy::TaffyTree` 布局计算，经 `cxx` 导出 |
| CMake | Corrosion 编 Rust staticlib + `corrosion_add_cxxbridge` |

## 构建

依赖：CMake ≥ 3.24、Qt 6、Rust 工具链。不支持 Qt 5。

```bash
cmake -S . -B build
cmake --build build
./build/examples/minimal/taffy_example_minimal
```

## FetchContent

```cmake
FetchContent_Declare(taffy
    GIT_REPOSITORY <url>
    GIT_TAG main)
FetchContent_MakeAvailable(taffy)

target_link_libraries(app PRIVATE taffy::taffy Qt6::Quick)
```

```cpp
#include <taffy/Taffy.h>
taffy::registerQmlTypes();
```

## 关键约束

- Rust 侧 **不要** 依赖 Qt / cxx-qt。
- 枚举在 C++（`TaffyTypes`）与 cxx bridge（`rust/src/lib.rs`）中保持同序，便于 `static_cast`。
- 私有 Qt 模块只链到 C++ target；不要放进任何 Rust 模块列表。

## 许可

MIT OR Apache-2.0
