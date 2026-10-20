//! Layout engine QObject + LayoutMode enum, exposed to C++ / QML via cxx-qt.

#[cxx_qt::bridge]
pub mod qobject {
    /// Layout algorithm used by TaffyItem.
    #[qenum(LayoutEngine)]
    enum LayoutMode {
        Flex,
        Grid,
        None,
    }

    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        type QString = cxx_qt_lib::QString;

        include!("cxx-qt-lib/qvector.h");
        type QVector_f32 = cxx_qt_lib::QVector<f32>;
    }

    extern "RustQt" {
        #[qobject]
        #[qproperty(LayoutMode, mode)]
        #[qproperty(f32, spacing)]
        type LayoutEngine = super::LayoutEngineRust;
    }

    extern "RustQt" {
        /// Compute a simple 1-D layout; returns child x positions.
        #[qinvokable]
        fn compute_row(self: &LayoutEngine, widths: &QVector_f32, available: f32) -> QVector_f32;

        #[qinvokable]
        fn mode_name(self: &LayoutEngine) -> QString;
    }
}

use cxx_qt_lib::{QString, QVector};
use qobject::LayoutMode;

pub struct LayoutEngineRust {
    mode: LayoutMode,
    spacing: f32,
}

impl Default for LayoutEngineRust {
    fn default() -> Self {
        Self {
            mode: LayoutMode::Flex,
            spacing: 0.0,
        }
    }
}

impl qobject::LayoutEngine {
    pub fn compute_row(&self, widths: &QVector<f32>, available: f32) -> QVector<f32> {
        let spacing = self.spacing();
        let mut x = 0.0f32;
        let mut positions = QVector::<f32>::default();

        for i in 0..widths.len() {
            let w = widths.get(i).copied().unwrap_or(0.0);
            if x + w > available && i > 0 {
                break;
            }
            positions.append(x);
            x += w + spacing;
        }

        positions
    }

    pub fn mode_name(&self) -> QString {
        // Shared CXX enums are associated constants, so compare instead of match.
        let mode = self.mode();
        let name = if *mode == LayoutMode::Flex {
            "Flex"
        } else if *mode == LayoutMode::Grid {
            "Grid"
        } else if *mode == LayoutMode::None {
            "None"
        } else {
            "Unknown"
        };
        QString::from(name)
    }
}
