use crate::utils::ffi::AlignItemsKeyword;

#[derive(Default)]
pub struct TaffyAttachedRust {
    pub flex_grow: f32,
    pub flex_shrink: f32,
    pub align_self: AlignItemsKeyword,
}
