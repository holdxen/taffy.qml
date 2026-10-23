#include "taffy/TaffyEngine.h"

#include <cstdint>
#include <limits>

namespace taffy {

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

quint32 Engine::addNode()
{
    ::NodeStyle style;
    style.display = static_cast<int32_t>(Display::Flex);
    style.position = static_cast<int32_t>(Position::Static);
    style.flex_direction = static_cast<int32_t>(FlexDirection::Row);
    style.flex_wrap = static_cast<int32_t>(FlexWrap::NoWrap);
    style.align_items = static_cast<int32_t>(AlignItemsKeyword::Normal);
    style.align_content = static_cast<int32_t>(AlignContentKeyword::Normal);
    style.justify_content = static_cast<int32_t>(AlignContentKeyword::Normal);
    style.overflow_x = static_cast<int32_t>(Overflow::Visible);
    style.overflow_y = static_cast<int32_t>(Overflow::Visible);
    style.box_sizing = static_cast<int32_t>(BoxSizing::BorderBox);
    style.direction = static_cast<int32_t>(StyleDirection::Ltr);
    style.width = std::numeric_limits<double>::quiet_NaN();
    style.height = std::numeric_limits<double>::quiet_NaN();
    style.flex_grow = 0.0;
    style.flex_shrink = 1.0;
    return d->engine->engine_add_node(style);
}

bool Engine::addChild(quint32 parent, quint32 child)
{
    try {
        d->engine->engine_add_child(parent, child);
        return true;
    } catch (...) {
        return false;
    }
}

bool Engine::setStyle(quint32 node,
                      Display display,
                      FlexDirection flexDirection,
                      FlexWrap flexWrap,
                      AlignItemsKeyword alignItems,
                      AlignContentKeyword alignContent,
                      AlignContentKeyword justifyContent,
                      qreal width, qreal height,
                      qreal flexGrow, qreal flexShrink)
{
    ::NodeStyle style;
    style.display = static_cast<int32_t>(display);
    style.position = static_cast<int32_t>(Position::Static);
    style.flex_direction = static_cast<int32_t>(flexDirection);
    style.flex_wrap = static_cast<int32_t>(flexWrap);
    style.align_items = static_cast<int32_t>(alignItems);
    style.align_content = static_cast<int32_t>(alignContent);
    style.justify_content = static_cast<int32_t>(justifyContent);
    style.overflow_x = static_cast<int32_t>(Overflow::Visible);
    style.overflow_y = static_cast<int32_t>(Overflow::Visible);
    style.box_sizing = static_cast<int32_t>(BoxSizing::BorderBox);
    style.direction = static_cast<int32_t>(StyleDirection::Ltr);
    style.width = width;
    style.height = height;
    style.flex_grow = flexGrow;
    style.flex_shrink = flexShrink;
    try {
        d->engine->engine_set_style(node, style);
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

bool Engine::compute(qreal availableWidth, qreal availableHeight)
{
    return compute(AvailableSpace::definite(availableWidth),
                   AvailableSpace::definite(availableHeight));
}

bool Engine::layout(quint32 node, Layout& out) const
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
