//! Export taffy layout types to C++ via cxx. No Qt dependency.

use taffy::geometry::{Point, Size};
use taffy::prelude::{
    AlignContent, AlignItems, AlignItemsKeyword as TAlignItems, AvailableSpace, BoxSizing,
    Dimension, Display, FlexDirection, FlexWrap, NodeId, Position, Style,
};
use taffy::style::{AlignContentKeyword as TAlignContent, AlignmentSafety, Direction, Overflow};
use taffy::tree::TaffyError;
use taffy::TaffyTree;

#[cxx::bridge]
pub mod ffi {
    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum Display {
        Block,
        FlowRoot,
        Flex,
        Grid,
        None,
    }

    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum Position {
        Static,
        Relative,
        Absolute,
        Fixed,
        Sticky,
    }

    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum FlexDirection {
        Row,
        Column,
        RowReverse,
        ColumnReverse,
    }

    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum FlexWrap {
        NoWrap,
        Wrap,
        WrapReverse,
    }

    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum AlignItemsKeyword {
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

    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum AlignContentKeyword {
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

    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum Overflow {
        Visible,
        Clip,
        Hidden,
        Scroll,
    }

    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum BoxSizing {
        BorderBox,
        ContentBox,
    }

    #[derive(Debug, Clone, Copy, PartialEq, Eq)]
    enum Direction {
        Ltr,
        Rtl,
    }

    #[derive(Debug, Clone, Copy)]
    struct LayoutRect {
        x: f64,
        y: f64,
        width: f64,
        height: f64,
    }

    #[derive(Debug, Clone, Copy)]
    struct NodeStyle {
        display: Display,
        position: Position,
        flex_direction: FlexDirection,
        flex_wrap: FlexWrap,
        align_items: AlignItemsKeyword,
        align_content: AlignContentKeyword,
        justify_content: AlignContentKeyword,
        overflow_x: Overflow,
        overflow_y: Overflow,
        box_sizing: BoxSizing,
        direction: Direction,
        width: f64,
        height: f64,
        flex_grow: f64,
        flex_shrink: f64,
    }

    extern "Rust" {
        type TaffyEngine;

        fn engine_new() -> Box<TaffyEngine>;
        fn engine_add_node(self: &mut TaffyEngine, style: &NodeStyle) -> u32;
        fn engine_add_child(self: &mut TaffyEngine, parent: u32, child: u32) -> Result<()>;
        fn engine_set_style(self: &mut TaffyEngine, node: u32, style: &NodeStyle) -> Result<()>;
        fn engine_compute(
            self: &mut TaffyEngine,
            available_width: f64,
            available_height: f64,
        ) -> Result<()>;
        fn engine_layout(self: &TaffyEngine, node: u32) -> Result<LayoutRect>;
        fn engine_clear(self: &mut TaffyEngine);
    }
}

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
        _ => FlexWrap::NoWrap,
    }
}

fn map_align_items(v: ffi::AlignItemsKeyword) -> AlignItems {
    let keyword = match v {
        ffi::AlignItemsKeyword::Normal => TAlignItems::Normal,
        ffi::AlignItemsKeyword::Start => TAlignItems::Start,
        ffi::AlignItemsKeyword::End => TAlignItems::End,
        ffi::AlignItemsKeyword::FlexStart => TAlignItems::FlexStart,
        ffi::AlignItemsKeyword::FlexEnd => TAlignItems::FlexEnd,
        ffi::AlignItemsKeyword::SelfStart => TAlignItems::SelfStart,
        ffi::AlignItemsKeyword::SelfEnd => TAlignItems::SelfEnd,
        ffi::AlignItemsKeyword::Center => TAlignItems::Center,
        ffi::AlignItemsKeyword::Baseline => TAlignItems::Baseline,
        ffi::AlignItemsKeyword::Stretch => TAlignItems::Stretch,
        _ => TAlignItems::Normal,
    };
    AlignItems {
        keyword,
        safety: AlignmentSafety::Default,
    }
}

fn map_align_content(v: ffi::AlignContentKeyword) -> AlignContent {
    let keyword = match v {
        ffi::AlignContentKeyword::Normal => TAlignContent::Normal,
        ffi::AlignContentKeyword::Start => TAlignContent::Start,
        ffi::AlignContentKeyword::End => TAlignContent::End,
        ffi::AlignContentKeyword::FlexStart => TAlignContent::FlexStart,
        ffi::AlignContentKeyword::FlexEnd => TAlignContent::FlexEnd,
        ffi::AlignContentKeyword::Center => TAlignContent::Center,
        ffi::AlignContentKeyword::Stretch => TAlignContent::Stretch,
        ffi::AlignContentKeyword::SpaceBetween => TAlignContent::SpaceBetween,
        ffi::AlignContentKeyword::SpaceEvenly => TAlignContent::SpaceEvenly,
        ffi::AlignContentKeyword::SpaceAround => TAlignContent::SpaceAround,
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

fn map_direction(v: ffi::Direction) -> Direction {
    match v {
        ffi::Direction::Ltr => Direction::Ltr,
        ffi::Direction::Rtl => Direction::Rtl,
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

fn to_taffy_style(s: &ffi::NodeStyle) -> Style {
    Style {
        display: map_display(s.display),
        position: map_position(s.position),
        flex_direction: map_flex_direction(s.flex_direction),
        flex_wrap: map_flex_wrap(s.flex_wrap),
        align_items: map_align_items(s.align_items),
        align_content: map_align_content(s.align_content),
        justify_content: map_align_content(s.justify_content),
        overflow: Point {
            x: map_overflow(s.overflow_x),
            y: map_overflow(s.overflow_y),
        },
        box_sizing: map_box_sizing(s.box_sizing),
        direction: map_direction(s.direction),
        size: Size {
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

    fn engine_compute(&mut self, available_width: f64, available_height: f64) -> Result<(), String> {
        let Some(&root) = self.nodes.first() else {
            return Ok(());
        };
        let size = Size {
            width: AvailableSpace::Definite(available_width as f32),
            height: AvailableSpace::Definite(available_height as f32),
        };
        self.tree.compute_layout(root, size).map_err(taffy_err)
    }

    fn engine_layout(&self, node: u32) -> Result<ffi::LayoutRect, String> {
        let n = self.node(node)?;
        let l = self.tree.layout(n).map_err(taffy_err)?;
        Ok(ffi::LayoutRect {
            x: l.location.x as f64,
            y: l.location.y as f64,
            width: l.size.width as f64,
            height: l.size.height as f64,
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
