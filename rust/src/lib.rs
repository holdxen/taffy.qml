//! Style enums are exported via cxx-qt `#[qenum]` (auto Q_ENUM for Qt/QML).
//! Layout engine stays plain `extern "Rust"` (no Qt logic).

use taffy::geometry::{Point as TPoint, Size as TSize};
use taffy::prelude::{
    AlignContent, AlignItems, AlignItemsKeyword as TAlignItems, AvailableSpace as TAvailableSpace,
    BoxSizing, Dimension, Display, FlexDirection, FlexWrap, NodeId, Position, Style,
};
use taffy::style::{AlignContentKeyword as TAlignContent, AlignmentSafety, Direction, Overflow};
use taffy::tree::TaffyError;
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

    #[derive(Debug, Clone, Copy)]
    struct Point {
        x: f64,
        y: f64,
    }

    #[derive(Debug, Clone, Copy)]
    struct Size {
        width: f64,
        height: f64,
    }

    #[derive(Debug, Clone, Copy)]
    struct Rect {
        left: f64,
        right: f64,
        top: f64,
        bottom: f64,
    }

    #[derive(Debug, Clone, Copy)]
    struct AvailableSpace {
        kind: i32,
        value: f64,
    }

    #[derive(Debug, Clone, Copy)]
    struct NodeStyle {
        display: i32,
        position: i32,
        flex_direction: i32,
        flex_wrap: i32,
        align_items: i32,
        align_content: i32,
        justify_content: i32,
        overflow_x: i32,
        overflow_y: i32,
        box_sizing: i32,
        direction: i32,
        width: f64,
        height: f64,
        flex_grow: f64,
        flex_shrink: f64,
    }

    #[derive(Debug, Clone, Copy)]
    struct Layout {
        order: u32,
        location: Point,
        size: Size,
        border: Rect,
        padding: Rect,
        margin: Rect,
        scrollbar_size: Size,
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

fn map_dim(v: f64) -> Dimension {
    if v.is_nan() {
        Dimension::auto()
    } else {
        Dimension::length(v as f32)
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

fn to_taffy_style(s: &ffi::NodeStyle) -> Style {
    Style {
        display: map_display(s.display),
        position: map_position(s.position),
        flex_direction: map_flex_direction(s.flex_direction),
        flex_wrap: map_flex_wrap(s.flex_wrap),
        align_items: map_align_items(s.align_items),
        align_content: map_align_content(s.align_content),
        justify_content: map_align_content(s.justify_content),
        overflow: TPoint {
            x: map_overflow(s.overflow_x),
            y: map_overflow(s.overflow_y),
        },
        box_sizing: map_box_sizing(s.box_sizing),
        direction: map_direction(s.direction),
        size: TSize {
            width: map_dim(s.width),
            height: map_dim(s.height),
        },
        flex_grow: s.flex_grow as f32,
        flex_shrink: s.flex_shrink as f32,
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
