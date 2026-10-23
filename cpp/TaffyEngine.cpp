#include "taffy/TaffyEngine.h"

#include <cstdint>

// cxx-generated glue (global namespace types)
#include <taffy_cxxbridge/lib.h>

namespace taffy {

namespace {

::LengthRect toFfi(const LengthRect& r)
{
    ::LengthRect o;
    o.left = r.left;
    o.right = r.right;
    o.top = r.top;
    o.bottom = r.bottom;
    return o;
}

::LengthSize toFfi(const LengthSize& s)
{
    ::LengthSize o;
    o.width = s.width;
    o.height = s.height;
    return o;
}

::NodeStyle toFfi(const Style& s)
{
    ::NodeStyle o;
    o.display = s.display;
    o.item_is_table = s.itemIsTable;
    o.item_is_replaced = s.itemIsReplaced;
    o.item_is_compressible_replaced = s.itemIsCompressibleReplaced;
    o.box_sizing = s.boxSizing;
    o.direction = s.direction;
    o.overflow_x = s.overflowX;
    o.overflow_y = s.overflowY;
    o.scrollbar_width = s.scrollbarWidth;
    o.contain = s.contain;
    o.css_float = s.cssFloat;
    o.clear = s.clear;
    o.position = s.position;
    o.inset = toFfi(s.inset);
    o.size = toFfi(s.size);
    o.min_size = toFfi(s.minSize);
    o.max_size = toFfi(s.maxSize);
    o.aspect_ratio = s.aspectRatio;
    o.margin = toFfi(s.margin);
    o.padding = toFfi(s.padding);
    o.border = toFfi(s.border);
    o.align_items = s.alignItems;
    o.has_align_self = s.hasAlignSelf;
    o.align_self = s.alignSelf;
    o.justify_items = s.justifyItems;
    o.has_justify_self = s.hasJustifySelf;
    o.justify_self = s.justifySelf;
    o.align_content = s.alignContent;
    o.justify_content = s.justifyContent;
    o.gap = toFfi(s.gap);
    o.text_align = s.textAlign;
    o.flex_direction = s.flexDirection;
    o.flex_wrap = s.flexWrap;
    o.flex_line_count = s.flexLineCount;
    o.flex_basis = s.flexBasis;
    o.flex_grow = s.flexGrow;
    o.flex_shrink = s.flexShrink;
    o.grid_auto_flow = s.gridAutoFlow;
    o.grid_template_rows = {};
    for (const auto& t : s.gridTemplateRows) {
        ::TrackSize ts;
        ts.min = t.min;
        ts.max = t.max;
        o.grid_template_rows.push_back(ts);
    }
    o.grid_template_columns = {};
    for (const auto& t : s.gridTemplateColumns) {
        ::TrackSize ts;
        ts.min = t.min;
        ts.max = t.max;
        o.grid_template_columns.push_back(ts);
    }
    o.grid_auto_rows = {};
    for (const auto& t : s.gridAutoRows) {
        ::TrackSize ts;
        ts.min = t.min;
        ts.max = t.max;
        o.grid_auto_rows.push_back(ts);
    }
    o.grid_auto_columns = {};
    for (const auto& t : s.gridAutoColumns) {
        ::TrackSize ts;
        ts.min = t.min;
        ts.max = t.max;
        o.grid_auto_columns.push_back(ts);
    }
    o.grid_row.start = s.gridRow.start;
    o.grid_row.end = s.gridRow.end;
    o.grid_column.start = s.gridColumn.start;
    o.grid_column.end = s.gridColumn.end;
    return o;
}

} // namespace

class EngineImpl
{
public:
    rust::Box<::TaffyEngine> engine = ::engine_new();
};

Engine::Engine()
    : d(std::make_unique<EngineImpl>())
{
}

Engine::~Engine() = default;

unsigned Engine::addNode()
{
    return addNode(Style{});
}

unsigned Engine::addNode(const Style& style)
{
    return d->engine->engine_add_node(toFfi(style));
}

bool Engine::addChild(unsigned parent, unsigned child)
{
    try {
        d->engine->engine_add_child(parent, child);
        return true;
    } catch (...) {
        return false;
    }
}

bool Engine::setStyle(unsigned node, const Style& style)
{
    try {
        d->engine->engine_set_style(node, toFfi(style));
        return true;
    } catch (...) {
        return false;
    }
}

bool Engine::compute(const AvailableSpace& width, const AvailableSpace& height)
{
    ::AvailableSpace w;
    w.kind = static_cast<int32_t>(width.kind);
    w.value = width.value;
    ::AvailableSpace h;
    h.kind = static_cast<int32_t>(height.kind);
    h.value = height.value;
    try {
        d->engine->engine_compute(w, h);
        return true;
    } catch (...) {
        return false;
    }
}

bool Engine::compute(double availableWidth, double availableHeight)
{
    return compute(AvailableSpace::definite(availableWidth),
                   AvailableSpace::definite(availableHeight));
}

bool Engine::layout(unsigned node, Layout& out) const
{
    try {
        const ::Layout l = d->engine->engine_layout(node);
        out.order = l.order;
        out.location.x = l.location.x;
        out.location.y = l.location.y;
        out.size.width = l.size.width;
        out.size.height = l.size.height;
        out.border.left = l.border.left;
        out.border.right = l.border.right;
        out.border.top = l.border.top;
        out.border.bottom = l.border.bottom;
        out.padding.left = l.padding.left;
        out.padding.right = l.padding.right;
        out.padding.top = l.padding.top;
        out.padding.bottom = l.padding.bottom;
        out.margin.left = l.margin.left;
        out.margin.right = l.margin.right;
        out.margin.top = l.margin.top;
        out.margin.bottom = l.margin.bottom;
        out.scrollbarSize.width = l.scrollbar_size.width;
        out.scrollbarSize.height = l.scrollbar_size.height;
        return true;
    } catch (...) {
        return false;
    }
}

void Engine::clear()
{
    d->engine->engine_clear();
}

} // namespace taffy
