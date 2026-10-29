#include "taffy/TaffyEngine.h"
#include "taffy/TaffyC.h"

#include <string>
#include <vector>

namespace taffy {

namespace {

const char* s(const std::string& v)
{
    return v.c_str();
}

void fillStyle(const Style& src, TaffyCStyle& o, std::vector<std::string>& nameRows,
               std::vector<std::string>& nameCols,
               std::vector<const char*>& rowPtrs, std::vector<const char*>& colPtrs)
{
    o = TaffyCStyle{};
    o.display = s(src.display);
    o.item_is_table = src.itemIsTable;
    o.item_is_replaced = src.itemIsReplaced;
    o.item_is_compressible_replaced = src.itemIsCompressibleReplaced;
    o.box_sizing = s(src.boxSizing);
    o.direction = s(src.direction);
    o.overflow_x = s(src.overflowX);
    o.overflow_y = s(src.overflowY);
    o.scrollbar_width = src.scrollbarWidth;
    o.contain = src.contain;
    o.css_float = s(src.cssFloat);
    o.clear = s(src.clear);
    o.position = s(src.position);

    o.inset_left = s(src.inset.left);
    o.inset_right = s(src.inset.right);
    o.inset_top = s(src.inset.top);
    o.inset_bottom = s(src.inset.bottom);

    o.size_width = s(src.size.width);
    o.size_height = s(src.size.height);
    o.min_size_width = s(src.minSize.width);
    o.min_size_height = s(src.minSize.height);
    o.max_size_width = s(src.maxSize.width);
    o.max_size_height = s(src.maxSize.height);

    o.aspect_ratio = src.aspectRatio;

    o.margin_left = s(src.margin.left);
    o.margin_right = s(src.margin.right);
    o.margin_top = s(src.margin.top);
    o.margin_bottom = s(src.margin.bottom);

    o.padding_left = s(src.padding.left);
    o.padding_right = s(src.padding.right);
    o.padding_top = s(src.padding.top);
    o.padding_bottom = s(src.padding.bottom);

    o.border_left = s(src.border.left);
    o.border_right = s(src.border.right);
    o.border_top = s(src.border.top);
    o.border_bottom = s(src.border.bottom);

    o.align_items = s(src.alignItems);
    o.align_self = s(src.alignSelf);
    o.justify_items = s(src.justifyItems);
    o.justify_self = s(src.justifySelf);
    o.align_content = s(src.alignContent);
    o.justify_content = s(src.justifyContent);

    o.gap_width = s(src.gap.width);
    o.gap_height = s(src.gap.height);

    o.text_align = s(src.textAlign);
    o.flex_direction = s(src.flexDirection);
    o.flex_wrap = s(src.flexWrap);
    o.flex_line_count = src.flexLineCount;
    o.flex_basis = s(src.flexBasis);
    o.flex_grow = src.flexGrow;
    o.flex_shrink = src.flexShrink;

    o.grid_auto_flow = s(src.gridAutoFlow);
    o.grid_template_rows = s(src.gridTemplateRows);
    o.grid_template_columns = s(src.gridTemplateColumns);
    o.grid_auto_rows = s(src.gridAutoRows);
    o.grid_auto_columns = s(src.gridAutoColumns);
    o.grid_template_areas = s(src.gridTemplateAreas);

    nameRows.clear();
    nameCols.clear();
    rowPtrs.clear();
    colPtrs.clear();
    for (const auto& n : src.gridTemplateRowNames) {
        nameRows.push_back(n);
    }
    for (const auto& n : src.gridTemplateColumnNames) {
        nameCols.push_back(n);
    }
    for (const auto& n : nameRows) {
        rowPtrs.push_back(n.c_str());
    }
    for (const auto& n : nameCols) {
        colPtrs.push_back(n.c_str());
    }
    o.grid_template_row_names = rowPtrs.empty() ? nullptr : rowPtrs.data();
    o.grid_template_row_names_len = rowPtrs.size();
    o.grid_template_column_names = colPtrs.empty() ? nullptr : colPtrs.data();
    o.grid_template_column_names_len = colPtrs.size();

    o.grid_row.start = s(src.gridRow.start);
    o.grid_row.end = s(src.gridRow.end);
    o.grid_column.start = s(src.gridColumn.start);
    o.grid_column.end = s(src.gridColumn.end);

    o.measure_width = src.measureWidth;
    o.measure_height = src.measureHeight;
}

} // namespace

class EngineImpl
{
public:
    TaffyEngine* engine = nullptr;

    // Keep string storage alive for fillStyle's const char* view.
    std::vector<std::string> nameRows;
    std::vector<std::string> nameCols;
    std::vector<const char*> rowPtrs;
    std::vector<const char*> colPtrs;
};

Engine::Engine()
    : d(std::make_unique<EngineImpl>())
{
    d->engine = taffy_engine_new();
}

Engine::~Engine()
{
    if (d && d->engine) {
        taffy_engine_delete(d->engine);
        d->engine = nullptr;
    }
}

std::uint32_t Engine::addNode()
{
    return addNode(Style{});
}

std::uint32_t Engine::addNode(const Style& style)
{
    TaffyCStyle c{};
    fillStyle(style, c, d->nameRows, d->nameCols, d->rowPtrs, d->colPtrs);
    return taffy_engine_add_node(d->engine, &c);
}

bool Engine::addChild(std::uint32_t parent, std::uint32_t child)
{
    return taffy_engine_add_child(d->engine, parent, child) == 0;
}

bool Engine::setStyle(std::uint32_t node, const Style& style)
{
    TaffyCStyle c{};
    fillStyle(style, c, d->nameRows, d->nameCols, d->rowPtrs, d->colPtrs);
    return taffy_engine_set_style(d->engine, node, &c) == 0;
}

bool Engine::compute(const AvailableSpace& width, const AvailableSpace& height)
{
    return taffy_engine_compute(d->engine, width.css.c_str(), height.css.c_str()) == 0;
}

bool Engine::compute(double availableWidth, double availableHeight)
{
    return compute(AvailableSpace::definite(availableWidth),
                   AvailableSpace::definite(availableHeight));
}

bool Engine::layout(std::uint32_t node, Layout& out) const
{
    TaffyCLayout l{};
    if (taffy_engine_layout(d->engine, node, &l) != 0) {
        return false;
    }
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
}

void Engine::clear()
{
    taffy_engine_clear(d->engine);
}

} // namespace taffy
