#pragma once

#include <memory>

#include <taffy/TaffyTypes.h>

namespace taffy {

class EngineImpl;

/** Geometry in abstract layout units (usually pixels). */
struct Rect
{
    double left = 0.0;
    double right = 0.0;
    double top = 0.0;
    double bottom = 0.0;
};

struct Size
{
    double width = 0.0;
    double height = 0.0;
};

struct Point
{
    double x = 0.0;
    double y = 0.0;
};

/** Mirrors taffy::Layout (without feature-gated fields). */
struct Layout
{
    quint32 order = 0;
    Point location;
    Size size;
    Rect border;
    Rect padding;
    Rect margin;
    Size scrollbarSize;
};

/**
 * C++ facade over the Rust taffy engine (cxx).
 */
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    quint32 addNode(const TaffyTypes* styleDefaults = nullptr);
    bool addChild(quint32 parent, quint32 child);
    bool setStyle(quint32 node, TaffyTypes::Display display,
                  TaffyTypes::FlexDirection flexDirection,
                  TaffyTypes::FlexWrap flexWrap,
                  TaffyTypes::AlignItemsKeyword alignItems,
                  TaffyTypes::AlignContentKeyword alignContent,
                  TaffyTypes::AlignContentKeyword justifyContent,
                  qreal width, qreal height,
                  qreal flexGrow = 0.0, qreal flexShrink = 1.0);
    bool compute(qreal availableWidth, qreal availableHeight);
    bool layout(quint32 node, Layout& out) const;
    void clear();

private:
    std::unique_ptr<EngineImpl> d;
};

} // namespace taffy
