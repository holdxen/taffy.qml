use cxx_qt_build::CxxQtBuilder;

fn main() {
    // No QmlModule: types are registered from C++ (avoid URI clash with TaffyItem).
    // qenum still generates Q_ENUM on the TaffyTypes QObject in the C++ header.
    CxxQtBuilder::new()
        .qt_module("Core")
        .qt_module("Gui")
        .qt_module("Qml")
        .qt_module("Quick")
        .file("src/lib.rs")
        .build();
}
