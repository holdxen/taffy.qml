use cxx_qt_build::CxxQtBuilder;

fn main() {
    // Types are registered to QML from C++ (taffy::registerQmlTypes)
    // so we do not declare a cxx-qt QML module here — that would conflict
    // with qmlRegisterType on the same URI.
    CxxQtBuilder::new()
        .qt_module("Core")
        .qt_module("Gui")
        .qt_module("Qml")
        .qt_module("Quick")
        .file("src/layout_engine.rs")
        .file("src/utils.rs")
        .build();
}
