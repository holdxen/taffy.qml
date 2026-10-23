#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <vector>

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

/** length / percentage / auto / keyword value. kind matches LengthKind qenum. */
struct LengthValue
{
    qint32 kind = 0; // 0 Auto, 1 Length, 2 Percent, 8 Zero
    double value = 0.0;

    static LengthValue autoValue()
    {
        return {};
    }
    static LengthValue length(double v)
    {
        LengthValue s;
        s.kind = 1;
        s.value = v;
        return s;
    }
    static LengthValue percent(double v)
    {
        LengthValue s;
        s.kind = 2;
        s.value = v;
        return s;
    }
    static LengthValue zero()
    {
        LengthValue s;
        s.kind = 8;
        return s;
    }
};

struct LengthSize
{
    LengthValue width;
    LengthValue height;
};

struct LengthRect
{
    LengthValue left;
    LengthValue right;
    LengthValue top;
    LengthValue bottom;
};

struct Placement
{
    qint32 kind = 0; // 0 Auto, 1 Line, 2 Span
    double value = 0.0;
};

struct LinePlacement
{
    Placement start;
    Placement end;
};

struct TrackSize
{
    qint32 minKind = 0;
    double minValue = 0.0;
    qint32 maxKind = 0;
    double maxValue = 0.0;
};

/**
 * Full taffy::Style surface (mirrors rust ffi::NodeStyle).
 * Enum fields use integer codes matching TaffyTypes Q_ENUMs.
 */
struct Style
{
    qint32 display = 2; // Display::Flex
    bool itemIsTable = false;
    bool itemIsReplaced = false;
    bool itemIsCompressibleReplaced = false;
    qint32 boxSizing = 0; // BoxSizing::BorderBox
    qint32 direction = 0; // StyleDirection::Ltr
    qint32 overflowX = 0; // Overflow::Visible
    qint32 overflowY = 0;
    double scrollbarWidth = 0.0;
    quint32 contain = 0;
    qint32 cssFloat = 0; // CssFloat::None
    qint32 clear = 0;
    qint32 position = 0;
    LengthRect inset;
    LengthSize size;
    LengthSize minSize;
    LengthSize maxSize;
    double aspectRatio = std::numeric_limits<double>::quiet_NaN();
    LengthRect margin;
    LengthRect padding;
    LengthRect border;
    qint32 alignItems = 0;
    bool hasAlignSelf = false;
    qint32 alignSelf = 0;
    qint32 justifyItems = 0;
    bool hasJustifySelf = false;
    qint32 justifySelf = 0;
    qint32 alignContent = 0;
    qint32 justifyContent = 0;
    LengthSize gap;
    qint32 textAlign = 0;
    qint32 flexDirection = 0;
    qint32 flexWrap = 0;
    quint32 flexLineCount = 1;
    LengthValue flexBasis;
    double flexGrow = 0.0;
    double flexShrink = 1.0;
    qint32 gridAutoFlow = 0;
    LinePlacement gridRow;
    LinePlacement gridColumn;
};

class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    quint32 addNode();
    quint32 addNode(const Style& style);
    bool addChild(quint32 parent, quint32 child);
    bool setStyle(quint32 node, const Style& style);
    bool compute(const AvailableSpace& width, const AvailableSpace& height);
    bool compute(qreal availableWidth, qreal availableHeight);
    bool layout(quint32 node, Layout& out) const;
    void clear();

private:
    std::unique_ptr<EngineImpl> d;
};

} // namespace taffy
