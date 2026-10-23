#pragma once

#include <memory>

#include <taffy/TaffyTypes.h>

namespace taffy {

class EngineImpl;

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

struct AvailableSpace
{
    enum class Kind {
        Definite,
        MinContent,
        MaxContent,
    };

    Kind kind = Kind::Definite;
    double value = 0.0;

    static AvailableSpace definite(double v)
    {
        AvailableSpace s;
        s.kind = Kind::Definite;
        s.value = v;
        return s;
    }
    static AvailableSpace minContent()
    {
        AvailableSpace s;
        s.kind = Kind::MinContent;
        return s;
    }
    static AvailableSpace maxContent()
    {
        AvailableSpace s;
        s.kind = Kind::MaxContent;
        return s;
    }
};

/**
 * C++ facade over the Rust taffy engine.
 * Enum types come from cxx-qt generated Q_ENUMs (TaffyTypes).
 */
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    quint32 addNode();
    bool addChild(quint32 parent, quint32 child);
    bool setStyle(quint32 node,
                  Display display,
                  FlexDirection flexDirection,
                  FlexWrap flexWrap,
                  AlignItemsKeyword alignItems,
                  AlignContentKeyword alignContent,
                  AlignContentKeyword justifyContent,
                  qreal width, qreal height,
                  qreal flexGrow = 0.0, qreal flexShrink = 1.0);
    bool compute(const AvailableSpace& width, const AvailableSpace& height);
    bool compute(qreal availableWidth, qreal availableHeight);
    bool layout(quint32 node, Layout& out) const;
    void clear();

private:
    std::unique_ptr<EngineImpl> d;
};

} // namespace taffy
