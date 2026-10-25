//! Pure cxx bridge. Style values are CSS-like strings (parsed by taffy).

use std::str::FromStr;

use taffy::geometry::{Line, Point as TPoint, Rect as TRect, Size as TSize};
use taffy::prelude::{
    AlignContent, AlignItems, AvailableSpace as TAvailableSpace, BoxSizing, Dimension, Display,
    FlexDirection, FlexWrap, LengthPercentage, LengthPercentageAuto, NodeId, Position, Style,
};
use taffy::style::{Clear, Direction, Float, GridAutoFlow, Overflow, TextAlign};
use taffy::style_helpers::{length, percent};
use taffy::tree::TaffyError;
use taffy::TaffyTree;
use taffy::{
    GridPlacement, GridTemplateComponent, TrackSizingFunction,
};

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
        /// "100px" | "min-content" | "max-content"
        css: String,
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
        // CSS track lists: "100px 1fr" | "repeat(2, minmax(100px, 1fr))"
        grid_template_rows: String,
        grid_template_columns: String,
        // CSS track sizing lists: "auto 100px" (no repeat)
        grid_auto_rows: String,
        grid_auto_columns: String,
        // CSS: "\"header header\" \"main sidebar\"" or header header / main sidebar
        grid_template_areas: String,
        // one entry per track: "start end" | "" | "line1"
        grid_template_row_names: Vec<String>,
        grid_template_column_names: Vec<String>,
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

fn parse_available_space(s: &str) -> TAvailableSpace {
    let s = s.trim();
    match s {
        "min-content" => TAvailableSpace::MinContent,
        "max-content" => TAvailableSpace::MaxContent,
        _ => {
            if let Some(v) = s.strip_suffix("px") {
                if let Ok(v) = v.trim().parse::<f32>() {
                    return TAvailableSpace::Definite(v);
                }
            }
            if let Some(p) = s.strip_suffix('%') {
                if let Ok(v) = p.trim().parse::<f32>() {
                    // percent has no parent size at compute() — treat as 0
                    let _ = v;
                    return TAvailableSpace::Definite(0.0);
                }
            }
            if let Ok(v) = s.parse::<f32>() {
                return TAvailableSpace::Definite(v);
            }
            TAvailableSpace::MinContent
        }
    }
}

fn parse_dim(s: &str) -> Dimension {
    let s = s.trim();
    if s.is_empty() {
        return Dimension::auto();
    }
    if let Ok(d) = Dimension::from_str(s) {
        return d;
    }
    // Bare number → px (e.g. "100")
    if let Ok(v) = s.parse::<f32>() {
        return Dimension::length(v);
    }
    Dimension::auto()
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
    // "100" style
    if let Ok(v) = s.trim().parse::<f32>() {
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


/// Parse a CSS track list into GridTemplateComponent items.
fn parse_grid_template(css: &str) -> Vec<GridTemplateComponent<String>> {
    let css = css.trim();
    if css.is_empty() {
        return Vec::new();
    }
    // Single component (may be repeat(...))
    if let Ok(c) = GridTemplateComponent::<String>::from_str(css) {
        return vec![c];
    }
    // Split on whitespace at top level, then parse each token / repeat() group.
    let mut out = Vec::new();
    let mut token = String::new();
    let mut depth = 0usize;
    for ch in css.chars() {
        match ch {
            '(' => {
                depth += 1;
                token.push(ch);
            }
            ')' => {
                depth = depth.saturating_sub(1);
                token.push(ch);
            }
            c if c.is_whitespace() && depth == 0 => {
                if !token.is_empty() {
                    if let Ok(item) = GridTemplateComponent::<String>::from_str(&token) {
                        out.push(item);
                    }
                    token.clear();
                }
            }
            c => token.push(c),
        }
    }
    if !token.is_empty() {
        if let Ok(item) = GridTemplateComponent::<String>::from_str(&token) {
            out.push(item);
        }
    }
    out
}

/// Parse CSS track sizing list (auto rows/columns): "auto 100px minmax(0, 1fr)"
fn parse_auto_tracks(css: &str) -> Vec<TrackSizingFunction> {
    let css = css.trim();
    if css.is_empty() {
        return Vec::new();
    }
    if let Ok(t) = TrackSizingFunction::from_str(css) {
        return vec![t];
    }
    let mut out = Vec::new();
    let mut token = String::new();
    let mut depth = 0usize;
    for ch in css.chars() {
        match ch {
            '(' => {
                depth += 1;
                token.push(ch);
            }
            ')' => {
                depth = depth.saturating_sub(1);
                token.push(ch);
            }
            c if c.is_whitespace() && depth == 0 => {
                if !token.is_empty() {
                    if let Ok(item) = TrackSizingFunction::from_str(&token) {
                        out.push(item);
                    }
                    token.clear();
                }
            }
            c => token.push(c),
        }
    }
    if !token.is_empty() {
        if let Ok(item) = TrackSizingFunction::from_str(&token) {
            out.push(item);
        }
    }
    out
}

/// Parse CSS grid-template-areas.
/// Accepts: `"a a" "b c"`  or  `a a / b c`
fn parse_grid_areas(css: &str) -> Option<taffy::GridTemplateAreas<String>> {
    let css = css.trim();
    if css.is_empty() {
        return None;
    }
    let rows: Vec<Vec<String>> = if css.contains('/') {
        css.split('/')
            .map(|row| {
                row.split_whitespace()
                    .map(|s| s.trim_matches('"').to_string())
                    .collect()
            })
            .filter(|r: &Vec<String>| !r.is_empty())
            .collect()
    } else {
        // quoted row strings: "a a" "b c"
        let mut rows = Vec::new();
        let mut cur: Vec<String>;
        let mut in_quote = false;
        let mut buf = String::new();
        for ch in css.chars() {
            match ch {
                '"' => {
                    if in_quote {
                        cur = buf.split_whitespace().map(|s| s.to_string()).collect();
                        if !cur.is_empty() {
                            rows.push(std::mem::take(&mut cur));
                        }
                        buf.clear();
                        in_quote = false;
                    } else {
                        in_quote = true;
                        buf.clear();
                    }
                }
                c if in_quote => buf.push(c),
                _ => {}
            }
        }
        rows
    };
    if rows.is_empty() {
        return None;
    }
    let row_count = rows.len() as u16;
    let column_count = rows.iter().map(|r| r.len()).max().unwrap_or(0) as u16;

    // Merge rectangular regions of identical names.
    let mut areas: Vec<taffy::GridTemplateArea<String>> = Vec::new();
    let mut used = vec![vec![false; column_count as usize]; row_count as usize];
    for r in 0..row_count as usize {
        for c in 0..column_count as usize {
            if used[r][c] {
                continue;
            }
            let name = rows[r].get(c).cloned().unwrap_or_else(|| ".".into());
            if name == "." {
                used[r][c] = true;
                continue;
            }
            let mut r_end = r;
            while r_end + 1 < row_count as usize
                && !used[r_end + 1][c]
                && rows[r_end + 1].get(c).map(|s| s.as_str()) == Some(name.as_str())
            {
                r_end += 1;
            }
            let mut c_end = c;
            while c_end + 1 < column_count as usize
                && rows[r].get(c_end + 1).map(|s| s.as_str()) == Some(name.as_str())
            {
                c_end += 1;
            }
            for rr in r..=r_end {
                for cc in c..=c_end {
                    if rows[rr].get(cc).map(|s| s.as_str()) == Some(name.as_str()) {
                        used[rr][cc] = true;
                    }
                }
            }
            areas.push(taffy::GridTemplateArea {
                name: name.clone(),
                row_start: r as u16,
                row_end: r_end as u16 + 1,
                column_start: c as u16,
                column_end: c_end as u16 + 1,
            });
        }
    }

    Some(taffy::GridTemplateAreas {
        areas,
        row_count,
        column_count,
    })
}

/// Split "start end" into names for one track.
fn parse_line_names(css: &str) -> Vec<String> {
    css.split_whitespace()
        .filter(|s| !s.is_empty())
        .map(|s| s.to_string())
        .collect()
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
        grid_template_rows: parse_grid_template(&s.grid_template_rows),
        grid_template_columns: parse_grid_template(&s.grid_template_columns),
        grid_auto_rows: parse_auto_tracks(&s.grid_auto_rows),
        grid_auto_columns: parse_auto_tracks(&s.grid_auto_columns),
        grid_template_areas: parse_grid_areas(&s.grid_template_areas),
        grid_template_row_names: s
            .grid_template_row_names
            .iter()
            .map(|n| parse_line_names(n))
            .collect(),
        grid_template_column_names: s
            .grid_template_column_names
            .iter()
            .map(|n| parse_line_names(n))
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
        let avail = |v: &ffi::AvailableSpace| parse_available_space(&v.css);
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

#[cfg(test)]
mod tests {
    use super::*;
    use taffy::prelude::*;
    use taffy::TaffyTree;

    #[test]
    fn column_grow_with_basis_zero() {
        let mut tree: TaffyTree<()> = TaffyTree::new();
        let root = tree
            .new_leaf(Style {
                display: Display::Flex,
                flex_direction: FlexDirection::Column,
                size: Size {
                    width: length(460.0),
                    height: length(340.0),
                },
                ..Style::DEFAULT
            })
            .unwrap();
        let bar = tree
            .new_leaf(Style {
                size: Size {
                    width: Dimension::auto(),
                    height: length(100.0),
                },
                ..Style::DEFAULT
            })
            .unwrap();
        let inner = tree
            .new_leaf(Style {
                size: Size {
                    width: Dimension::auto(),
                    height: length(0.0),
                },
                flex_basis: length(0.0),
                flex_grow: 1.0,
                ..Style::DEFAULT
            })
            .unwrap();
        tree.add_child(root, bar).unwrap();
        tree.add_child(root, inner).unwrap();
        tree.compute_layout(
            root,
            Size {
                width: AvailableSpace::Definite(460.0),
                height: AvailableSpace::Definite(340.0),
            },
        )
        .unwrap();
        let l = tree.layout(inner).unwrap();
        println!("inner layout {}x{} at {},{}", l.size.width, l.size.height, l.location.x, l.location.y);
        assert!(l.size.height > 100.0, "expected grow, got {}", l.size.height);
    }

    #[test]
    fn column_grow_with_size_auto_basis_pct() {
        let mut tree: TaffyTree<()> = TaffyTree::new();
        let root = tree
            .new_leaf(Style {
                display: Display::Flex,
                flex_direction: FlexDirection::Column,
                size: Size {
                    width: length(460.0),
                    height: length(340.0),
                },
                ..Style::DEFAULT
            })
            .unwrap();
        let bar = tree
            .new_leaf(Style {
                size: Size {
                    width: Dimension::auto(),
                    height: length(100.0),
                },
                ..Style::DEFAULT
            })
            .unwrap();
        let inner = tree
            .new_leaf(Style {
                size: Size {
                    width: Dimension::auto(),
                    height: Dimension::auto(),
                },
                flex_basis: percent(0.0),
                flex_grow: 1.0,
                ..Style::DEFAULT
            })
            .unwrap();
        tree.add_child(root, bar).unwrap();
        tree.add_child(root, inner).unwrap();
        tree.compute_layout(
            root,
            Size {
                width: AvailableSpace::Definite(460.0),
                height: AvailableSpace::Definite(340.0),
            },
        )
        .unwrap();
        let l = tree.layout(inner).unwrap();
        println!("inner2 layout {}x{}", l.size.width, l.size.height);
        assert!(l.size.height > 100.0, "expected grow, got {}", l.size.height);
    }
}
