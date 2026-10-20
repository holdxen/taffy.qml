// Style enums bridged from the taffy crate to Qt/QML via cxx-qt.
// Naming: enum name prefixes every variant (see FlexDirection::FlexDirectionRow).

// Re-export so the bridge can refer to `super::TaffyLayoutRust`.
pub use crate::layout::TaffyLayoutRust;

use crate::utils::ffi::{
    AlignContentKeyword,
    AlignItemsKeyword,
    AlignmentSafety,
    BoxGenerationMode,
    BoxSizing,
    Clear,
    Direction,
    Display,
    FlexDirection,
    FlexWrap,
    Float,
    FloatDirection,
    GridAutoFlow,
    Overflow,
    Position,
    TextAlign,
};

#[cxx_qt::bridge]
pub mod ffi {

    #[qenum(TaffyType)]
    pub enum Display {
        DisplayBlock,
        DisplayFlowRoot,
        DisplayFlex,
        DisplayGrid,
        DisplayNone,
    }

    #[qenum(TaffyType)]
    pub enum BoxGenerationMode {
        BoxGenerationModeNormal,
        BoxGenerationModeNone,
    }

    #[qenum(TaffyType)]
    pub enum Position {
        PositionStatic,
        PositionRelative,
        PositionAbsolute,
        PositionFixed,
        PositionSticky,
    }

    #[qenum(TaffyType)]
    pub enum BoxSizing {
        BoxSizingBorderBox,
        BoxSizingContentBox,
    }

    #[qenum(TaffyType)]
    pub enum Overflow {
        OverflowVisible,
        OverflowClip,
        OverflowHidden,
        OverflowScroll,
    }

    #[qenum(TaffyType)]
    pub enum Direction {
        DirectionLtr,
        DirectionRtl,
    }

    #[qenum(TaffyType)]
    pub enum FlexDirection {
        FlexDirectionRow,
        FlexDirectionColumn,
        FlexDirectionRowReverse,
        FlexDirectionColumnReverse,
    }

    #[qenum(TaffyType)]
    pub enum FlexWrap {
        FlexWrapNoWrap,
        FlexWrapWrap,
        FlexWrapWrapReverse,
        FlexWrapBalance,
        FlexWrapBalanceReverse,
    }

    #[qenum(TaffyType)]
    pub enum AlignItemsKeyword {
        AlignItemsKeywordNormal,
        AlignItemsKeywordStart,
        AlignItemsKeywordEnd,
        AlignItemsKeywordFlexStart,
        AlignItemsKeywordFlexEnd,
        AlignItemsKeywordSelfStart,
        AlignItemsKeywordSelfEnd,
        AlignItemsKeywordCenter,
        AlignItemsKeywordBaseline,
        AlignItemsKeywordStretch,
    }

    #[qenum(TaffyType)]
    pub enum AlignContentKeyword {
        AlignContentKeywordNormal,
        AlignContentKeywordStart,
        AlignContentKeywordEnd,
        AlignContentKeywordFlexStart,
        AlignContentKeywordFlexEnd,
        AlignContentKeywordCenter,
        AlignContentKeywordStretch,
        AlignContentKeywordSpaceBetween,
        AlignContentKeywordSpaceEvenly,
        AlignContentKeywordSpaceAround,
    }

    #[qenum(TaffyType)]
    pub enum AlignmentSafety {
        AlignmentSafetyDefault,
        AlignmentSafetyUnsafe,
        AlignmentSafetySafe,
    }

    #[qenum(TaffyType)]
    pub enum Float {
        FloatLeft,
        FloatRight,
        FloatNone,
    }

    #[qenum(TaffyType)]
    pub enum FloatDirection {
        FloatDirectionLeft,
        FloatDirectionRight,
    }

    #[qenum(TaffyType)]
    pub enum Clear {
        ClearLeft,
        ClearRight,
        ClearBoth,
        ClearNone,
    }

    #[qenum(TaffyType)]
    pub enum TextAlign {
        TextAlignAuto,
        TextAlignLegacyLeft,
        TextAlignLegacyRight,
        TextAlignLegacyCenter,
    }

    #[qenum(TaffyType)]
    pub enum GridAutoFlow {
        GridAutoFlowRow,
        GridAutoFlowColumn,
        GridAutoFlowRowDense,
        GridAutoFlowColumnDense,
    }

    unsafe extern "C++Qt" {
        include!(<QtQuick/QQuickItem>);
        /// Base for the layout item
        #[qobject]
        type QQuickItem;
    }

    extern "RustQt" {
        #[qobject]
        #[qml_element]
        type TaffyType = super::TaffyTypeRust;

        #[qobject]
        #[base = QQuickItem]
        #[qml_element]
        #[qproperty(Display, display)]
        #[qproperty(FlexDirection, flex_direction)]
        #[qproperty(FlexWrap, flex_wrap)]
        #[qproperty(AlignItemsKeyword, align_items)]
        #[qproperty(AlignContentKeyword, justify_content)]
        #[qproperty(AlignContentKeyword, align_content)]
        type TaffyLayout = super::TaffyLayoutRust;
    }

    impl cxx_qt::Initialize for TaffyLayout {}

}

impl From<Display> for taffy::Display {
    fn from(value: Display) -> Self {
        match value {
            Display::DisplayBlock => taffy::Display::Block,
            Display::DisplayFlowRoot => taffy::Display::FlowRoot,
            Display::DisplayFlex => taffy::Display::Flex,
            Display::DisplayGrid => taffy::Display::Grid,
            Display::DisplayNone => taffy::Display::None,
            _ => Default::default()
        }
    }
}

impl From<taffy::Display> for Display {
    fn from(value: taffy::Display) -> Self {
        match value {
            taffy::Display::Block => Self::DisplayBlock,
            taffy::Display::FlowRoot => Self::DisplayFlowRoot,
            taffy::Display::Flex => Self::DisplayFlex,
            taffy::Display::Grid => Self::DisplayGrid,
            taffy::Display::None => Self::DisplayNone
        }
    }
}

impl From<BoxGenerationMode> for taffy::BoxGenerationMode {
    fn from(value: BoxGenerationMode) -> Self {
        match value {
            BoxGenerationMode::BoxGenerationModeNormal => taffy::BoxGenerationMode::Normal,
            BoxGenerationMode::BoxGenerationModeNone => taffy::BoxGenerationMode::None,
            _ => Default::default()
        }
    }
}

impl From<taffy::BoxGenerationMode> for BoxGenerationMode {
    fn from(value: taffy::BoxGenerationMode) -> Self {
        match value {
            taffy::BoxGenerationMode::Normal => Self::BoxGenerationModeNormal,
            taffy::BoxGenerationMode::None => Self::BoxGenerationModeNone
        }
    }
}

impl From<Position> for taffy::Position {
    fn from(value: Position) -> Self {
        match value {
            Position::PositionStatic => taffy::Position::Static,
            Position::PositionRelative => taffy::Position::Relative,
            Position::PositionAbsolute => taffy::Position::Absolute,
            Position::PositionFixed => taffy::Position::Fixed,
            Position::PositionSticky => taffy::Position::Sticky,
            _ => Default::default()
        }
    }
}

impl From<taffy::Position> for Position {
    fn from(value: taffy::Position) -> Self {
        match value {
            taffy::Position::Static => Self::PositionStatic,
            taffy::Position::Relative => Self::PositionRelative,
            taffy::Position::Absolute => Self::PositionAbsolute,
            taffy::Position::Fixed => Self::PositionFixed,
            taffy::Position::Sticky => Self::PositionSticky
        }
    }
}

impl From<BoxSizing> for taffy::BoxSizing {
    fn from(value: BoxSizing) -> Self {
        match value {
            BoxSizing::BoxSizingBorderBox => taffy::BoxSizing::BorderBox,
            BoxSizing::BoxSizingContentBox => taffy::BoxSizing::ContentBox,
            _ => Default::default()
        }
    }
}

impl From<taffy::BoxSizing> for BoxSizing {
    fn from(value: taffy::BoxSizing) -> Self {
        match value {
            taffy::BoxSizing::BorderBox => Self::BoxSizingBorderBox,
            taffy::BoxSizing::ContentBox => Self::BoxSizingContentBox
        }
    }
}

impl From<Overflow> for taffy::Overflow {
    fn from(value: Overflow) -> Self {
        match value {
            Overflow::OverflowVisible => taffy::Overflow::Visible,
            Overflow::OverflowClip => taffy::Overflow::Clip,
            Overflow::OverflowHidden => taffy::Overflow::Hidden,
            Overflow::OverflowScroll => taffy::Overflow::Scroll,
            _ => Default::default()
        }
    }
}

impl From<taffy::Overflow> for Overflow {
    fn from(value: taffy::Overflow) -> Self {
        match value {
            taffy::Overflow::Visible => Self::OverflowVisible,
            taffy::Overflow::Clip => Self::OverflowClip,
            taffy::Overflow::Hidden => Self::OverflowHidden,
            taffy::Overflow::Scroll => Self::OverflowScroll
        }
    }
}

impl From<Direction> for taffy::Direction {
    fn from(value: Direction) -> Self {
        match value {
            Direction::DirectionLtr => taffy::Direction::Ltr,
            Direction::DirectionRtl => taffy::Direction::Rtl,
            _ => Default::default()
        }
    }
}

impl From<taffy::Direction> for Direction {
    fn from(value: taffy::Direction) -> Self {
        match value {
            taffy::Direction::Ltr => Self::DirectionLtr,
            taffy::Direction::Rtl => Self::DirectionRtl
        }
    }
}

impl From<FlexDirection> for taffy::FlexDirection {
    fn from(value: FlexDirection) -> Self {
        match value {
            FlexDirection::FlexDirectionRow => taffy::FlexDirection::Row,
            FlexDirection::FlexDirectionColumn => taffy::FlexDirection::Column,
            FlexDirection::FlexDirectionRowReverse => taffy::FlexDirection::RowReverse,
            FlexDirection::FlexDirectionColumnReverse => taffy::FlexDirection::ColumnReverse,
            _ => Default::default()
        }
    }
}

impl From<taffy::FlexDirection> for FlexDirection {
    fn from(value: taffy::FlexDirection) -> Self {
        match value {
            taffy::FlexDirection::Row => Self::FlexDirectionRow,
            taffy::FlexDirection::Column => Self::FlexDirectionColumn,
            taffy::FlexDirection::RowReverse => Self::FlexDirectionRowReverse,
            taffy::FlexDirection::ColumnReverse => Self::FlexDirectionColumnReverse
        }
    }
}

impl From<FlexWrap> for taffy::FlexWrap {
    fn from(value: FlexWrap) -> Self {
        match value {
            FlexWrap::FlexWrapNoWrap => taffy::FlexWrap::NoWrap,
            FlexWrap::FlexWrapWrap => taffy::FlexWrap::Wrap,
            FlexWrap::FlexWrapWrapReverse => taffy::FlexWrap::WrapReverse,
            FlexWrap::FlexWrapBalance => taffy::FlexWrap::Balance,
            FlexWrap::FlexWrapBalanceReverse => taffy::FlexWrap::BalanceReverse,
            _ => Default::default()
        }
    }
}

impl From<taffy::FlexWrap> for FlexWrap {
    fn from(value: taffy::FlexWrap) -> Self {
        match value {
            taffy::FlexWrap::NoWrap => Self::FlexWrapNoWrap,
            taffy::FlexWrap::Wrap => Self::FlexWrapWrap,
            taffy::FlexWrap::WrapReverse => Self::FlexWrapWrapReverse,
            taffy::FlexWrap::Balance => Self::FlexWrapBalance,
            taffy::FlexWrap::BalanceReverse => Self::FlexWrapBalanceReverse
        }
    }
}

impl From<AlignItemsKeyword> for taffy::AlignItemsKeyword {
    fn from(value: AlignItemsKeyword) -> Self {
        match value {
            AlignItemsKeyword::AlignItemsKeywordNormal => taffy::AlignItemsKeyword::Normal,
            AlignItemsKeyword::AlignItemsKeywordStart => taffy::AlignItemsKeyword::Start,
            AlignItemsKeyword::AlignItemsKeywordEnd => taffy::AlignItemsKeyword::End,
            AlignItemsKeyword::AlignItemsKeywordFlexStart => taffy::AlignItemsKeyword::FlexStart,
            AlignItemsKeyword::AlignItemsKeywordFlexEnd => taffy::AlignItemsKeyword::FlexEnd,
            AlignItemsKeyword::AlignItemsKeywordSelfStart => taffy::AlignItemsKeyword::SelfStart,
            AlignItemsKeyword::AlignItemsKeywordSelfEnd => taffy::AlignItemsKeyword::SelfEnd,
            AlignItemsKeyword::AlignItemsKeywordCenter => taffy::AlignItemsKeyword::Center,
            AlignItemsKeyword::AlignItemsKeywordBaseline => taffy::AlignItemsKeyword::Baseline,
            AlignItemsKeyword::AlignItemsKeywordStretch => taffy::AlignItemsKeyword::Stretch,
            _ => Default::default()
        }
    }
}

impl From<taffy::AlignItemsKeyword> for AlignItemsKeyword {
    fn from(value: taffy::AlignItemsKeyword) -> Self {
        match value {
            taffy::AlignItemsKeyword::Normal => Self::AlignItemsKeywordNormal,
            taffy::AlignItemsKeyword::Start => Self::AlignItemsKeywordStart,
            taffy::AlignItemsKeyword::End => Self::AlignItemsKeywordEnd,
            taffy::AlignItemsKeyword::FlexStart => Self::AlignItemsKeywordFlexStart,
            taffy::AlignItemsKeyword::FlexEnd => Self::AlignItemsKeywordFlexEnd,
            taffy::AlignItemsKeyword::SelfStart => Self::AlignItemsKeywordSelfStart,
            taffy::AlignItemsKeyword::SelfEnd => Self::AlignItemsKeywordSelfEnd,
            taffy::AlignItemsKeyword::Center => Self::AlignItemsKeywordCenter,
            taffy::AlignItemsKeyword::Baseline => Self::AlignItemsKeywordBaseline,
            taffy::AlignItemsKeyword::Stretch => Self::AlignItemsKeywordStretch
        }
    }
}

impl From<AlignContentKeyword> for taffy::AlignContentKeyword {
    fn from(value: AlignContentKeyword) -> Self {
        match value {
            AlignContentKeyword::AlignContentKeywordNormal => taffy::AlignContentKeyword::Normal,
            AlignContentKeyword::AlignContentKeywordStart => taffy::AlignContentKeyword::Start,
            AlignContentKeyword::AlignContentKeywordEnd => taffy::AlignContentKeyword::End,
            AlignContentKeyword::AlignContentKeywordFlexStart => taffy::AlignContentKeyword::FlexStart,
            AlignContentKeyword::AlignContentKeywordFlexEnd => taffy::AlignContentKeyword::FlexEnd,
            AlignContentKeyword::AlignContentKeywordCenter => taffy::AlignContentKeyword::Center,
            AlignContentKeyword::AlignContentKeywordStretch => taffy::AlignContentKeyword::Stretch,
            AlignContentKeyword::AlignContentKeywordSpaceBetween => taffy::AlignContentKeyword::SpaceBetween,
            AlignContentKeyword::AlignContentKeywordSpaceEvenly => taffy::AlignContentKeyword::SpaceEvenly,
            AlignContentKeyword::AlignContentKeywordSpaceAround => taffy::AlignContentKeyword::SpaceAround,
            _ => Default::default()
        }
    }
}

impl From<taffy::AlignContentKeyword> for AlignContentKeyword {
    fn from(value: taffy::AlignContentKeyword) -> Self {
        match value {
            taffy::AlignContentKeyword::Normal => Self::AlignContentKeywordNormal,
            taffy::AlignContentKeyword::Start => Self::AlignContentKeywordStart,
            taffy::AlignContentKeyword::End => Self::AlignContentKeywordEnd,
            taffy::AlignContentKeyword::FlexStart => Self::AlignContentKeywordFlexStart,
            taffy::AlignContentKeyword::FlexEnd => Self::AlignContentKeywordFlexEnd,
            taffy::AlignContentKeyword::Center => Self::AlignContentKeywordCenter,
            taffy::AlignContentKeyword::Stretch => Self::AlignContentKeywordStretch,
            taffy::AlignContentKeyword::SpaceBetween => Self::AlignContentKeywordSpaceBetween,
            taffy::AlignContentKeyword::SpaceEvenly => Self::AlignContentKeywordSpaceEvenly,
            taffy::AlignContentKeyword::SpaceAround => Self::AlignContentKeywordSpaceAround
        }
    }
}

impl From<AlignmentSafety> for taffy::AlignmentSafety {
    fn from(value: AlignmentSafety) -> Self {
        match value {
            AlignmentSafety::AlignmentSafetyDefault => taffy::AlignmentSafety::Default,
            AlignmentSafety::AlignmentSafetyUnsafe => taffy::AlignmentSafety::Unsafe,
            AlignmentSafety::AlignmentSafetySafe => taffy::AlignmentSafety::Safe,
            _ => Default::default()
        }
    }
}

impl From<taffy::AlignmentSafety> for AlignmentSafety {
    fn from(value: taffy::AlignmentSafety) -> Self {
        match value {
            taffy::AlignmentSafety::Default => Self::AlignmentSafetyDefault,
            taffy::AlignmentSafety::Unsafe => Self::AlignmentSafetyUnsafe,
            taffy::AlignmentSafety::Safe => Self::AlignmentSafetySafe
        }
    }
}

impl From<Float> for taffy::Float {
    fn from(value: Float) -> Self {
        match value {
            Float::FloatLeft => taffy::Float::Left,
            Float::FloatRight => taffy::Float::Right,
            Float::FloatNone => taffy::Float::None,
            _ => Default::default()
        }
    }
}

impl From<taffy::Float> for Float {
    fn from(value: taffy::Float) -> Self {
        match value {
            taffy::Float::Left => Self::FloatLeft,
            taffy::Float::Right => Self::FloatRight,
            taffy::Float::None => Self::FloatNone
        }
    }
}

impl From<FloatDirection> for taffy::FloatDirection {
    fn from(value: FloatDirection) -> Self {
        match value {
            FloatDirection::FloatDirectionLeft => taffy::FloatDirection::Left,
            FloatDirection::FloatDirectionRight => taffy::FloatDirection::Right,
            _ => taffy::FloatDirection::Left
        }
    }
}

impl From<taffy::FloatDirection> for FloatDirection {
    fn from(value: taffy::FloatDirection) -> Self {
        match value {
            taffy::FloatDirection::Left => Self::FloatDirectionLeft,
            taffy::FloatDirection::Right => Self::FloatDirectionRight
        }
    }
}

impl From<Clear> for taffy::Clear {
    fn from(value: Clear) -> Self {
        match value {
            Clear::ClearLeft => taffy::Clear::Left,
            Clear::ClearRight => taffy::Clear::Right,
            Clear::ClearBoth => taffy::Clear::Both,
            Clear::ClearNone => taffy::Clear::None,
            _ => Default::default()
        }
    }
}

impl From<taffy::Clear> for Clear {
    fn from(value: taffy::Clear) -> Self {
        match value {
            taffy::Clear::Left => Self::ClearLeft,
            taffy::Clear::Right => Self::ClearRight,
            taffy::Clear::Both => Self::ClearBoth,
            taffy::Clear::None => Self::ClearNone
        }
    }
}

impl From<TextAlign> for taffy::TextAlign {
    fn from(value: TextAlign) -> Self {
        match value {
            TextAlign::TextAlignAuto => taffy::TextAlign::Auto,
            TextAlign::TextAlignLegacyLeft => taffy::TextAlign::LegacyLeft,
            TextAlign::TextAlignLegacyRight => taffy::TextAlign::LegacyRight,
            TextAlign::TextAlignLegacyCenter => taffy::TextAlign::LegacyCenter,
            _ => Default::default()
        }
    }
}

impl From<taffy::TextAlign> for TextAlign {
    fn from(value: taffy::TextAlign) -> Self {
        match value {
            taffy::TextAlign::Auto => Self::TextAlignAuto,
            taffy::TextAlign::LegacyLeft => Self::TextAlignLegacyLeft,
            taffy::TextAlign::LegacyRight => Self::TextAlignLegacyRight,
            taffy::TextAlign::LegacyCenter => Self::TextAlignLegacyCenter
        }
    }
}

impl From<GridAutoFlow> for taffy::GridAutoFlow {
    fn from(value: GridAutoFlow) -> Self {
        match value {
            GridAutoFlow::GridAutoFlowRow => taffy::GridAutoFlow::Row,
            GridAutoFlow::GridAutoFlowColumn => taffy::GridAutoFlow::Column,
            GridAutoFlow::GridAutoFlowRowDense => taffy::GridAutoFlow::RowDense,
            GridAutoFlow::GridAutoFlowColumnDense => taffy::GridAutoFlow::ColumnDense,
            _ => Default::default()
        }
    }
}

impl From<taffy::GridAutoFlow> for GridAutoFlow {
    fn from(value: taffy::GridAutoFlow) -> Self {
        match value {
            taffy::GridAutoFlow::Row => Self::GridAutoFlowRow,
            taffy::GridAutoFlow::Column => Self::GridAutoFlowColumn,
            taffy::GridAutoFlow::RowDense => Self::GridAutoFlowRowDense,
            taffy::GridAutoFlow::ColumnDense => Self::GridAutoFlowColumnDense
        }
    }
}


// Defaults match taffy's #[default] variants.
impl Default for Display {
    fn default() -> Self {
        Self::DisplayFlex
    }
}

impl Default for BoxGenerationMode {
    fn default() -> Self {
        Self::BoxGenerationModeNormal
    }
}

impl Default for Position {
    fn default() -> Self {
        Self::PositionStatic
    }
}

impl Default for BoxSizing {
    fn default() -> Self {
        Self::BoxSizingBorderBox
    }
}

impl Default for Overflow {
    fn default() -> Self {
        Self::OverflowVisible
    }
}

impl Default for Direction {
    fn default() -> Self {
        Self::DirectionLtr
    }
}

impl Default for FlexDirection {
    fn default() -> Self {
        Self::FlexDirectionRow
    }
}

impl Default for FlexWrap {
    fn default() -> Self {
        Self::FlexWrapNoWrap
    }
}

impl Default for AlignItemsKeyword {
    fn default() -> Self {
        Self::AlignItemsKeywordNormal
    }
}

impl Default for AlignContentKeyword {
    fn default() -> Self {
        Self::AlignContentKeywordNormal
    }
}

impl Default for AlignmentSafety {
    fn default() -> Self {
        Self::AlignmentSafetyDefault
    }
}

impl Default for Float {
    fn default() -> Self {
        Self::FloatNone
    }
}

impl Default for FloatDirection {
    fn default() -> Self {
        Self::FloatDirectionLeft
    }
}

impl Default for Clear {
    fn default() -> Self {
        Self::ClearNone
    }
}

impl Default for TextAlign {
    fn default() -> Self {
        Self::TextAlignAuto
    }
}

impl Default for GridAutoFlow {
    fn default() -> Self {
        Self::GridAutoFlowRow
    }
}

impl cxx_qt::Initialize for ffi::TaffyLayout {
    fn initialize(self: core::pin::Pin<&mut Self>) {
        // QQuickItem base is initialized by Qt.
    }
}

#[derive(Default)]
pub struct TaffyTypeRust;
