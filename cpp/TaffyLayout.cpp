#include "taffy/TaffyLayout.h"

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

Display TaffyLayout::display() const
{
    return m_display;
}

void TaffyLayout::setDisplay(Display display)
{
    if (m_display == display) {
        return;
    }
    m_display = display;
    emit displayChanged();
    invalidate();
}

FlexDirection TaffyLayout::flexDirection() const
{
    return m_flexDirection;
}

void TaffyLayout::setFlexDirection(FlexDirection direction)
{
    if (m_flexDirection == direction) {
        return;
    }
    m_flexDirection = direction;
    emit flexDirectionChanged();
    invalidate();
}

FlexWrap TaffyLayout::flexWrap() const
{
    return m_flexWrap;
}

void TaffyLayout::setFlexWrap(FlexWrap wrap)
{
    if (m_flexWrap == wrap) {
        return;
    }
    m_flexWrap = wrap;
    emit flexWrapChanged();
    invalidate();
}

AlignItems TaffyLayout::alignItems() const
{
    return m_alignItems;
}

void TaffyLayout::setAlignItems(AlignItems align)
{
    if (m_alignItems == align) {
        return;
    }
    m_alignItems = align;
    emit alignItemsChanged();
    invalidate();
}

AlignContent TaffyLayout::alignContent() const
{
    return m_alignContent;
}

void TaffyLayout::setAlignContent(AlignContent align)
{
    if (m_alignContent == align) {
        return;
    }
    m_alignContent = align;
    emit alignContentChanged();
    invalidate();
}

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
    // Alignment is applied via the taffy style; per-item alignment can be wired later.
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
    const quint32 root = m_engine.addNode();
    m_engine.setStyle(root, m_display, m_flexDirection, m_flexWrap,
                      m_alignItems, m_alignContent, m_alignContent,
                      qQNaN(), qQNaN());

    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        QQuickItem* child = itemAt(i);
        const quint32 node = m_engine.addNode();
        qreal w = child->width();
        qreal h = child->height();
        if (w <= 0) {
            w = child->implicitWidth() > 0 ? child->implicitWidth() : 100;
        }
        if (h <= 0) {
            h = child->implicitHeight() > 0 ? child->implicitHeight() : 40;
        }
        m_engine.setStyle(node, Display::Flex, FlexDirection::Row, FlexWrap::NoWrap,
                          AlignItems::Normal, AlignContent::Normal,
                          AlignContent::Normal, w, h);
        m_engine.addChild(root, node);
    }

    switch (which) {
    case Qt::MinimumSize:
        m_engine.compute(taffy::AvailableSpace::minContent(), taffy::AvailableSpace::minContent());
        break;
    case Qt::MaximumSize:
        m_engine.compute(taffy::AvailableSpace::maxContent(), taffy::AvailableSpace::maxContent());
        break;
    case Qt::PreferredSize:
    case Qt::MinimumDescent:
    case Qt::NSizeHints:
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
    relayout(size);
}

void TaffyLayout::relayout(const QSizeF& size)
{
    m_engine.clear();
    const quint32 root = m_engine.addNode();
    m_engine.setStyle(root, m_display, m_flexDirection, m_flexWrap,
                      m_alignItems, m_alignContent, m_alignContent,
                      size.width(), size.height());

    QList<QQuickItem*> layoutItems;
    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        QQuickItem* child = itemAt(i);
        layoutItems.append(child);
        const quint32 node = m_engine.addNode();
        qreal w = child->width();
        qreal h = child->height();
        if (w <= 0) {
            w = child->implicitWidth() > 0 ? child->implicitWidth() : 100;
        }
        if (h <= 0) {
            h = child->implicitHeight() > 0 ? child->implicitHeight() : 40;
        }
        m_engine.setStyle(node, Display::Flex, FlexDirection::Row, FlexWrap::NoWrap,
                          AlignItems::Normal, AlignContent::Normal,
                          AlignContent::Normal, w, h);
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
