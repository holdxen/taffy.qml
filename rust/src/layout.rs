use taffy::{AlignContent, AlignItems, Display, FlexDirection, FlexWrap, JustifyContent, Size};


#[derive(Default)]
pub struct TaffyLayoutRust {
    display: Display,
    flex_direction: FlexDirection,
    flex_wrap: FlexWrap,
    flex_line_count: u16,

    justify_content: JustifyContent,
    align_items: AlignItems,
    align_content: AlignContent,
}

// impl Default for TaffyLayoutRust {
//     fn default() -> Self {
//         Self {
//             display: taffy::Display::DEFAULT,
//             flex_direction: FlexDirection::default(),
//
//         }
//     }
// }

#[cxx_qt::bridge]
pub mod ffi {
    unsafe extern "C++" {
        include!(<QtQuick/QQuickItem>);
        type QQuickItem;
    }

    extern "RustQt" {
        #[qobject]
        #[base = QQuickItem]
        #[qml_element]
        type TaffyLayout = super::TaffyLayoutRust;
    }

    impl cxx_qt::Initialize for TaffyLayout {}
}

impl cxx_qt::Initialize for ffi::TaffyLayout {
    fn initialize(self: core::pin::Pin<&mut Self>) {
        // 初始化 QQuickItem
    }
}