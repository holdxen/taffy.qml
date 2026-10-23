//! Pure cxx bridge. Style values are CSS-like strings (parsed by taffy).

use std::str::FromStr;

use taffy::geometry::{Line, MinMax, Point as TPoint, Rect as TRect, Size as TSize};
use taffy::prelude::{
    AlignContent, AlignItems, AvailableSpace as TAvailableSpace, BoxSizing, Dimension, Display,
    FlexDirection, FlexWrap, LengthPercentage, LengthPercentageAuto, NodeId, Position, Style,
};
use taffy::style::{Clear, Direction, Float, GridAutoFlow, Overflow, TextAlign};
use taffy::style_helpers::{length, percent};
use taffy::tree::TaffyError;
use taffy::{GridPlacement, GridTemplateComponent, MaxTrackSizingFunction, MinTrackSizingFunction, TrackSizingFunction};
use taffy::TaffyTree;

#[cxx::bridge]
pub mod ffi {
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

    /// CSS-like length: "auto" | "100px" | "50%" | "min-content" | "max-content" | "0"
    struct LengthValue {
        css: String,
    }

    struct LengthSize {
        width: String,
        height: String,
    }

    struct LengthRect {
        left: String,
        right: String,
        top: String,
        bottom: String,
    }

    struct Placement {
        /// "auto" | "1" | "span 2"
        css: String,
    }

    struct LinePlacement {
        start: String,
        end: String,
    }

    /// min/max track size as CSS, e.g. "auto", "100px", "1fr", "min-content"
    struct TrackSize {
        min: String,
        max: String,
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

    /// Full style surface; keyword / dimension fields are CSS strings.
    struct NodeStyle {
        display: String,
        item_is_table: bool,
        item_is_replaced: bool,
        item_is_compressible_replaced: bool,
        box_sizing: String,
        direction: String,
        overflow_x: String,
        overflow_y: String,
        scrollbar_width: f64,
        contain: u32,
        css_float: String,
        clear: String,
        position: String,
        inset: LengthRect,
        size: LengthSize,
        min_size: LengthSize,
        max_size: LengthSize,
        aspect_ratio: f64,
        margin: LengthRect,
        padding: LengthRect,
        border: LengthRect,
        align_items: String,
        align_self: String,
        justify_items: String,
        justify_self: String,
        align_content: String,
        justify_content: String,
        gap: LengthSize,
        text_align: String,
        flex_direction: String,
        flex_wrap: String,
        flex_line_count: u32,
        flex_basis: String,
        flex_grow: f64,
        flex_shrink: f64,
        grid_auto_flow: String,
        grid_template_rows: Vec<TrackSize>,
        grid_template_columns: Vec<TrackSize>,
        grid_auto_rows: Vec<TrackSize>,
        grid_auto_columns: Vec<TrackSize>,
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

fn parse_or<T: FromStr>(s: &str, fallback: T) -> T {
    s.parse().unwrap_or(fallback)
}

fn parse_dim(s: &str) -> Dimension {
    if s.is_empty() {
        return Dimension::auto();
    }
    Dimension::from_str(s).unwrap_or_else(|_| Dimension::auto())
}

fn parse_lp(s: &str) -> LengthPercentage {
    // taffy FromStr for LengthPercentage may not exist; map manually.
    let s = s.trim();
    if let Some(p) = s.strip_suffix('%') {
        if let Ok(v) = p.trim().parse::<f32>() {
            return percent(v / 100.0);
        }
    }
    if let Some(v) = s.strip_suffix("px") {
        if let Ok(v) = v.trim().parse::<f32>() {
            return length(v);
        }
    }
    if let Ok(v) = s.parse::<f32>() {
        return length(v);
    }
    length(0.0)
}

fn parse_lpa(s: &str) -> LengthPercentageAuto {
    let s = s.trim();
    if s == "auto" || s.is_empty() {
        return LengthPercentageAuto::auto();
    }
    if let Some(p) = s.strip_suffix('%') {
        if let Ok(v) = p.trim().parse::<f32>() {
            return percent(v / 100.0);
        }
    }
    if let Some(v) = s.strip_suffix("px") {
        if let Ok(v) = v.trim().parse::<f32>() {
            return length(v);
        }
    }
    if let Ok(v) = s.parse::<f32>() {
        return length(v);
    }
    LengthPercentageAuto::auto()
}

fn parse_min_track(s: &str) -> MinTrackSizingFunction {
    let s = s.trim();
    match s {
        "auto" | "" => MinTrackSizingFunction::auto(),
        "min-content" => MinTrackSizingFunction::min_content(),
        "max-content" => MinTrackSizingFunction::max_content(),
        _ => {
            if let Some(p) = s.strip_suffix('%') {
                if let Ok(v) = p.trim().parse::<f32>() {
                    return percent(v / 100.0);
                }
            }
            if let Some(v) = s.strip_suffix("px") {
                if let Ok(v) = v.trim().parse::<f32>() {
                    return length(v);
                }
            }
            if let Ok(v) = s.parse::<f32>() {
                return length(v);
            }
            MinTrackSizingFunction::auto()
        }
    }
}

fn parse_max_track(s: &str) -> MaxTrackSizingFunction {
    let s = s.trim();
    match s {
        "auto" | "" => MaxTrackSizingFunction::auto(),
        "min-content" => MaxTrackSizingFunction::min_content(),
        "max-content" => MaxTrackSizingFunction::max_content(),
        _ => {
            if let Some(v) = s.strip_suffix("fr") {
                if let Ok(v) = v.trim().parse::<f32>() {
                    return MaxTrackSizingFunction::fr(v);
                }
            }
            if let Some(p) = s.strip_suffix('%') {
                if let Ok(v) = p.trim().parse::<f32>() {
                    return percent(v / 100.0);
                }
            }
            if let Some(v) = s.strip_suffix("px") {
                if let Ok(v) = v.trim().parse::<f32>() {
                    return length(v);
                }
            }
            if let Ok(v) = s.parse::<f32>() {
                return length(v);
            }
            MaxTrackSizingFunction::auto()
        }
    }
}

fn parse_placement(s: &str) -> GridPlacement {
    let s = s.trim();
    if s == "auto" || s.is_empty() {
        return GridPlacement::Auto;
    }
    if let Some(rest) = s.strip_prefix("span") {
        if let Ok(n) = rest.trim().parse::<u16>() {
            return taffy::style_helpers::span(n);
        }
    }
    if let Ok(n) = s.parse::<i16>() {
        return taffy::style_helpers::line(n);
    }
    GridPlacement::Auto
}

fn parse_line(start: &str, end: &str) -> Line<GridPlacement> {
    Line {
        start: parse_placement(start),
        end: parse_placement(end),
    }
}

fn parse_track(min: &str, max: &str) -> TrackSizingFunction {
    MinMax {
        min: parse_min_track(min),
        max: parse_max_track(max),
    }
}

fn parse_align_items(s: &str) -> AlignItems {
    parse_or(s, AlignItems::NORMAL)
}

fn parse_align_content(s: &str) -> AlignContent {
    parse_or(s, AlignContent::NORMAL)
}

/// Empty string or "auto" means unset (taffy Option::None).
fn parse_optional_align(s: &str) -> Option<AlignItems> {
    let s = s.trim();
    if s.is_empty() || s == "auto" {
        None
    } else {
        Some(parse_align_items(s))
    }
}

fn to_taffy_style(s: &ffi::NodeStyle) -> Style {
    let dim = |v: &str| parse_dim(v);
    let size_dim = |sz: &ffi::LengthSize| TSize {
        width: dim(&sz.width),
        height: dim(&sz.height),
    };
    let size_lpa = |sz: &ffi::LengthSize| TSize {
        width: parse_lpa(&sz.width),
        height: parse_lpa(&sz.height),
    };
    let size_lp = |sz: &ffi::LengthSize| TSize {
        width: parse_lp(&sz.width),
        height: parse_lp(&sz.height),
    };
    let rect_lpa = |r: &ffi::LengthRect| TRect {
        left: parse_lpa(&r.left),
        right: parse_lpa(&r.right),
        top: parse_lpa(&r.top),
        bottom: parse_lpa(&r.bottom),
    };
    let rect_lp = |r: &ffi::LengthRect| TRect {
        left: parse_lp(&r.left),
        right: parse_lp(&r.right),
        top: parse_lp(&r.top),
        bottom: parse_lp(&r.bottom),
    };

    Style {
        display: parse_or(&s.display, Display::Flex),
        item_is_table: s.item_is_table,
        item_is_replaced: s.item_is_replaced,
        item_is_compressible_replaced: s.item_is_compressible_replaced,
        box_sizing: parse_or(&s.box_sizing, BoxSizing::BorderBox),
        direction: parse_or(&s.direction, Direction::Ltr),
        overflow: TPoint {
            x: parse_or(&s.overflow_x, Overflow::Visible),
            y: parse_or(&s.overflow_y, Overflow::Visible),
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
        float: parse_or(&s.css_float, Float::None),
        clear: parse_or(&s.clear, Clear::None),
        position: parse_or(&s.position, Position::Static),
        inset: rect_lpa(&s.inset),
        size: size_dim(&s.size),
        min_size: size_lpa(&s.min_size),
        max_size: size_lpa(&s.max_size),
        aspect_ratio: if s.aspect_ratio.is_nan() {
            None
        } else {
            Some(s.aspect_ratio as f32)
        },
        margin: rect_lpa(&s.margin),
        padding: rect_lp(&s.padding),
        border: rect_lp(&s.border),
        align_items: parse_align_items(&s.align_items),
        align_self: parse_optional_align(&s.align_self),
        justify_items: parse_align_items(&s.justify_items),
        justify_self: parse_optional_align(&s.justify_self),
        align_content: parse_align_content(&s.align_content),
        justify_content: parse_align_content(&s.justify_content),
        gap: size_lp(&s.gap),
        text_align: parse_or(&s.text_align, TextAlign::Auto),
        flex_direction: parse_or(&s.flex_direction, FlexDirection::Row),
        flex_wrap: parse_or(&s.flex_wrap, FlexWrap::NoWrap),
        flex_line_count: s.flex_line_count.max(1) as u16,
        flex_basis: dim(&s.flex_basis),
        flex_grow: s.flex_grow as f32,
        flex_shrink: s.flex_shrink as f32,
        grid_auto_flow: parse_or(&s.grid_auto_flow, GridAutoFlow::Row),
        grid_template_rows: s
            .grid_template_rows
            .iter()
            .map(|t| GridTemplateComponent::Single(parse_track(&t.min, &t.max)))
            .collect(),
        grid_template_columns: s
            .grid_template_columns
            .iter()
            .map(|t| GridTemplateComponent::Single(parse_track(&t.min, &t.max)))
            .collect(),
        grid_auto_rows: s
            .grid_auto_rows
            .iter()
            .map(|t| parse_track(&t.min, &t.max))
            .collect(),
        grid_auto_columns: s
            .grid_auto_columns
            .iter()
            .map(|t| parse_track(&t.min, &t.max))
            .collect(),
        grid_row: parse_line(&s.grid_row.start, &s.grid_row.end),
        grid_column: parse_line(&s.grid_column.start, &s.grid_column.end),
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
        let avail = |v: &ffi::AvailableSpace| match v.kind {
            1 => TAvailableSpace::MinContent,
            2 => TAvailableSpace::MaxContent,
            _ => TAvailableSpace::Definite(v.value as f32),
        };
        let size = TSize {
            width: avail(width),
            height: avail(height),
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
