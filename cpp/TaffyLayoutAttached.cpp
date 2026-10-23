#include "taffy/TaffyLayoutAttached.h"
#include "taffy/TaffyEngine.h"

#include <limits>

namespace {

bool isAuto(qreal v)
{
    return v != v; // NaN
}

taffy::LengthValue lenOrAuto(qreal v)
{
    return isAuto(v) ? taffy::LengthValue::autoValue() : taffy::LengthValue::length(v);
}

taffy::LengthValue lenOrZero(qreal v)
{
    return isAuto(v) ? taffy::LengthValue::zero() : taffy::LengthValue::length(v);
}

} // namespace

TaffyLayoutAttached::TaffyLayoutAttached(QObject* parent)
    : QObject(parent)
{
}

qreal TaffyLayoutAttached::width() const
{
    return m_width;
}
void TaffyLayoutAttached::setWidth(qreal v)
{
    m_width = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::height() const
{
    return m_height;
}
void TaffyLayoutAttached::setHeight(qreal v)
{
    m_height = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::minWidth() const
{
    return m_minWidth;
}
void TaffyLayoutAttached::setMinWidth(qreal v)
{
    m_minWidth = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::minHeight() const
{
    return m_minHeight;
}
void TaffyLayoutAttached::setMinHeight(qreal v)
{
    m_minHeight = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::maxWidth() const
{
    return m_maxWidth;
}
void TaffyLayoutAttached::setMaxWidth(qreal v)
{
    m_maxWidth = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::maxHeight() const
{
    return m_maxHeight;
}
void TaffyLayoutAttached::setMaxHeight(qreal v)
{
    m_maxHeight = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::aspectRatio() const
{
    return m_aspectRatio;
}
void TaffyLayoutAttached::setAspectRatio(qreal v)
{
    m_aspectRatio = v;
    emit itemStyleChanged();
}

qreal TaffyLayoutAttached::flexBasis() const
{
    return m_flexBasis;
}
void TaffyLayoutAttached::setFlexBasis(qreal v)
{
    m_flexBasis = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::flexGrow() const
{
    return m_flexGrow;
}
void TaffyLayoutAttached::setFlexGrow(qreal v)
{
    m_flexGrow = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::flexShrink() const
{
    return m_flexShrink;
}
void TaffyLayoutAttached::setFlexShrink(qreal v)
{
    m_flexShrink = v;
    emit itemStyleChanged();
}

bool TaffyLayoutAttached::hasAlignSelf() const
{
    return m_hasAlignSelf;
}
void TaffyLayoutAttached::setHasAlignSelf(bool v)
{
    m_hasAlignSelf = v;
    emit itemStyleChanged();
}
AlignItems TaffyLayoutAttached::alignSelf() const
{
    return m_alignSelf;
}
void TaffyLayoutAttached::setAlignSelf(AlignItems v)
{
    m_alignSelf = v;
    emit itemStyleChanged();
}
bool TaffyLayoutAttached::hasJustifySelf() const
{
    return m_hasJustifySelf;
}
void TaffyLayoutAttached::setHasJustifySelf(bool v)
{
    m_hasJustifySelf = v;
    emit itemStyleChanged();
}
AlignItems TaffyLayoutAttached::justifySelf() const
{
    return m_justifySelf;
}
void TaffyLayoutAttached::setJustifySelf(AlignItems v)
{
    m_justifySelf = v;
    emit itemStyleChanged();
}

qreal TaffyLayoutAttached::marginTop() const
{
    return m_marginTop;
}
void TaffyLayoutAttached::setMarginTop(qreal v)
{
    m_marginTop = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::marginRight() const
{
    return m_marginRight;
}
void TaffyLayoutAttached::setMarginRight(qreal v)
{
    m_marginRight = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::marginBottom() const
{
    return m_marginBottom;
}
void TaffyLayoutAttached::setMarginBottom(qreal v)
{
    m_marginBottom = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::marginLeft() const
{
    return m_marginLeft;
}
void TaffyLayoutAttached::setMarginLeft(qreal v)
{
    m_marginLeft = v;
    emit itemStyleChanged();
}

qreal TaffyLayoutAttached::paddingTop() const
{
    return m_paddingTop;
}
void TaffyLayoutAttached::setPaddingTop(qreal v)
{
    m_paddingTop = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::paddingRight() const
{
    return m_paddingRight;
}
void TaffyLayoutAttached::setPaddingRight(qreal v)
{
    m_paddingRight = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::paddingBottom() const
{
    return m_paddingBottom;
}
void TaffyLayoutAttached::setPaddingBottom(qreal v)
{
    m_paddingBottom = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::paddingLeft() const
{
    return m_paddingLeft;
}
void TaffyLayoutAttached::setPaddingLeft(qreal v)
{
    m_paddingLeft = v;
    emit itemStyleChanged();
}

qreal TaffyLayoutAttached::borderTop() const
{
    return m_borderTop;
}
void TaffyLayoutAttached::setBorderTop(qreal v)
{
    m_borderTop = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::borderRight() const
{
    return m_borderRight;
}
void TaffyLayoutAttached::setBorderRight(qreal v)
{
    m_borderRight = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::borderBottom() const
{
    return m_borderBottom;
}
void TaffyLayoutAttached::setBorderBottom(qreal v)
{
    m_borderBottom = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::borderLeft() const
{
    return m_borderLeft;
}
void TaffyLayoutAttached::setBorderLeft(qreal v)
{
    m_borderLeft = v;
    emit itemStyleChanged();
}

Position TaffyLayoutAttached::position() const
{
    return m_position;
}
void TaffyLayoutAttached::setPosition(Position v)
{
    m_position = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::insetTop() const
{
    return m_insetTop;
}
void TaffyLayoutAttached::setInsetTop(qreal v)
{
    m_insetTop = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::insetRight() const
{
    return m_insetRight;
}
void TaffyLayoutAttached::setInsetRight(qreal v)
{
    m_insetRight = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::insetBottom() const
{
    return m_insetBottom;
}
void TaffyLayoutAttached::setInsetBottom(qreal v)
{
    m_insetBottom = v;
    emit itemStyleChanged();
}
qreal TaffyLayoutAttached::insetLeft() const
{
    return m_insetLeft;
}
void TaffyLayoutAttached::setInsetLeft(qreal v)
{
    m_insetLeft = v;
    emit itemStyleChanged();
}
