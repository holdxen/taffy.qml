#pragma once

#include <QObject>

/**
 * Qt/QML-facing style enums. Values match the cxx bridge in rust/src/lib.rs
 * so they can be cast when calling into the Rust engine.
 */
class TaffyTypes : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    enum Display {
        DisplayBlock,
        DisplayFlowRoot,
        DisplayFlex,
        DisplayGrid,
        DisplayNone,
    };
    Q_ENUM(Display)

    enum Position {
        PositionStatic,
        PositionRelative,
        PositionAbsolute,
        PositionFixed,
        PositionSticky,
    };
    Q_ENUM(Position)

    enum FlexDirection {
        FlexDirectionRow,
        FlexDirectionColumn,
        FlexDirectionRowReverse,
        FlexDirectionColumnReverse,
    };
    Q_ENUM(FlexDirection)

    enum FlexWrap {
        FlexWrapNoWrap,
        FlexWrapWrap,
        FlexWrapWrapReverse,
    };
    Q_ENUM(FlexWrap)

    enum AlignItemsKeyword {
        AlignItemsKeywordNormal,
        AlignItemsKeywordStart,
        AlignItemsKeywordEnd,
        AlignItemsKeywordFlexStart,
        AlignItemsKeywordFlexEnd,
        AlignItemsKeywordSelfStart,
        AlignItemsKeywordSelfEnd,
        AlignItemsKeywordCenter,
        AlignItemsKeywordBaseline,
        AlignItemsKeywordStretch,
    };
    Q_ENUM(AlignItemsKeyword)

    enum AlignContentKeyword {
        AlignContentKeywordNormal,
        AlignContentKeywordStart,
        AlignContentKeywordEnd,
        AlignContentKeywordFlexStart,
        AlignContentKeywordFlexEnd,
        AlignContentKeywordCenter,
        AlignContentKeywordStretch,
        AlignContentKeywordSpaceBetween,
        AlignContentKeywordSpaceEvenly,
        AlignContentKeywordSpaceAround,
    };
    Q_ENUM(AlignContentKeyword)

    enum Overflow {
        OverflowVisible,
        OverflowClip,
        OverflowHidden,
        OverflowScroll,
    };
    Q_ENUM(Overflow)

    enum BoxSizing {
        BoxSizingBorderBox,
        BoxSizingContentBox,
    };
    Q_ENUM(BoxSizing)

    enum Direction {
        DirectionLtr,
        DirectionRtl,
    };
    Q_ENUM(Direction)
};
