//! taffy layout engine exposed through a plain C ABI (no cxx).
//! All public entry points are `taffy_*` `extern "C"` so they cannot clash
//! with another crate's cxx-generated symbols.

use std::ffi::{c_char, CStr};
use std::panic::{catch_unwind, AssertUnwindSafe};
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
use taffy::{GridPlacement, GridTemplateArea, GridTemplateAreas, GridTemplateComponent, TrackSizingFunction};

// ---------------------------------------------------------------------------
// C ABI types
// ---------------------------------------------------------------------------

#[repr(C)]
pub struct TaffyCRect {
    pub left: f64,
    pub right: f64,
    pub top: f64,
    pub bottom: f64,
}

#[repr(C)]
pub struct TaffyCSize {
    pub width: f64,
    pub height: f64,
}

#[repr(C)]
pub struct TaffyCPoint {
    pub x: f64,
    pub y: f64,
}

#[repr(C)]
pub struct TaffyCLayout {
    pub order: u32,
    pub location: TaffyCPoint,
    pub size: TaffyCSize,
    pub border: TaffyCRect,
    pub padding: TaffyCRect,
    pub margin: TaffyCRect,
    pub scrollbar_size: TaffyCSize,
}

/// Two C strings: start / end placement ("" or NULL = "auto").
#[repr(C)]
pub struct TaffyCLine {
    pub start: *const c_char,
    pub end: *const c_char,
}

/// Track as two C strings: min / max CSS sizing functions.
#[repr(C)]
pub struct TaffyCTrack {
    pub min: *const c_char,
    pub max: *const c_char,
}

/// Full style; string fields are UTF-8 C strings (NULL = empty / auto).
#[repr(C)]
pub struct TaffyCStyle {
    pub display: *const c_char,
    pub item_is_table: bool,
    pub item_is_replaced: bool,
    pub item_is_compressible_replaced: bool,
    pub box_sizing: *const c_char,
    pub direction: *const c_char,
    pub overflow_x: *const c_char,
    pub overflow_y: *const c_char,
    pub scrollbar_width: f64,
    pub contain: u32,
    pub css_float: *const c_char,
    pub clear: *const c_char,
    pub position: *const c_char,

    pub inset_left: *const c_char,
    pub inset_right: *const c_char,
    pub inset_top: *const c_char,
    pub inset_bottom: *const c_char,

    pub size_width: *const c_char,
    pub size_height: *const c_char,
    pub min_size_width: *const c_char,
    pub min_size_height: *const c_char,
    pub max_size_width: *const c_char,
    pub max_size_height: *const c_char,

    pub aspect_ratio: f64,

    pub margin_left: *const c_char,
    pub margin_right: *const c_char,
    pub margin_top: *const c_char,
    pub margin_bottom: *const c_char,

    pub padding_left: *const c_char,
    pub padding_right: *const c_char,
    pub padding_top: *const c_char,
    pub padding_bottom: *const c_char,

    pub border_left: *const c_char,
    pub border_right: *const c_char,
    pub border_top: *const c_char,
    pub border_bottom: *const c_char,

    pub align_items: *const c_char,
    pub align_self: *const c_char,
    pub justify_items: *const c_char,
    pub justify_self: *const c_char,
    pub align_content: *const c_char,
    pub justify_content: *const c_char,

    pub gap_width: *const c_char,
    pub gap_height: *const c_char,

    pub text_align: *const c_char,
    pub flex_direction: *const c_char,
    pub flex_wrap: *const c_char,
    pub flex_line_count: u32,
    pub flex_basis: *const c_char,
    pub flex_grow: f64,
    pub flex_shrink: f64,

    pub grid_auto_flow: *const c_char,
    pub grid_template_rows: *const c_char,
    pub grid_template_columns: *const c_char,
    pub grid_auto_rows: *const c_char,
    pub grid_auto_columns: *const c_char,
    pub grid_template_areas: *const c_char,

    pub grid_template_row_names: *const *const c_char,
    pub grid_template_row_names_len: usize,
    pub grid_template_column_names: *const *const c_char,
    pub grid_template_column_names_len: usize,

    pub grid_row: TaffyCLine,
    pub grid_column: TaffyCLine,

    pub measure_width: f64,
    pub measure_height: f64,
}

#[repr(C)]
pub struct TaffyCAvailableSpace {
    /// "100px" | "min-content" | "max-content"
    pub css: *const c_char,
}

// ---------------------------------------------------------------------------
// C helpers
// ---------------------------------------------------------------------------

unsafe fn cstr<'a>(p: *const c_char) -> &'a str {
    if p.is_null() {
        return "";
    }
    CStr::from_ptr(p).to_str().unwrap_or("")
}

// ---------------------------------------------------------------------------
// CSS mapping (same rules as before)
// ---------------------------------------------------------------------------

fn parse_dim(s: &str) -> Dimension {
    let s = s.trim();
    if s.is_empty() {
        return Dimension::auto();
    }
    if let Ok(d) = Dimension::from_str(s) {
        return d;
    }
    if let Ok(v) = s.parse::<f32>() {
        return Dimension::length(v);
    }
    Dimension::auto()
}

fn parse_lp(s: &str) -> LengthPercentage {
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
            if let Ok(v) = s.parse::<f32>() {
                return TAvailableSpace::Definite(v);
            }
            TAvailableSpace::MinContent
        }
    }
}

fn parse_align_items(s: &str) -> AlignItems {
    AlignItems::from_str(s).unwrap_or(AlignItems::NORMAL)
}

fn parse_align_content(s: &str) -> AlignContent {
    AlignContent::from_str(s).unwrap_or(AlignContent::NORMAL)
}

fn parse_optional_align(s: &str) -> Option<AlignItems> {
    let s = s.trim();
    if s.is_empty() || s == "auto" {
        None
    } else {
        Some(parse_align_items(s))
    }
}

fn parse_grid_template(css: &str) -> Vec<GridTemplateComponent<String>> {
    let css = css.trim();
    if css.is_empty() {
        return Vec::new();
    }
    if let Ok(c) = GridTemplateComponent::<String>::from_str(css) {
        return vec![c];
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

fn parse_grid_areas(css: &str) -> Option<GridTemplateAreas<String>> {
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
        let mut rows = Vec::new();
        let mut buf = String::new();
        let mut in_quote = false;
        for ch in css.chars() {
            match ch {
                '"' => {
                    if in_quote {
                        let cur: Vec<String> =
                            buf.split_whitespace().map(|s| s.to_string()).collect();
                        if !cur.is_empty() {
                            rows.push(cur);
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
    let mut areas: Vec<GridTemplateArea<String>> = Vec::new();
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
            areas.push(GridTemplateArea {
                name: name.clone(),
                row_start: r as u16,
                row_end: r_end as u16 + 1,
                column_start: c as u16,
                column_end: c_end as u16 + 1,
            });
        }
    }
    Some(GridTemplateAreas {
        areas,
        row_count,
        column_count,
    })
}

fn parse_line_names(css: &str) -> Vec<String> {
    css.split_whitespace()
        .filter(|s| !s.is_empty())
        .map(|s| s.to_string())
        .collect()
}

fn unsafe_style_to_taffy(s: &TaffyCStyle) -> Style {
    unsafe {
        let strf = cstr;
        let dim = |p: *const c_char| parse_dim(strf(p));
        let lp = |p: *const c_char| parse_lp(strf(p));
        let lpa = |p: *const c_char| parse_lpa(strf(p));

        let mut grid_template_row_names = Vec::new();
        if !s.grid_template_row_names.is_null() {
            for i in 0..s.grid_template_row_names_len {
                let p = *s.grid_template_row_names.add(i);
                grid_template_row_names.push(parse_line_names(strf(p)));
            }
        }
        let mut grid_template_column_names = Vec::new();
        if !s.grid_template_column_names.is_null() {
            for i in 0..s.grid_template_column_names_len {
                let p = *s.grid_template_column_names.add(i);
                grid_template_column_names.push(parse_line_names(strf(p)));
            }
        }

        Style {
            display: parse_or_display(strf(s.display)),
            item_is_table: s.item_is_table,
            item_is_replaced: s.item_is_replaced,
            item_is_compressible_replaced: s.item_is_compressible_replaced,
            box_sizing: parse_or_box(strf(s.box_sizing)),
            direction: parse_or_direction(strf(s.direction)),
            overflow: TPoint {
                x: parse_or_overflow(strf(s.overflow_x)),
                y: parse_or_overflow(strf(s.overflow_y)),
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
            float: parse_or_float(strf(s.css_float)),
            clear: parse_or_clear(strf(s.clear)),
            position: parse_or_position(strf(s.position)),
            inset: TRect {
                left: lpa(s.inset_left),
                right: lpa(s.inset_right),
                top: lpa(s.inset_top),
                bottom: lpa(s.inset_bottom),
            },
            size: TSize {
                width: dim(s.size_width),
                height: dim(s.size_height),
            },
            min_size: TSize {
                width: lpa(s.min_size_width),
                height: lpa(s.min_size_height),
            },
            max_size: TSize {
                width: lpa(s.max_size_width),
                height: lpa(s.max_size_height),
            },
            aspect_ratio: if s.aspect_ratio.is_nan() {
                None
            } else {
                Some(s.aspect_ratio as f32)
            },
            margin: TRect {
                left: lpa(s.margin_left),
                right: lpa(s.margin_right),
                top: lpa(s.margin_top),
                bottom: lpa(s.margin_bottom),
            },
            padding: TRect {
                left: lp(s.padding_left),
                right: lp(s.padding_right),
                top: lp(s.padding_top),
                bottom: lp(s.padding_bottom),
            },
            border: TRect {
                left: lp(s.border_left),
                right: lp(s.border_right),
                top: lp(s.border_top),
                bottom: lp(s.border_bottom),
            },
            align_items: parse_align_items(strf(s.align_items)),
            align_self: parse_optional_align(strf(s.align_self)),
            justify_items: parse_align_items(strf(s.justify_items)),
            justify_self: parse_optional_align(strf(s.justify_self)),
            align_content: parse_align_content(strf(s.align_content)),
            justify_content: parse_align_content(strf(s.justify_content)),
            gap: TSize {
                width: lp(s.gap_width),
                height: lp(s.gap_height),
            },
            text_align: parse_or_text_align(strf(s.text_align)),
            flex_direction: parse_or_flex_direction(strf(s.flex_direction)),
            flex_wrap: parse_or_flex_wrap(strf(s.flex_wrap)),
            flex_line_count: s.flex_line_count.max(1) as u16,
            flex_basis: dim(s.flex_basis),
            flex_grow: s.flex_grow as f32,
            flex_shrink: s.flex_shrink as f32,
            grid_auto_flow: parse_or_grid_flow(strf(s.grid_auto_flow)),
            grid_template_rows: parse_grid_template(strf(s.grid_template_rows)),
            grid_template_columns: parse_grid_template(strf(s.grid_template_columns)),
            grid_auto_rows: parse_auto_tracks(strf(s.grid_auto_rows)),
            grid_auto_columns: parse_auto_tracks(strf(s.grid_auto_columns)),
            grid_template_areas: parse_grid_areas(strf(s.grid_template_areas)),
            grid_template_row_names,
            grid_template_column_names,
            grid_row: Line {
                start: parse_placement(strf(s.grid_row.start)),
                end: parse_placement(strf(s.grid_row.end)),
            },
            grid_column: Line {
                start: parse_placement(strf(s.grid_column.start)),
                end: parse_placement(strf(s.grid_column.end)),
            },
            ..Style::DEFAULT
        }
    }
}

fn parse_or_display(s: &str) -> Display {
    s.parse().unwrap_or(Display::Flex)
}
fn parse_or_box(s: &str) -> BoxSizing {
    s.parse().unwrap_or(BoxSizing::BorderBox)
}
fn parse_or_direction(s: &str) -> Direction {
    s.parse().unwrap_or(Direction::Ltr)
}
fn parse_or_overflow(s: &str) -> Overflow {
    s.parse().unwrap_or(Overflow::Visible)
}
fn parse_or_float(s: &str) -> Float {
    s.parse().unwrap_or(Float::None)
}
fn parse_or_clear(s: &str) -> Clear {
    s.parse().unwrap_or(Clear::None)
}
fn parse_or_position(s: &str) -> Position {
    s.parse().unwrap_or(Position::Static)
}
fn parse_or_text_align(s: &str) -> TextAlign {
    s.parse().unwrap_or(TextAlign::Auto)
}
fn parse_or_flex_direction(s: &str) -> FlexDirection {
    s.parse().unwrap_or(FlexDirection::Row)
}
fn parse_or_flex_wrap(s: &str) -> FlexWrap {
    s.parse().unwrap_or(FlexWrap::NoWrap)
}
fn parse_or_grid_flow(s: &str) -> GridAutoFlow {
    s.parse().unwrap_or(GridAutoFlow::Row)
}

// ---------------------------------------------------------------------------
// Engine
// ---------------------------------------------------------------------------

pub struct TaffyEngine {
    tree: TaffyTree,
    nodes: Vec<NodeId>,
}

impl TaffyEngine {
    fn new() -> Self {
        Self {
            tree: TaffyTree::new(),
            nodes: Vec::new(),
        }
    }

    fn node(&self, index: u32) -> Result<NodeId, String> {
        self.nodes
            .get(index as usize)
            .copied()
            .ok_or_else(|| format!("unknown node id {index}"))
    }
}

fn taffy_err(e: TaffyError) -> String {
    e.to_string()
}

// ---------------------------------------------------------------------------
// extern "C" API
// ---------------------------------------------------------------------------

/// Create engine. Returns NULL on failure.
#[no_mangle]
pub unsafe extern "C" fn taffy_engine_new() -> *mut TaffyEngine {
    match catch_unwind(|| Box::new(TaffyEngine::new())) {
        Ok(b) => Box::into_raw(b),
        Err(_) => std::ptr::null_mut(),
    }
}

/// Destroy engine. NULL is a no-op.
#[no_mangle]
pub unsafe extern "C" fn taffy_engine_delete(engine: *mut TaffyEngine) {
    if !engine.is_null() {
        drop(Box::from_raw(engine));
    }
}

/// Add node; returns node id (u32::MAX on error).
#[no_mangle]
pub unsafe extern "C" fn taffy_engine_add_node(
    engine: *mut TaffyEngine,
    style: *const TaffyCStyle,
) -> u32 {
    if engine.is_null() {
        return u32::MAX;
    }
    let eng = &mut *engine;
    let style = if style.is_null() {
        Style::DEFAULT
    } else {
        unsafe_style_to_taffy(&*style)
    };
    match catch_unwind(AssertUnwindSafe(|| eng.tree.new_leaf(style))) {
        Ok(Ok(node)) => {
            eng.nodes.push(node);
            (eng.nodes.len() - 1) as u32
        }
        _ => u32::MAX,
    }
}

/// 0 = ok, 1 = error.
#[no_mangle]
pub unsafe extern "C" fn taffy_engine_add_child(
    engine: *mut TaffyEngine,
    parent: u32,
    child: u32,
) -> i32 {
    if engine.is_null() {
        return 1;
    }
    let eng = &mut *engine;
    let result = catch_unwind(AssertUnwindSafe(|| {
        let p = eng.node(parent).map_err(|e| e)?;
        let c = eng.node(child).map_err(|e| e)?;
        eng.tree.add_child(p, c).map_err(taffy_err)
    }));
    match result {
        Ok(Ok(())) => 0,
        _ => 1,
    }
}

#[no_mangle]
pub unsafe extern "C" fn taffy_engine_set_style(
    engine: *mut TaffyEngine,
    node: u32,
    style: *const TaffyCStyle,
) -> i32 {
    if engine.is_null() || style.is_null() {
        return 1;
    }
    let eng = &mut *engine;
    let style = unsafe_style_to_taffy(&*style);
    let result = catch_unwind(AssertUnwindSafe(|| {
        let n = eng.node(node).map_err(|e| e)?;
        eng.tree.set_style(n, style).map_err(taffy_err)
    }));
    match result {
        Ok(Ok(())) => 0,
        _ => 1,
    }
}

#[no_mangle]
pub unsafe extern "C" fn taffy_engine_compute(
    engine: *mut TaffyEngine,
    width_css: *const c_char,
    height_css: *const c_char,
) -> i32 {
    if engine.is_null() {
        return 1;
    }
    let eng = &mut *engine;
    let result = catch_unwind(AssertUnwindSafe(|| {
        let Some(&root) = eng.nodes.first() else {
            return Ok(());
        };
        let size = TSize {
            width: parse_available_space(cstr(width_css)),
            height: parse_available_space(cstr(height_css)),
        };
        eng.tree.compute_layout(root, size).map_err(taffy_err)
    }));
    match result {
        Ok(Ok(())) => 0,
        _ => 1,
    }
}

#[no_mangle]
pub unsafe extern "C" fn taffy_engine_layout(
    engine: *const TaffyEngine,
    node: u32,
    out: *mut TaffyCLayout,
) -> i32 {
    if engine.is_null() || out.is_null() {
        return 1;
    }
    let eng = &*engine;
    let result = catch_unwind(AssertUnwindSafe(|| {
        let n = eng.node(node).map_err(|e| e)?;
        eng.tree.layout(n).map(|l| {
            let o = TaffyCLayout {
                order: l.order,
                location: TaffyCPoint {
                    x: l.location.x as f64,
                    y: l.location.y as f64,
                },
                size: TaffyCSize {
                    width: l.size.width as f64,
                    height: l.size.height as f64,
                },
                border: TaffyCRect {
                    left: l.border.left as f64,
                    right: l.border.right as f64,
                    top: l.border.top as f64,
                    bottom: l.border.bottom as f64,
                },
                padding: TaffyCRect {
                    left: l.padding.left as f64,
                    right: l.padding.right as f64,
                    top: l.padding.top as f64,
                    bottom: l.padding.bottom as f64,
                },
                margin: TaffyCRect {
                    left: l.margin.left as f64,
                    right: l.margin.right as f64,
                    top: l.margin.top as f64,
                    bottom: l.margin.bottom as f64,
                },
                scrollbar_size: TaffyCSize {
                    width: l.scrollbar_size.width as f64,
                    height: l.scrollbar_size.height as f64,
                },
            };
            *out = o;
        })
        .map_err(taffy_err)
    }));
    match result {
        Ok(Ok(())) => 0,
        _ => 1,
    }
}

#[no_mangle]
pub unsafe extern "C" fn taffy_engine_clear(engine: *mut TaffyEngine) {
    if engine.is_null() {
        return;
    }
    let eng = &mut *engine;
    let _ = catch_unwind(AssertUnwindSafe(|| {
        eng.tree = TaffyTree::new();
        eng.nodes.clear();
    }));
}
