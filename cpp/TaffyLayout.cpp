#include "taffy/TaffyLayout.h"

#include <cmath>

class TaffyLayoutPrivate : public QQuickLayoutPrivate
{
public:
    TaffyLayoutPrivate() = default;
};

TaffyLayout::TaffyLayout(QQuickItem* parent)
    : QQuickLayout(*new TaffyLayoutPrivate, parent)
{
}

TaffyLayout::~TaffyLayout() = default;

TaffyLayoutAttached* TaffyLayout::qmlAttachedProperties(QObject* object)
{
    return new TaffyLayoutAttached(object);
}

// --- container-side properties ---

Display TaffyLayout::display() const
{
    return m_display;
}
void TaffyLayout::setDisplay(Display v)
{
    if (m_display == v)
        return;
    m_display = v;
    emit styleChanged();
    invalidate();
}
FlexDirection TaffyLayout::flexDirection() const
{
    return m_flexDirection;
}
void TaffyLayout::setFlexDirection(FlexDirection v)
{
    if (m_flexDirection == v)
        return;
    m_flexDirection = v;
    emit styleChanged();
    invalidate();
}
FlexWrap TaffyLayout::flexWrap() const
{
    return m_flexWrap;
}
void TaffyLayout::setFlexWrap(FlexWrap v)
{
    if (m_flexWrap == v)
        return;
    m_flexWrap = v;
    emit styleChanged();
    invalidate();
}
qint32 TaffyLayout::flexLineCount() const
{
    return m_flexLineCount;
}
void TaffyLayout::setFlexLineCount(qint32 v)
{
    if (m_flexLineCount == v)
        return;
    m_flexLineCount = v;
    emit styleChanged();
    invalidate();
}
AlignItems TaffyLayout::alignItems() const
{
    return m_alignItems;
}
void TaffyLayout::setAlignItems(AlignItems v)
{
    if (m_alignItems == v)
        return;
    m_alignItems = v;
    emit styleChanged();
    invalidate();
}
AlignContent TaffyLayout::alignContent() const
{
    return m_alignContent;
}
void TaffyLayout::setAlignContent(AlignContent v)
{
    if (m_alignContent == v)
        return;
    m_alignContent = v;
    emit styleChanged();
    invalidate();
}
AlignContent TaffyLayout::justifyContent() const
{
    return m_justifyContent;
}
void TaffyLayout::setJustifyContent(AlignContent v)
{
    if (m_justifyContent == v)
        return;
    m_justifyContent = v;
    emit styleChanged();
    invalidate();
}
qreal TaffyLayout::gapWidth() const
{
    return m_gapWidth;
}
void TaffyLayout::setGapWidth(qreal v)
{
    if (qFuzzyCompare(m_gapWidth, v))
        return;
    m_gapWidth = v;
    emit styleChanged();
    invalidate();
}
qreal TaffyLayout::gapHeight() const
{
    return m_gapHeight;
}
void TaffyLayout::setGapHeight(qreal v)
{
    if (qFuzzyCompare(m_gapHeight, v))
        return;
    m_gapHeight = v;
    emit styleChanged();
    invalidate();
}
GridAutoFlow TaffyLayout::gridAutoFlow() const
{
    return m_gridAutoFlow;
}
void TaffyLayout::setGridAutoFlow(GridAutoFlow v)
{
    if (m_gridAutoFlow == v)
        return;
    m_gridAutoFlow = v;
    emit styleChanged();
    invalidate();
}
qreal TaffyLayout::paddingTop() const
{
    return m_paddingTop;
}
void TaffyLayout::setPaddingTop(qreal v)
{
    if (qFuzzyCompare(m_paddingTop, v))
        return;
    m_paddingTop = v;
    emit styleChanged();
    invalidate();
}
qreal TaffyLayout::paddingRight() const
{
    return m_paddingRight;
}
void TaffyLayout::setPaddingRight(qreal v)
{
    if (qFuzzyCompare(m_paddingRight, v))
        return;
    m_paddingRight = v;
    emit styleChanged();
    invalidate();
}
qreal TaffyLayout::paddingBottom() const
{
    return m_paddingBottom;
}
void TaffyLayout::setPaddingBottom(qreal v)
{
    if (qFuzzyCompare(m_paddingBottom, v))
        return;
    m_paddingBottom = v;
    emit styleChanged();
    invalidate();
}
qreal TaffyLayout::paddingLeft() const
{
    return m_paddingLeft;
}
void TaffyLayout::setPaddingLeft(qreal v)
{
    if (qFuzzyCompare(m_paddingLeft, v))
        return;
    m_paddingLeft = v;
    emit styleChanged();
    invalidate();
}
StyleDirection TaffyLayout::direction() const
{
    return m_direction;
}
void TaffyLayout::setDirection(StyleDirection v)
{
    if (m_direction == v)
        return;
    m_direction = v;
    emit styleChanged();
    invalidate();
}

// --- style builders ---

namespace {

taffy::LengthValue lenOrAuto(qreal v)
{
    return (v != v) ? taffy::LengthValue::autoValue() : taffy::LengthValue::length(v);
}

taffy::LengthValue lenOrZero(qreal v)
{
    return (v != v) ? taffy::LengthValue::zero() : taffy::LengthValue::length(v);
}

} // namespace

taffy::Style TaffyLayout::containerStyle(const QSizeF& size) const
{
    taffy::Style s;
    s.display = static_cast<qint32>(m_display);
    s.flexDirection = static_cast<qint32>(m_flexDirection);
    s.flexWrap = static_cast<qint32>(m_flexWrap);
    s.flexLineCount = quint32(std::max(1, m_flexLineCount));
    s.alignItems = static_cast<qint32>(m_alignItems);
    s.alignContent = static_cast<qint32>(m_alignContent);
    s.justifyContent = static_cast<qint32>(m_justifyContent);
    s.gap.width = taffy::LengthValue::length(m_gapWidth);
    s.gap.height = taffy::LengthValue::length(m_gapHeight);
    s.gridAutoFlow = static_cast<qint32>(m_gridAutoFlow);
    s.direction = static_cast<qint32>(m_direction);
    s.padding.top = taffy::LengthValue::length(m_paddingTop);
    s.padding.right = taffy::LengthValue::length(m_paddingRight);
    s.padding.bottom = taffy::LengthValue::length(m_paddingBottom);
    s.padding.left = taffy::LengthValue::length(m_paddingLeft);
    if (size.width() > 0 && size.height() > 0) {
        s.size.width = taffy::LengthValue::length(size.width());
        s.size.height = taffy::LengthValue::length(size.height());
    }
    return s;
}

taffy::Style TaffyLayout::itemStyle(QQuickItem* child) const
{
    taffy::Style s;
    s.display = static_cast<qint32>(Display::Flex);

    auto* attached = qobject_cast<TaffyLayoutAttached*>(qmlAttachedPropertiesObject<TaffyLayout>(child, false));
    if (!attached) {
        // fall back to item geometry
        qreal w = child->width() > 0 ? child->width() : (child->implicitWidth() > 0 ? child->implicitWidth() : 100);
        qreal h = child->height() > 0 ? child->height() : (child->implicitHeight() > 0 ? child->implicitHeight() : 40);
        s.size.width = taffy::LengthValue::length(w);
        s.size.height = taffy::LengthValue::length(h);
        return s;
    }

    qreal w = attached->width();
    qreal h = attached->height();
    if (w != w) {
        w = child->width() > 0 ? child->width() : (child->implicitWidth() > 0 ? child->implicitWidth() : 100);
        s.size.width = taffy::LengthValue::length(w);
    } else {
        s.size.width = taffy::LengthValue::length(w);
    }
    if (h != h) {
        h = child->height() > 0 ? child->height() : (child->implicitHeight() > 0 ? child->implicitHeight() : 40);
        s.size.height = taffy::LengthValue::length(h);
    } else {
        s.size.height = taffy::LengthValue::length(h);
    }

    s.minSize.width = lenOrAuto(attached->minWidth());
    s.minSize.height = lenOrAuto(attached->minHeight());
    s.maxSize.width = lenOrAuto(attached->maxWidth());
    s.maxSize.height = lenOrAuto(attached->maxHeight());
    // NaN means "none" on the Rust side
    s.aspectRatio = attached->aspectRatio();

    s.flexBasis = lenOrAuto(attached->flexBasis());
    s.flexGrow = attached->flexGrow();
    s.flexShrink = attached->flexShrink();

    s.hasAlignSelf = attached->hasAlignSelf();
    s.alignSelf = static_cast<qint32>(attached->alignSelf());
    s.hasJustifySelf = attached->hasJustifySelf();
    s.justifySelf = static_cast<qint32>(attached->justifySelf());

    s.margin.top = lenOrZero(attached->marginTop());
    s.margin.right = lenOrZero(attached->marginRight());
    s.margin.bottom = lenOrZero(attached->marginBottom());
    s.margin.left = lenOrZero(attached->marginLeft());

    s.padding.top = lenOrZero(attached->paddingTop());
    s.padding.right = lenOrZero(attached->paddingRight());
    s.padding.bottom = lenOrZero(attached->paddingBottom());
    s.padding.left = lenOrZero(attached->paddingLeft());

    s.border.top = lenOrZero(attached->borderTop());
    s.border.right = lenOrZero(attached->borderRight());
    s.border.bottom = lenOrZero(attached->borderBottom());
    s.border.left = lenOrZero(attached->borderLeft());

    s.position = static_cast<qint32>(attached->position());
    s.inset.top = lenOrAuto(attached->insetTop());
    s.inset.right = lenOrAuto(attached->insetRight());
    s.inset.bottom = lenOrAuto(attached->insetBottom());
    s.inset.left = lenOrAuto(attached->insetLeft());

    return s;
}

// --- QQuickLayout ---

void TaffyLayout::updateLayoutItems()
{
    invalidate();
}

void TaffyLayout::itemChange(ItemChange change, const ItemChangeData& value)
{
    QQuickLayout::itemChange(change, value);
    if (change == ItemChildAddedChange || change == ItemChildRemovedChange
        || change == ItemVisibleHasChanged) {
        invalidate();
    }
}

QQuickItem* TaffyLayout::itemAt(int index) const
{
    const auto kids = childItems();
    int visible = 0;
    for (QQuickItem* child : kids) {
        if (!shouldIgnoreItem(child)) {
            if (visible == index) {
                return child;
            }
            ++visible;
        }
    }
    return nullptr;
}

int TaffyLayout::itemCount() const
{
    int n = 0;
    for (QQuickItem* child : childItems()) {
        if (!shouldIgnoreItem(child)) {
            ++n;
        }
    }
    return n;
}

void TaffyLayout::setAlignment(QQuickItem* item, Qt::Alignment align)
{
    Q_UNUSED(item);
    Q_UNUSED(align);
}

void TaffyLayout::setStretchFactor(QQuickItem* item, int stretchFactor, Qt::Orientation orient)
{
    Q_UNUSED(item);
    Q_UNUSED(stretchFactor);
    Q_UNUSED(orient);
}

QSizeF TaffyLayout::sizeHint(Qt::SizeHint whichSizeHint) const
{
    return computeSizeHint(whichSizeHint);
}

QSizeF TaffyLayout::computeSizeHint(Qt::SizeHint which) const
{
    m_engine.clear();
    const quint32 root = m_engine.addNode(containerStyle(QSizeF()));
    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        QQuickItem* child = itemAt(i);
        const quint32 node = m_engine.addNode(itemStyle(child));
        m_engine.addChild(root, node);
    }

    switch (which) {
    case Qt::MinimumSize:
        m_engine.compute(taffy::AvailableSpace::minContent(), taffy::AvailableSpace::minContent());
        break;
    case Qt::MaximumSize:
        m_engine.compute(taffy::AvailableSpace::maxContent(), taffy::AvailableSpace::maxContent());
        break;
    default:
        m_engine.compute(taffy::AvailableSpace::maxContent(), taffy::AvailableSpace::minContent());
        break;
    }

    taffy::Layout rootLayout;
    if (!m_engine.layout(root, rootLayout)) {
        return QSizeF(0, 0);
    }
    return QSizeF(rootLayout.size.width, rootLayout.size.height);
}

void TaffyLayout::rearrange(const QSizeF& size)
{
    m_engine.clear();
    const quint32 root = m_engine.addNode(containerStyle(size));

    QList<QQuickItem*> layoutItems;
    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        QQuickItem* child = itemAt(i);
        layoutItems.append(child);
        const quint32 node = m_engine.addNode(itemStyle(child));
        m_engine.addChild(root, node);
    }

    if (!m_engine.compute(size.width(), size.height())) {
        return;
    }

    for (int i = 0; i < layoutItems.size(); ++i) {
        taffy::Layout l;
        if (m_engine.layout(quint32(i + 1), l)) {
            layoutItems[i]->setX(l.location.x);
            layoutItems[i]->setY(l.location.y);
            layoutItems[i]->setSize(QSizeF(l.size.width, l.size.height));
        }
    }
}
