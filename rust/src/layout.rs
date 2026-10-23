// Layout node Rust state. The QObject (`TaffyLayout`) lives in `utils`'s
// cxx-qt bridge so it can use the style enums defined there.

use crate::attached::TaffyAttachedRust;
pub use super::utils::ffi::*;

#[derive(Default)]
pub struct TaffyLayoutRust {
    pub display: Display,
    pub flex_direction: FlexDirection,
    pub flex_wrap: FlexWrap,
    pub flex_line_count: u16,

    pub justify_content: AlignContentKeyword,
    pub align_items: AlignItemsKeyword,
    pub align_content: AlignContentKeyword,

    pub dirty: bool
}


