#pragma once

#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <vector>

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
    std::uint32_t order = 0;
    Point location;
    Size size;
    Rect border;
    Rect padding;
    Rect margin;
    Size scrollbarSize;
};

/**
 * Available space on one axis, CSS-like:
 *   "100px" / "100"  — definite
 *   "min-content"    — min-content
 *   "max-content"    — max-content
 */
struct AvailableSpace
{
    std::string css = "auto";

    static AvailableSpace definite(double v)
    {
        AvailableSpace s;
        s.css = std::to_string(v) + "px";
        return s;
    }
    static AvailableSpace minContent()
    {
        AvailableSpace s;
        s.css = "min-content";
        return s;
    }
    static AvailableSpace maxContent()
    {
        AvailableSpace s;
        s.css = "max-content";
        return s;
    }
    static AvailableSpace parse(const std::string& css)
    {
        AvailableSpace s;
        s.css = css;
        return s;
    }
};

/** CSS-like length string: "auto", "100px", "50%", "min-content", "0" */
using CssLength = std::string;

struct LengthSize
{
    CssLength width = "auto";
    CssLength height = "auto";
};

struct LengthRect
{
    CssLength left = "0";
    CssLength right = "0";
    CssLength top = "0";
    CssLength bottom = "0";
};

struct LinePlacement
{
    std::string start = "auto";
    std::string end = "auto";
};


/**
 * Full style surface; keyword fields are CSS strings.
 * Examples: display="flex", flex_direction="row", align_items="center",
 *           width="100px", flex_basis="auto"
 */
struct Style
{
    std::string display = "flex";
    bool itemIsTable = false;
    bool itemIsReplaced = false;
    bool itemIsCompressibleReplaced = false;
    std::string boxSizing = "border-box";
    std::string direction = "ltr";
    std::string overflowX = "visible";
    std::string overflowY = "visible";
    double scrollbarWidth = 0.0;
    std::uint32_t contain = 0;
    std::string cssFloat = "none";
    std::string clear = "none";
    std::string position = "static";
    LengthRect inset;
    LengthSize size;
    LengthSize minSize;
    LengthSize maxSize;
    double aspectRatio = std::numeric_limits<double>::quiet_NaN();
    LengthRect margin;
    LengthRect padding;
    LengthRect border;
    std::string alignItems = "normal";
    std::string alignSelf; // empty or "auto" = unset
    std::string justifyItems = "normal";
    std::string justifySelf; // empty or "auto" = unset
    std::string alignContent = "normal";
    std::string justifyContent = "normal";
    LengthSize gap;
    std::string textAlign = "auto";
    std::string flexDirection = "row";
    std::string flexWrap = "nowrap";
    std::uint32_t flexLineCount = 1;
    std::string flexBasis = "auto";
    double flexGrow = 0.0;
    double flexShrink = 1.0;
    std::string gridAutoFlow = "row";
    // CSS track lists
    std::string gridTemplateRows;
    std::string gridTemplateColumns;
    std::string gridAutoRows;
    std::string gridAutoColumns;
    // CSS: "\"header header\" \"main sidebar\"" or "header header / main sidebar"
    std::string gridTemplateAreas;
    // one entry per track: "start end"
    std::vector<std::string> gridTemplateRowNames;
    std::vector<std::string> gridTemplateColumnNames;
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

    std::uint32_t addNode();
    std::uint32_t addNode(const Style& style);
    bool addChild(std::uint32_t parent, std::uint32_t child);
    bool setStyle(std::uint32_t node, const Style& style);
    bool compute(const AvailableSpace& width, const AvailableSpace& height);
    bool compute(double availableWidth, double availableHeight);
    bool layout(std::uint32_t node, Layout& out) const;
    void clear();

private:
    std::unique_ptr<EngineImpl> d;
};

} // namespace taffy
