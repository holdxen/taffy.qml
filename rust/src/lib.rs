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

fn map_display(v: i32) -> Display {
    match v {
        0 => Display::Block,
        1 => Display::FlowRoot,
        2 => Display::Flex,
        3 => Display::Grid,
        4 => Display::None,
        _ => Display::Flex,
    }
}

fn map_position(v: i32) -> Position {
    match v {
        0 => Position::Static,
        1 => Position::Relative,
        2 => Position::Absolute,
        3 => Position::Fixed,
        4 => Position::Sticky,
        _ => Position::Static,
    }
}

fn map_flex_direction(v: i32) -> FlexDirection {
    match v {
        0 => FlexDirection::Row,
        1 => FlexDirection::Column,
        2 => FlexDirection::RowReverse,
        3 => FlexDirection::ColumnReverse,
        _ => FlexDirection::Row,
    }
}

fn map_flex_wrap(v: i32) -> FlexWrap {
    match v {
        0 => FlexWrap::NoWrap,
        1 => FlexWrap::Wrap,
        2 => FlexWrap::WrapReverse,
        3 => FlexWrap::Balance,
        4 => FlexWrap::BalanceReverse,
        _ => FlexWrap::NoWrap,
    }
}

fn map_align_items(v: i32) -> AlignItems {
    let keyword = match v {
        0 => TAlignItems::Normal,
        1 => TAlignItems::Start,
        2 => TAlignItems::End,
        3 => TAlignItems::FlexStart,
        4 => TAlignItems::FlexEnd,
        5 => TAlignItems::SelfStart,
        6 => TAlignItems::SelfEnd,
        7 => TAlignItems::Center,
        8 => TAlignItems::Baseline,
        9 => TAlignItems::Stretch,
        _ => TAlignItems::Normal,
    };
    AlignItems {
        keyword,
        safety: AlignmentSafety::Default,
    }
}

fn map_align_content(v: i32) -> AlignContent {
    let keyword = match v {
        0 => TAlignContent::Normal,
        1 => TAlignContent::Start,
        2 => TAlignContent::End,
        3 => TAlignContent::FlexStart,
        4 => TAlignContent::FlexEnd,
        5 => TAlignContent::Center,
        6 => TAlignContent::Stretch,
        7 => TAlignContent::SpaceBetween,
        8 => TAlignContent::SpaceEvenly,
        9 => TAlignContent::SpaceAround,
        _ => TAlignContent::Normal,
    };
    AlignContent {
        keyword,
        safety: AlignmentSafety::Default,
    }
}

fn map_overflow(v: i32) -> Overflow {
    match v {
        0 => Overflow::Visible,
        1 => Overflow::Clip,
        2 => Overflow::Hidden,
        3 => Overflow::Scroll,
        _ => Overflow::Visible,
    }
}

fn map_box_sizing(v: i32) -> BoxSizing {
    match v {
        0 => BoxSizing::BorderBox,
        1 => BoxSizing::ContentBox,
        _ => BoxSizing::BorderBox,
    }
}

fn map_direction(v: i32) -> Direction {
    match v {
        0 => Direction::Ltr,
        1 => Direction::Rtl,
        _ => Direction::Ltr,
    }
}

fn map_text_align(v: i32) -> TextAlign {
    match v {
        0 => TextAlign::Auto,
        1 => TextAlign::LegacyLeft,
        2 => TextAlign::LegacyRight,
        3 => TextAlign::LegacyCenter,
        _ => TextAlign::Auto,
    }
}

fn map_float(v: i32) -> Float {
    match v {
        1 => Float::Left,
        2 => Float::Right,
        _ => Float::None,
    }
}

fn map_clear(v: i32) -> Clear {
    match v {
        1 => Clear::Left,
        2 => Clear::Right,
        3 => Clear::Both,
        _ => Clear::None,
    }
}

fn map_grid_auto_flow(v: i32) -> GridAutoFlow {
    match v {
        0 => GridAutoFlow::Row,
        1 => GridAutoFlow::Column,
        2 => GridAutoFlow::RowDense,
        3 => GridAutoFlow::ColumnDense,
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

fn to_taffy_style(s: &ffi::NodeStyle) -> Style {
    Style {
        display: map_display(s.display),
        item_is_table: s.item_is_table,
        item_is_replaced: s.item_is_replaced,
        item_is_compressible_replaced: s.item_is_compressible_replaced,
        box_sizing: map_box_sizing(s.box_sizing),
        direction: map_direction(s.direction),
        overflow: TPoint {
            x: map_overflow(s.overflow_x),
            y: map_overflow(s.overflow_y),
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
        float: map_float(s.css_float),
        clear: map_clear(s.clear),
        position: map_position(s.position),
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
        align_items: map_align_items(s.align_items),
        align_self: if s.has_align_self {
            Some(map_align_items(s.align_self))
        } else {
            None
        },
        justify_items: map_align_items(s.justify_items),
        justify_self: if s.has_justify_self {
            Some(map_align_items(s.justify_self))
        } else {
            None
        },
        align_content: map_align_content(s.align_content),
        justify_content: map_align_content(s.justify_content),
        gap: map_length_size_lp(&s.gap),
        text_align: map_text_align(s.text_align),
        flex_direction: map_flex_direction(s.flex_direction),
        flex_wrap: map_flex_wrap(s.flex_wrap),
        flex_line_count: s.flex_line_count.max(1) as u16,
        flex_basis: map_dim(&s.flex_basis),
        flex_grow: s.flex_grow as f32,
        flex_shrink: s.flex_shrink as f32,
        grid_auto_flow: map_grid_auto_flow(s.grid_auto_flow),
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
