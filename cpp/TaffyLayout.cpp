#include "taffy/TaffyLayout.h"

#include <algorithm>
#include <cmath>

class TaffyLayoutPrivate : public QQuickLayoutPrivate
{
public:
    TaffyLayoutPrivate() = default;
};

namespace {

std::string toStd(const QString& s)
{
    return s.toStdString();
}

} // namespace

TaffyLayout::TaffyLayout(QQuickItem* parent)
    : QQuickLayout(*new TaffyLayoutPrivate, parent)
{
}

TaffyLayout::~TaffyLayout() = default;

TaffyLayoutAttached* TaffyLayout::qmlAttachedProperties(QObject* object)
{
    return new TaffyLayoutAttached(object);
}

QString TaffyLayout::display() const
{
    return m_display;
}
void TaffyLayout::setDisplay(const QString& v)
{
    if (m_display == v)
        return;
    m_display = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::flexDirection() const
{
    return m_flexDirection;
}
void TaffyLayout::setFlexDirection(const QString& v)
{
    if (m_flexDirection == v)
        return;
    m_flexDirection = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::flexWrap() const
{
    return m_flexWrap;
}
void TaffyLayout::setFlexWrap(const QString& v)
{
    if (m_flexWrap == v)
        return;
    m_flexWrap = v;
    emit styleChanged();
    invalidate();
}
int TaffyLayout::flexLineCount() const
{
    return m_flexLineCount;
}
void TaffyLayout::setFlexLineCount(int v)
{
    if (m_flexLineCount == v)
        return;
    m_flexLineCount = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::alignItems() const
{
    return m_alignItems;
}
void TaffyLayout::setAlignItems(const QString& v)
{
    if (m_alignItems == v)
        return;
    m_alignItems = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::alignContent() const
{
    return m_alignContent;
}
void TaffyLayout::setAlignContent(const QString& v)
{
    if (m_alignContent == v)
        return;
    m_alignContent = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::justifyContent() const
{
    return m_justifyContent;
}
void TaffyLayout::setJustifyContent(const QString& v)
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
QString TaffyLayout::gridAutoFlow() const
{
    return m_gridAutoFlow;
}
void TaffyLayout::setGridAutoFlow(const QString& v)
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
QString TaffyLayout::direction() const
{
    return m_direction;
}
void TaffyLayout::setDirection(const QString& v)
{
    if (m_direction == v)
        return;
    m_direction = v;
    emit styleChanged();
    invalidate();
}

taffy::Style TaffyLayout::containerStyle(const QSizeF& size) const
{
    taffy::Style s;
    s.display = toStd(m_display);
    s.flexDirection = toStd(m_flexDirection);
    s.flexWrap = toStd(m_flexWrap);
    s.flexLineCount = unsigned(std::max(1, m_flexLineCount));
    s.alignItems = toStd(m_alignItems);
    s.alignContent = toStd(m_alignContent);
    s.justifyContent = toStd(m_justifyContent);
    s.gap.width = std::to_string(m_gapWidth) + "px";
    s.gap.height = std::to_string(m_gapHeight) + "px";
    s.gridAutoFlow = toStd(m_gridAutoFlow);
    s.direction = toStd(m_direction);
    s.padding.top = std::to_string(m_paddingTop) + "px";
    s.padding.right = std::to_string(m_paddingRight) + "px";
    s.padding.bottom = std::to_string(m_paddingBottom) + "px";
    s.padding.left = std::to_string(m_paddingLeft) + "px";
    if (size.width() > 0 && size.height() > 0) {
        s.size.width = std::to_string(size.width()) + "px";
        s.size.height = std::to_string(size.height()) + "px";
    }
    return s;
}

taffy::Style TaffyLayout::itemStyle(QQuickItem* child) const
{
    taffy::Style s;
    s.display = "flex";

    auto* attached = qobject_cast<TaffyLayoutAttached*>(qmlAttachedPropertiesObject<TaffyLayout>(child, false));

    qreal fallbackW = child->width() > 0 ? child->width() : (child->implicitWidth() > 0 ? child->implicitWidth() : 100);
    qreal fallbackH = child->height() > 0 ? child->height() : (child->implicitHeight() > 0 ? child->implicitHeight() : 40);

    if (!attached) {
        s.size.width = std::to_string(fallbackW) + "px";
        s.size.height = std::to_string(fallbackH) + "px";
        return s;
    }

    const QString w = attached->width();
    const QString h = attached->height();
    s.size.width = (w == QLatin1String("auto") || w.isEmpty())
                       ? std::to_string(fallbackW) + "px"
                       : toStd(w);
    s.size.height = (h == QLatin1String("auto") || h.isEmpty())
                        ? std::to_string(fallbackH) + "px"
                        : toStd(h);

    s.minSize.width = toStd(attached->minWidth());
    s.minSize.height = toStd(attached->minHeight());
    s.maxSize.width = toStd(attached->maxWidth());
    s.maxSize.height = toStd(attached->maxHeight());
    s.aspectRatio = attached->aspectRatio();

    s.flexBasis = toStd(attached->flexBasis());
    s.flexGrow = attached->flexGrow();
    s.flexShrink = attached->flexShrink();

    s.hasAlignSelf = attached->hasAlignSelf();
    s.alignSelf = toStd(attached->alignSelf());
    s.hasJustifySelf = attached->hasJustifySelf();
    s.justifySelf = toStd(attached->justifySelf());

    s.margin.top = std::to_string(attached->marginTop()) + "px";
    s.margin.right = std::to_string(attached->marginRight()) + "px";
    s.margin.bottom = std::to_string(attached->marginBottom()) + "px";
    s.margin.left = std::to_string(attached->marginLeft()) + "px";

    s.padding.top = std::to_string(attached->paddingTop()) + "px";
    s.padding.right = std::to_string(attached->paddingRight()) + "px";
    s.padding.bottom = std::to_string(attached->paddingBottom()) + "px";
    s.padding.left = std::to_string(attached->paddingLeft()) + "px";

    s.border.top = std::to_string(attached->borderTop()) + "px";
    s.border.right = std::to_string(attached->borderRight()) + "px";
    s.border.bottom = std::to_string(attached->borderBottom()) + "px";
    s.border.left = std::to_string(attached->borderLeft()) + "px";

    s.position = toStd(attached->position());
    s.inset.top = toStd(attached->insetTop());
    s.inset.right = toStd(attached->insetRight());
    s.inset.bottom = toStd(attached->insetBottom());
    s.inset.left = toStd(attached->insetLeft());

    return s;
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
    m_engine.clear();
    const unsigned root = m_engine.addNode(containerStyle(QSizeF()));
    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        QQuickItem* child = itemAt(i);
        const unsigned node = m_engine.addNode(itemStyle(child));
        m_engine.addChild(root, node);
    }

    switch (whichSizeHint) {
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
    const unsigned root = m_engine.addNode(containerStyle(size));

    QList<QQuickItem*> layoutItems;
    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        QQuickItem* child = itemAt(i);
        layoutItems.append(child);
        const unsigned node = m_engine.addNode(itemStyle(child));
        m_engine.addChild(root, node);
    }

    if (!m_engine.compute(size.width(), size.height())) {
        return;
    }

    for (int i = 0; i < layoutItems.size(); ++i) {
        taffy::Layout l;
        if (m_engine.layout(unsigned(i + 1), l)) {
            layoutItems[i]->setX(l.location.x);
            layoutItems[i]->setY(l.location.y);
            layoutItems[i]->setSize(QSizeF(l.size.width, l.size.height));
        }
    }
}
