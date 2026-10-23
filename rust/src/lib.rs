use taffy::geometry::{Line, MinMax, Point as TPoint, Rect as TRect, Size as TSize};
use taffy::prelude::{
    AlignContent, AlignItems, AlignItemsKeyword as TAlignItems, AvailableSpace as TAvailableSpace,
    BoxSizing, Dimension, Display, FlexDirection, FlexWrap, LengthPercentage, LengthPercentageAuto,
    NodeId, Position, Style,
};
use taffy::style::{
    AlignContentKeyword as TAlignContent, AlignmentSafety, Clear, Direction, Float, GridAutoFlow,
    Overflow, TextAlign,
};
use taffy::prelude::{TaffyFitContent, TaffyGridLine, TaffyGridSpan, TaffyMaxContent, TaffyMinContent, TaffyZero};
use taffy::style_helpers::{length, percent};
use taffy::tree::TaffyError;
use taffy::{GridPlacement, MaxTrackSizingFunction, MinTrackSizingFunction, TrackSizingFunction};
use taffy::TaffyTree;

#[cxx_qt::bridge]
pub mod ffi {
    extern "RustQt" {
        #[qobject]
        type TaffyTypes = super::TaffyTypesRust;
    }

    #[qenum(TaffyTypes)]
    enum Display {
        Block,
        FlowRoot,
        Flex,
        Grid,
        None,
    }

    #[qenum(TaffyTypes)]
    enum Position {
        Static,
        Relative,
        Absolute,
        Fixed,
        Sticky,
    }

    #[qenum(TaffyTypes)]
    enum FlexDirection {
        Row,
        Column,
        RowReverse,
        ColumnReverse,
    }

    #[qenum(TaffyTypes)]
    enum FlexWrap {
        NoWrap,
        Wrap,
        WrapReverse,
        Balance,
        BalanceReverse,
    }

    #[qenum(TaffyTypes)]
    enum AlignItems {
        Normal,
        Start,
        End,
        FlexStart,
        FlexEnd,
        SelfStart,
        SelfEnd,
        Center,
        Baseline,
        Stretch,
    }

    #[qenum(TaffyTypes)]
    enum AlignContent {
        Normal,
        Start,
        End,
        FlexStart,
        FlexEnd,
        Center,
        Stretch,
        SpaceBetween,
        SpaceEvenly,
        SpaceAround,
    }

    #[qenum(TaffyTypes)]
    enum Overflow {
        Visible,
        Clip,
        Hidden,
        Scroll,
    }

    #[qenum(TaffyTypes)]
    enum BoxSizing {
        BorderBox,
        ContentBox,
    }

    #[qenum(TaffyTypes)]
    enum StyleDirection {
        Ltr,
        Rtl,
    }

    #[qenum(TaffyTypes)]
    enum AvailableSpaceKind {
        Definite,
        MinContent,
        MaxContent,
    }

    #[qenum(TaffyTypes)]
    enum TextAlign {
        Auto,
        LegacyLeft,
        LegacyRight,
        LegacyCenter,
    }

    #[qenum(TaffyTypes)]
    enum CssFloat {
        None,
        Left,
        Right,
    }

    #[qenum(TaffyTypes)]
    enum CssClear {
        None,
        Left,
        Right,
        Both,
    }

    #[qenum(TaffyTypes)]
    enum GridAutoFlow {
        Row,
        Column,
        RowDense,
        ColumnDense,
    }

    #[qenum(TaffyTypes)]
    enum LengthKind {
        Auto,
        Length,
        Percent,
        MinContent,
        MaxContent,
        FitContent,
        Stretch,
        Content,
        Zero,
    }

    #[qenum(TaffyTypes)]
    enum PlacementKind {
        Auto,
        Line,
        Span,
    }

    #[qenum(TaffyTypes)]
    enum TrackMinKind {
        Auto,
        Length,
        Percent,
        MinContent,
        MaxContent,
        FitContent,
    }

    #[qenum(TaffyTypes)]
    enum TrackMaxKind {
        Auto,
        Length,
        Percent,
        MinContent,
        MaxContent,
        FitContent,
        Fr,
    }

    struct Point {
        x: f64,
        y: f64,
    }

    struct Size {
        width: f64,
        height: f64,
    }

    struct Rect {
        left: f64,
        right: f64,
        top: f64,
        bottom: f64,
    }

    struct AvailableSpace {
        kind: i32,
        value: f64,
    }

    struct LengthValue {
        kind: i32,
        value: f64,
    }

    struct LengthSize {
        width: LengthValue,
        height: LengthValue,
    }

    struct LengthRect {
        left: LengthValue,
        right: LengthValue,
        top: LengthValue,
        bottom: LengthValue,
    }

    struct Placement {
        kind: i32,
        value: f64,
    }

    struct LinePlacement {
        start: Placement,
        end: Placement,
    }

    struct TrackSize {
        min_kind: i32,
        min_value: f64,
        max_kind: i32,
        max_value: f64,
    }

    struct Layout {
        order: u32,
        location: Point,
        size: Size,
        border: Rect,
        padding: Rect,
        margin: Rect,
        scrollbar_size: Size,
    }

    struct NodeStyle {
        display: i32,
        item_is_table: bool,
        item_is_replaced: bool,
        item_is_compressible_replaced: bool,
        box_sizing: i32,
        direction: i32,
        overflow_x: i32,
        overflow_y: i32,
        scrollbar_width: f64,
        contain: u32,
        css_float: i32,
        clear: i32,
        position: i32,
        inset: LengthRect,
        size: LengthSize,
        min_size: LengthSize,
        max_size: LengthSize,
        aspect_ratio: f64,
        margin: LengthRect,
        padding: LengthRect,
        border: LengthRect,
        align_items: i32,
        has_align_self: bool,
        align_self: i32,
        justify_items: i32,
        has_justify_self: bool,
        justify_self: i32,
        align_content: i32,
        justify_content: i32,
        gap: LengthSize,
        text_align: i32,
        flex_direction: i32,
        flex_wrap: i32,
        flex_line_count: u32,
        flex_basis: LengthValue,
        flex_grow: f64,
        flex_shrink: f64,
        grid_auto_flow: i32,
        grid_row: LinePlacement,
        grid_column: LinePlacement,
    }

    extern "Rust" {
        type TaffyEngine;

        fn engine_new() -> Box<TaffyEngine>;
        fn engine_add_node(self: &mut TaffyEngine, style: &NodeStyle) -> u32;
        fn engine_add_child(self: &mut TaffyEngine, parent: u32, child: u32) -> Result<()>;
        fn engine_set_style(self: &mut TaffyEngine, node: u32, style: &NodeStyle) -> Result<()>;
        fn engine_compute(
            self: &mut TaffyEngine,
            width: &AvailableSpace,
            height: &AvailableSpace,
        ) -> Result<()>;
        fn engine_layout(self: &TaffyEngine, node: u32) -> Result<Layout>;
        fn engine_clear(self: &mut TaffyEngine);
    }
}

#[derive(Default)]
pub struct TaffyTypesRust;

fn map_display(v: ffi::Display) -> Display {
    match v {
        ffi::Display::Block => Display::Block,
        ffi::Display::FlowRoot => Display::FlowRoot,
        ffi::Display::Flex => Display::Flex,
        ffi::Display::Grid => Display::Grid,
        ffi::Display::None => Display::None,
        _ => Display::Flex,
    }
}

fn map_position(v: ffi::Position) -> Position {
    match v {
        ffi::Position::Static => Position::Static,
        ffi::Position::Relative => Position::Relative,
        ffi::Position::Absolute => Position::Absolute,
        ffi::Position::Fixed => Position::Fixed,
        ffi::Position::Sticky => Position::Sticky,
        _ => Position::Static,
    }
}

fn map_flex_direction(v: ffi::FlexDirection) -> FlexDirection {
    match v {
        ffi::FlexDirection::Row => FlexDirection::Row,
        ffi::FlexDirection::Column => FlexDirection::Column,
        ffi::FlexDirection::RowReverse => FlexDirection::RowReverse,
        ffi::FlexDirection::ColumnReverse => FlexDirection::ColumnReverse,
        _ => FlexDirection::Row,
    }
}

fn map_flex_wrap(v: ffi::FlexWrap) -> FlexWrap {
    match v {
        ffi::FlexWrap::NoWrap => FlexWrap::NoWrap,
        ffi::FlexWrap::Wrap => FlexWrap::Wrap,
        ffi::FlexWrap::WrapReverse => FlexWrap::WrapReverse,
        ffi::FlexWrap::Balance => FlexWrap::Balance,
        ffi::FlexWrap::BalanceReverse => FlexWrap::BalanceReverse,
        _ => FlexWrap::NoWrap,
    }
}

fn map_align_items(v: ffi::AlignItems) -> AlignItems {
    let keyword = match v {
        ffi::AlignItems::Normal => TAlignItems::Normal,
        ffi::AlignItems::Start => TAlignItems::Start,
        ffi::AlignItems::End => TAlignItems::End,
        ffi::AlignItems::FlexStart => TAlignItems::FlexStart,
        ffi::AlignItems::FlexEnd => TAlignItems::FlexEnd,
        ffi::AlignItems::SelfStart => TAlignItems::SelfStart,
        ffi::AlignItems::SelfEnd => TAlignItems::SelfEnd,
        ffi::AlignItems::Center => TAlignItems::Center,
        ffi::AlignItems::Baseline => TAlignItems::Baseline,
        ffi::AlignItems::Stretch => TAlignItems::Stretch,
        _ => TAlignItems::Normal,
    };
    AlignItems {
        keyword,
        safety: AlignmentSafety::Default,
    }
}

fn map_align_content(v: ffi::AlignContent) -> AlignContent {
    let keyword = match v {
        ffi::AlignContent::Normal => TAlignContent::Normal,
        ffi::AlignContent::Start => TAlignContent::Start,
        ffi::AlignContent::End => TAlignContent::End,
        ffi::AlignContent::FlexStart => TAlignContent::FlexStart,
        ffi::AlignContent::FlexEnd => TAlignContent::FlexEnd,
        ffi::AlignContent::Center => TAlignContent::Center,
        ffi::AlignContent::Stretch => TAlignContent::Stretch,
        ffi::AlignContent::SpaceBetween => TAlignContent::SpaceBetween,
        ffi::AlignContent::SpaceEvenly => TAlignContent::SpaceEvenly,
        ffi::AlignContent::SpaceAround => TAlignContent::SpaceAround,
        _ => TAlignContent::Normal,
    };
    AlignContent {
        keyword,
        safety: AlignmentSafety::Default,
    }
}

fn map_overflow(v: ffi::Overflow) -> Overflow {
    match v {
        ffi::Overflow::Visible => Overflow::Visible,
        ffi::Overflow::Clip => Overflow::Clip,
        ffi::Overflow::Hidden => Overflow::Hidden,
        ffi::Overflow::Scroll => Overflow::Scroll,
        _ => Overflow::Visible,
    }
}

fn map_box_sizing(v: ffi::BoxSizing) -> BoxSizing {
    match v {
        ffi::BoxSizing::BorderBox => BoxSizing::BorderBox,
        ffi::BoxSizing::ContentBox => BoxSizing::ContentBox,
        _ => BoxSizing::BorderBox,
    }
}

fn map_direction(v: ffi::StyleDirection) -> Direction {
    match v {
        ffi::StyleDirection::Ltr => Direction::Ltr,
        ffi::StyleDirection::Rtl => Direction::Rtl,
        _ => Direction::Ltr,
    }
}

fn map_text_align(v: ffi::TextAlign) -> TextAlign {
    match v {
        ffi::TextAlign::Auto => TextAlign::Auto,
        ffi::TextAlign::LegacyLeft => TextAlign::LegacyLeft,
        ffi::TextAlign::LegacyRight => TextAlign::LegacyRight,
        ffi::TextAlign::LegacyCenter => TextAlign::LegacyCenter,
        _ => TextAlign::Auto,
    }
}

fn map_float(v: ffi::CssFloat) -> Float {
    match v {
        ffi::CssFloat::Left => Float::Left,
        ffi::CssFloat::Right => Float::Right,
        _ => Float::None,
    }
}

fn map_clear(v: ffi::CssClear) -> Clear {
    match v {
        ffi::CssClear::Left => Clear::Left,
        ffi::CssClear::Right => Clear::Right,
        ffi::CssClear::Both => Clear::Both,
        _ => Clear::None,
    }
}

fn map_grid_auto_flow(v: ffi::GridAutoFlow) -> GridAutoFlow {
    match v {
        ffi::GridAutoFlow::Row => GridAutoFlow::Row,
        ffi::GridAutoFlow::Column => GridAutoFlow::Column,
        ffi::GridAutoFlow::RowDense => GridAutoFlow::RowDense,
        ffi::GridAutoFlow::ColumnDense => GridAutoFlow::ColumnDense,
        _ => GridAutoFlow::Row,
    }
}

fn map_available_space(v: &ffi::AvailableSpace) -> TAvailableSpace {
    match v.kind {
        0 => TAvailableSpace::Definite(v.value as f32),
        1 => TAvailableSpace::MinContent,
        2 => TAvailableSpace::MaxContent,
        _ => TAvailableSpace::Definite(v.value as f32),
    }
}

fn map_dim(v: &ffi::LengthValue) -> Dimension {
    match v.kind {
        0 => Dimension::auto(),
        1 => Dimension::length(v.value as f32),
        2 => Dimension::percent((v.value / 100.0) as f32),
        3 => Dimension::min_content(),
        4 => Dimension::max_content(),
        5 => Dimension::fit_content(),
        6 => Dimension::stretch(),
        7 => Dimension::content(),
        8 => Dimension::length(0.0),
        _ => Dimension::auto(),
    }
}

fn map_lp(v: &ffi::LengthValue) -> LengthPercentage {
    match v.kind {
        2 => percent((v.value / 100.0) as f32),
        8 => length(0.0),
        _ => length(v.value as f32),
    }
}

fn map_lpa(v: &ffi::LengthValue) -> LengthPercentageAuto {
    match v.kind {
        0 => LengthPercentageAuto::auto(),
        2 => percent((v.value / 100.0) as f32),
        8 => length(0.0),
        _ => length(v.value as f32),
    }
}

fn map_length_size_dim(s: &ffi::LengthSize) -> TSize<Dimension> {
    TSize {
        width: map_dim(&s.width),
        height: map_dim(&s.height),
    }
}

fn map_length_size_lpa(s: &ffi::LengthSize) -> TSize<LengthPercentageAuto> {
    TSize {
        width: map_lpa(&s.width),
        height: map_lpa(&s.height),
    }
}

fn map_length_size_lp(s: &ffi::LengthSize) -> TSize<LengthPercentage> {
    TSize {
        width: map_lp(&s.width),
        height: map_lp(&s.height),
    }
}

fn map_length_rect_lpa(r: &ffi::LengthRect) -> TRect<LengthPercentageAuto> {
    TRect {
        left: map_lpa(&r.left),
        right: map_lpa(&r.right),
        top: map_lpa(&r.top),
        bottom: map_lpa(&r.bottom),
    }
}

fn map_length_rect_lp(r: &ffi::LengthRect) -> TRect<LengthPercentage> {
    TRect {
        left: map_lp(&r.left),
        right: map_lp(&r.right),
        top: map_lp(&r.top),
        bottom: map_lp(&r.bottom),
    }
}

fn map_min_track(kind: i32, value: f64) -> MinTrackSizingFunction {
    match kind {
        1 => length(value as f32),
        2 => percent((value / 100.0) as f32),
        3 => MinTrackSizingFunction::min_content(),
        4 => MinTrackSizingFunction::max_content(),
        5 => MinTrackSizingFunction::min_content(),
        _ => MinTrackSizingFunction::auto(),
    }
}

fn map_max_track(kind: i32, value: f64) -> MaxTrackSizingFunction {
    match kind {
        1 => length(value as f32),
        2 => percent((value / 100.0) as f32),
        3 => MaxTrackSizingFunction::min_content(),
        4 => MaxTrackSizingFunction::max_content(),
        5 => MaxTrackSizingFunction::fit_content_px(value as f32),
        6 => MaxTrackSizingFunction::fr(value as f32),
        _ => MaxTrackSizingFunction::auto(),
    }
}

fn map_track(t: &ffi::TrackSize) -> TrackSizingFunction {
    MinMax {
        min: map_min_track(t.min_kind, t.min_value),
        max: map_max_track(t.max_kind, t.max_value),
    }
}

fn map_placement(p: &ffi::Placement) -> GridPlacement {
    match p.kind {
        1 => taffy::style_helpers::line(p.value as i16),
        2 => taffy::style_helpers::span(p.value as u16),
        _ => GridPlacement::Auto,
    }
}

fn map_line(p: &ffi::LinePlacement) -> Line<GridPlacement> {
    Line {
        start: map_placement(&p.start),
        end: map_placement(&p.end),
    }
}

fn display_from_i32(v: i32) -> ffi::Display {
    match v {
        0 => ffi::Display::Block,
        1 => ffi::Display::FlowRoot,
        2 => ffi::Display::Flex,
        3 => ffi::Display::Grid,
        4 => ffi::Display::None,
        _ => ffi::Display::Flex,
    }
}

fn position_from_i32(v: i32) -> ffi::Position {
    match v {
        0 => ffi::Position::Static,
        1 => ffi::Position::Relative,
        2 => ffi::Position::Absolute,
        3 => ffi::Position::Fixed,
        4 => ffi::Position::Sticky,
        _ => ffi::Position::Static,
    }
}

fn flex_direction_from_i32(v: i32) -> ffi::FlexDirection {
    match v {
        0 => ffi::FlexDirection::Row,
        1 => ffi::FlexDirection::Column,
        2 => ffi::FlexDirection::RowReverse,
        3 => ffi::FlexDirection::ColumnReverse,
        _ => ffi::FlexDirection::Row,
    }
}

fn flex_wrap_from_i32(v: i32) -> ffi::FlexWrap {
    match v {
        0 => ffi::FlexWrap::NoWrap,
        1 => ffi::FlexWrap::Wrap,
        2 => ffi::FlexWrap::WrapReverse,
        3 => ffi::FlexWrap::Balance,
        4 => ffi::FlexWrap::BalanceReverse,
        _ => ffi::FlexWrap::NoWrap,
    }
}

fn align_items_from_i32(v: i32) -> ffi::AlignItems {
    match v {
        0 => ffi::AlignItems::Normal,
        1 => ffi::AlignItems::Start,
        2 => ffi::AlignItems::End,
        3 => ffi::AlignItems::FlexStart,
        4 => ffi::AlignItems::FlexEnd,
        5 => ffi::AlignItems::SelfStart,
        6 => ffi::AlignItems::SelfEnd,
        7 => ffi::AlignItems::Center,
        8 => ffi::AlignItems::Baseline,
        9 => ffi::AlignItems::Stretch,
        _ => ffi::AlignItems::Normal,
    }
}

fn align_content_from_i32(v: i32) -> ffi::AlignContent {
    match v {
        0 => ffi::AlignContent::Normal,
        1 => ffi::AlignContent::Start,
        2 => ffi::AlignContent::End,
        3 => ffi::AlignContent::FlexStart,
        4 => ffi::AlignContent::FlexEnd,
        5 => ffi::AlignContent::Center,
        6 => ffi::AlignContent::Stretch,
        7 => ffi::AlignContent::SpaceBetween,
        8 => ffi::AlignContent::SpaceEvenly,
        9 => ffi::AlignContent::SpaceAround,
        _ => ffi::AlignContent::Normal,
    }
}

fn overflow_from_i32(v: i32) -> ffi::Overflow {
    match v {
        0 => ffi::Overflow::Visible,
        1 => ffi::Overflow::Clip,
        2 => ffi::Overflow::Hidden,
        3 => ffi::Overflow::Scroll,
        _ => ffi::Overflow::Visible,
    }
}

fn box_sizing_from_i32(v: i32) -> ffi::BoxSizing {
    match v {
        0 => ffi::BoxSizing::BorderBox,
        1 => ffi::BoxSizing::ContentBox,
        _ => ffi::BoxSizing::BorderBox,
    }
}

fn style_direction_from_i32(v: i32) -> ffi::StyleDirection {
    match v {
        0 => ffi::StyleDirection::Ltr,
        1 => ffi::StyleDirection::Rtl,
        _ => ffi::StyleDirection::Ltr,
    }
}

fn text_align_from_i32(v: i32) -> ffi::TextAlign {
    match v {
        0 => ffi::TextAlign::Auto,
        1 => ffi::TextAlign::LegacyLeft,
        2 => ffi::TextAlign::LegacyRight,
        3 => ffi::TextAlign::LegacyCenter,
        _ => ffi::TextAlign::Auto,
    }
}

fn css_float_from_i32(v: i32) -> ffi::CssFloat {
    match v {
        1 => ffi::CssFloat::Left,
        2 => ffi::CssFloat::Right,
        _ => ffi::CssFloat::None,
    }
}

fn css_clear_from_i32(v: i32) -> ffi::CssClear {
    match v {
        1 => ffi::CssClear::Left,
        2 => ffi::CssClear::Right,
        3 => ffi::CssClear::Both,
        _ => ffi::CssClear::None,
    }
}

fn grid_auto_flow_from_i32(v: i32) -> ffi::GridAutoFlow {
    match v {
        0 => ffi::GridAutoFlow::Row,
        1 => ffi::GridAutoFlow::Column,
        2 => ffi::GridAutoFlow::RowDense,
        3 => ffi::GridAutoFlow::ColumnDense,
        _ => ffi::GridAutoFlow::Row,
    }
}

fn to_taffy_style(s: &ffi::NodeStyle) -> Style {
    Style {
        display: map_display(display_from_i32(s.display)),
        item_is_table: s.item_is_table,
        item_is_replaced: s.item_is_replaced,
        item_is_compressible_replaced: s.item_is_compressible_replaced,
        box_sizing: map_box_sizing(box_sizing_from_i32(s.box_sizing)),
        direction: map_direction(style_direction_from_i32(s.direction)),
        overflow: TPoint {
            x: map_overflow(overflow_from_i32(s.overflow_x)),
            y: map_overflow(overflow_from_i32(s.overflow_y)),
        },
        scrollbar_width: s.scrollbar_width as f32,
        contain: {
            use taffy::style::Contain;
            let mut c = Contain::NONE;
            if s.contain & 1 != 0 {
                c = c.union(Contain::LAYOUT);
            }
            if s.contain & 2 != 0 {
                c = c.union(Contain::PAINT);
            }
            c
        },
        float: map_float(css_float_from_i32(s.css_float)),
        clear: map_clear(css_clear_from_i32(s.clear)),
        position: map_position(position_from_i32(s.position)),
        inset: map_length_rect_lpa(&s.inset),
        size: map_length_size_dim(&s.size),
        min_size: map_length_size_lpa(&s.min_size),
        max_size: map_length_size_lpa(&s.max_size),
        aspect_ratio: if s.aspect_ratio.is_nan() {
            None
        } else {
            Some(s.aspect_ratio as f32)
        },
        margin: map_length_rect_lpa(&s.margin),
        padding: map_length_rect_lp(&s.padding),
        border: map_length_rect_lp(&s.border),
        align_items: map_align_items(align_items_from_i32(s.align_items)),
        align_self: if s.has_align_self {
            Some(map_align_items(align_items_from_i32(s.align_self)))
        } else {
            None
        },
        justify_items: map_align_items(align_items_from_i32(s.justify_items)),
        justify_self: if s.has_justify_self {
            Some(map_align_items(align_items_from_i32(s.justify_self)))
        } else {
            None
        },
        align_content: map_align_content(align_content_from_i32(s.align_content)),
        justify_content: map_align_content(align_content_from_i32(s.justify_content)),
        gap: map_length_size_lp(&s.gap),
        text_align: map_text_align(text_align_from_i32(s.text_align)),
        flex_direction: map_flex_direction(flex_direction_from_i32(s.flex_direction)),
        flex_wrap: map_flex_wrap(flex_wrap_from_i32(s.flex_wrap)),
        flex_line_count: s.flex_line_count.max(1) as u16,
        flex_basis: map_dim(&s.flex_basis),
        flex_grow: s.flex_grow as f32,
        flex_shrink: s.flex_shrink as f32,
        grid_auto_flow: map_grid_auto_flow(grid_auto_flow_from_i32(s.grid_auto_flow)),
        grid_row: map_line(&s.grid_row),
        grid_column: map_line(&s.grid_column),
        ..Style::DEFAULT
    }
}

pub struct TaffyEngine {
    tree: TaffyTree,
    nodes: Vec<NodeId>,
}

fn engine_new() -> Box<TaffyEngine> {
    Box::new(TaffyEngine {
        tree: TaffyTree::new(),
        nodes: Vec::new(),
    })
}

impl TaffyEngine {
    fn node(&self, index: u32) -> Result<NodeId, String> {
        self.nodes
            .get(index as usize)
            .copied()
            .ok_or_else(|| format!("unknown node id {index}"))
    }

    fn engine_add_node(&mut self, style: &ffi::NodeStyle) -> u32 {
        let node = self.tree.new_leaf(to_taffy_style(style)).expect("new_leaf");
        self.nodes.push(node);
        (self.nodes.len() - 1) as u32
    }

    fn engine_add_child(&mut self, parent: u32, child: u32) -> Result<(), String> {
        let p = self.node(parent)?;
        let c = self.node(child)?;
        self.tree.add_child(p, c).map_err(taffy_err)
    }

    fn engine_set_style(&mut self, node: u32, style: &ffi::NodeStyle) -> Result<(), String> {
        let n = self.node(node)?;
        self.tree.set_style(n, to_taffy_style(style)).map_err(taffy_err)
    }

    fn engine_compute(
        &mut self,
        width: &ffi::AvailableSpace,
        height: &ffi::AvailableSpace,
    ) -> Result<(), String> {
        let Some(&root) = self.nodes.first() else {
            return Ok(());
        };
        let size = TSize {
            width: map_available_space(width),
            height: map_available_space(height),
        };
        self.tree.compute_layout(root, size).map_err(taffy_err)
    }

    fn engine_layout(&self, node: u32) -> Result<ffi::Layout, String> {
        let n = self.node(node)?;
        let l = self.tree.layout(n).map_err(taffy_err)?;
        Ok(ffi::Layout {
            order: l.order,
            location: ffi::Point {
                x: l.location.x as f64,
                y: l.location.y as f64,
            },
            size: ffi::Size {
                width: l.size.width as f64,
                height: l.size.height as f64,
            },
            border: ffi::Rect {
                left: l.border.left as f64,
                right: l.border.right as f64,
                top: l.border.top as f64,
                bottom: l.border.bottom as f64,
            },
            padding: ffi::Rect {
                left: l.padding.left as f64,
                right: l.padding.right as f64,
                top: l.padding.top as f64,
                bottom: l.padding.bottom as f64,
            },
            margin: ffi::Rect {
                left: l.margin.left as f64,
                right: l.margin.right as f64,
                top: l.margin.top as f64,
                bottom: l.margin.bottom as f64,
            },
            scrollbar_size: ffi::Size {
                width: l.scrollbar_size.width as f64,
                height: l.scrollbar_size.height as f64,
            },
        })
    }

    fn engine_clear(&mut self) {
        self.tree = TaffyTree::new();
        self.nodes.clear();
    }
}

fn taffy_err(e: TaffyError) -> String {
    e.to_string()
}
