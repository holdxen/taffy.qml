#include "taffy/TaffyLayout.h"

#include <algorithm>

#include <QDebug>
#include <QMetaProperty>
#include <QTextStream>
#include <QtQml/qqml.h>
#include <QtQuick/private/qquickanchors_p.h>
#include <QtQuick/private/qquickitem_p.h>

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

// "1 / 4" -> start=1 end=4; "span 2" -> start=span 2 end=auto
void setLinePlacement(taffy::LinePlacement& p, const QString& css)
{
    const QString s = css.trimmed();
    if (s.isEmpty()) {
        p.start = "auto";
        p.end = "auto";
        return;
    }
    const auto slash = s.indexOf(QLatin1Char('/'));
    if (slash < 0) {
        p.start = s.toStdString();
        p.end = "auto";
        return;
    }
    p.start = s.left(slash).trimmed().toStdString();
    const QString end = s.mid(slash + 1).trimmed();
    p.end = end.isEmpty() ? std::string("auto") : end.toStdString();
}

// Accept "100" as well as "100px" for length-like values.
std::string lengthCss(const QString& s)
{
    QString t = s.trimmed();
    if (t.isEmpty() || t == QLatin1String("auto")
        || t == QLatin1String("min-content") || t == QLatin1String("max-content")
        || t == QLatin1String("stretch") || t == QLatin1String("content")
        || t == QLatin1String("fit-content") || t.endsWith(QLatin1String("%"))
        || t.endsWith(QLatin1String("px")) || t.endsWith(QLatin1String("fr"))) {
        return t.toStdString();
    }
    bool ok = false;
    t.toDouble(&ok);
    if (ok) {
        return t.toStdString() + "px";
    }
    return t.toStdString();
}

} // namespace

TaffyLayout::TaffyLayout(QQuickItem* parent)
    : QQuickLayout(*new TaffyLayoutPrivate, parent)
{
}

TaffyLayout::~TaffyLayout() = default;

TaffyLayoutAttached* TaffyLayout::qmlAttachedProperties(QObject* object)
{
    auto* attached = new TaffyLayoutAttached(object);
    if (auto* child = qobject_cast<QQuickItem*>(object)) {
        if (auto* layout = qobject_cast<TaffyLayout*>(child->parentItem())) {
            QObject::connect(attached, &TaffyLayoutAttached::itemStyleChanged,
                             layout, &TaffyLayout::onItemStyleChanged,
                             Qt::UniqueConnection);
        }
    }
    return attached;
}

void TaffyLayout::watchItemStyle(QQuickItem* child) const
{
    if (!child) {
        return;
    }
    auto* self = const_cast<TaffyLayout*>(this);
    // Controls (Button, etc.) can update implicit size after the first polish
    // (9→20→42→57). If we already positioned against a stale snapshot, flex-end
    // overflows the container until the next external rearrange. Re-layout here.
    QObject::connect(child, &QQuickItem::implicitWidthChanged,
                     self, &TaffyLayout::onChildImplicitSizeChanged,
                     Qt::UniqueConnection);
    QObject::connect(child, &QQuickItem::implicitHeightChanged,
                     self, &TaffyLayout::onChildImplicitSizeChanged,
                     Qt::UniqueConnection);
    auto* attached = qobject_cast<TaffyLayoutAttached*>(
        qmlAttachedPropertiesObject<TaffyLayout>(child, false));
    if (!attached) {
        return;
    }
    QObject::connect(attached, &TaffyLayoutAttached::itemStyleChanged,
                     self, &TaffyLayout::onItemStyleChanged,
                     Qt::UniqueConnection);
}

void TaffyLayout::onItemStyleChanged()
{
    invalidate();
}

void TaffyLayout::onChildImplicitSizeChanged()
{
    invalidate();
}

void TaffyLayout::componentComplete()
{
    QQuickLayout::componentComplete();
    applySizeHintsToItem();
    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        QQuickItem* child = itemAt(i);
        watchItemStyle(child);
        checkItemAnchors(child);
    }
    // First layout, same as QQuickFlexboxLayout: skip if a parent layout will drive us.
    if (qobject_cast<QQuickLayout*>(parentItem())) {
        return;
    }
    // Anchors may not be resolved yet (e.g. anchors.fill → negative margins).
    if (width() > 0 && height() > 0) {
        rearrange(QSizeF(width(), height()));
    }
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
void TaffyLayout::resetGapWidth()
{
    if (qFuzzyCompare(m_gapWidth, 0.0))
        return;
    m_gapWidth = 0.0;
    emit styleChanged();
    invalidate();
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
void TaffyLayout::resetGapHeight()
{
    if (qFuzzyCompare(m_gapHeight, 0.0))
        return;
    m_gapHeight = 0.0;
    emit styleChanged();
    invalidate();
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


QString TaffyLayout::gridTemplateRows() const { return m_gridTemplateRows; }
void TaffyLayout::setGridTemplateRows(const QString& v)
{
    if (m_gridTemplateRows == v)
        return;
    m_gridTemplateRows = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::gridTemplateColumns() const { return m_gridTemplateColumns; }
void TaffyLayout::setGridTemplateColumns(const QString& v)
{
    if (m_gridTemplateColumns == v)
        return;
    m_gridTemplateColumns = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::gridAutoRows() const { return m_gridAutoRows; }
void TaffyLayout::setGridAutoRows(const QString& v)
{
    if (m_gridAutoRows == v)
        return;
    m_gridAutoRows = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::gridAutoColumns() const { return m_gridAutoColumns; }
void TaffyLayout::setGridAutoColumns(const QString& v)
{
    if (m_gridAutoColumns == v)
        return;
    m_gridAutoColumns = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::gridTemplateAreas() const { return m_gridTemplateAreas; }
void TaffyLayout::setGridTemplateAreas(const QString& v)
{
    if (m_gridTemplateAreas == v)
        return;
    m_gridTemplateAreas = v;
    emit styleChanged();
    invalidate();
}
QStringList TaffyLayout::gridTemplateRowNames() const { return m_gridTemplateRowNames; }
void TaffyLayout::setGridTemplateRowNames(const QStringList& v)
{
    if (m_gridTemplateRowNames == v)
        return;
    m_gridTemplateRowNames = v;
    emit styleChanged();
    invalidate();
}
QStringList TaffyLayout::gridTemplateColumnNames() const { return m_gridTemplateColumnNames; }
void TaffyLayout::setGridTemplateColumnNames(const QStringList& v)
{
    if (m_gridTemplateColumnNames == v)
        return;
    m_gridTemplateColumnNames = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::boxSizing() const { return m_boxSizing; }
void TaffyLayout::setBoxSizing(const QString& v)
{
    if (m_boxSizing == v)
        return;
    m_boxSizing = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::overflowX() const { return m_overflowX; }
void TaffyLayout::setOverflowX(const QString& v)
{
    if (m_overflowX == v)
        return;
    m_overflowX = v;
    emit styleChanged();
    invalidate();
}
QString TaffyLayout::overflowY() const { return m_overflowY; }
void TaffyLayout::setOverflowY(const QString& v)
{
    if (m_overflowY == v)
        return;
    m_overflowY = v;
    emit styleChanged();
    invalidate();
}
qreal TaffyLayout::scrollbarWidth() const { return m_scrollbarWidth; }
void TaffyLayout::setScrollbarWidth(qreal v)
{
    if (qFuzzyCompare(m_scrollbarWidth, v))
        return;
    m_scrollbarWidth = v;
    emit styleChanged();
    invalidate();
}

taffy::Style TaffyLayout::containerStyle(const QSizeF& size) const
{
    taffy::Style s;
    s.display = toStd(m_display);
    s.flexDirection = toStd(m_flexDirection);
    s.flexWrap = toStd(m_flexWrap);
    s.flexLineCount = std::uint32_t(std::max(1, m_flexLineCount));
    s.alignItems = toStd(m_alignItems);
    s.alignContent = toStd(m_alignContent);
    s.justifyContent = toStd(m_justifyContent);
    s.gap.width = std::to_string(m_gapWidth) + "px";
    s.gap.height = std::to_string(m_gapHeight) + "px";
    s.gridAutoFlow = toStd(m_gridAutoFlow);
    s.gridTemplateRows = toStd(m_gridTemplateRows);
    s.gridTemplateColumns = toStd(m_gridTemplateColumns);
    s.gridAutoRows = toStd(m_gridAutoRows);
    s.gridAutoColumns = toStd(m_gridAutoColumns);
    s.gridTemplateAreas = toStd(m_gridTemplateAreas);
    s.gridTemplateRowNames.clear();
    for (const auto& n : m_gridTemplateRowNames) {
        s.gridTemplateRowNames.push_back(n.toStdString());
    }
    s.gridTemplateColumnNames.clear();
    for (const auto& n : m_gridTemplateColumnNames) {
        s.gridTemplateColumnNames.push_back(n.toStdString());
    }
    s.boxSizing = toStd(m_boxSizing);
    s.overflowX = toStd(m_overflowX);
    s.overflowY = toStd(m_overflowY);
    s.scrollbarWidth = m_scrollbarWidth;
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

    watchItemStyle(child);
    auto* attached = qobject_cast<TaffyLayoutAttached*>(qmlAttachedPropertiesObject<TaffyLayout>(child, false));

    // Same size-hint chain as QQuickFlexboxLayout:
    // QQuickLayoutAttached (Layout.*) → implicitWidth/Height → width/height.
    QSizeF hints[Qt::NSizeHints];
    QQuickLayoutAttached* layoutInfo = nullptr;
    QQuickLayout::effectiveSizeHints_helper(child, hints, &layoutInfo, true);
    const QSizeF pref = hints[Qt::PreferredSize];

    // Content size for taffy leaf measure (Qt FlexboxLayout does this via Yoga).
    // Cross-axis auto + align-items:center/start/end shrinks to this instead of 0.
    s.measureWidth = pref.width();
    s.measureHeight = pref.height();

    auto toLen = [](qreal v) -> std::string {
        if (v <= 0)
            return "auto";
        return std::to_string(v) + "px";
    };

    if (!attached) {
        // No attached flex props: use content size on the main axis, stretch on cross.
        const bool rowMain = (m_flexDirection == QLatin1String("row")
                              || m_flexDirection == QLatin1String("row-reverse"));
        if (rowMain) {
            s.size.width = toLen(pref.width());
            s.size.height = "auto";
        } else {
            s.size.height = toLen(pref.height());
            s.size.width = "auto";
        }
        return s;
    }

    const QString w = attached->width();
    const QString h = attached->height();
    const bool wAuto = (w == QLatin1String("auto") || w.isEmpty());
    const bool hAuto = (h == QLatin1String("auto") || h.isEmpty());

    // CSS flex: flex-basis applies to the main axis and overrides width/height
    // for the flex base size. Cross-axis "auto" allows align-items: stretch.
    // Only bake in content size on the main axis when flex-basis is auto.
    const QString basis = attached->flexBasis();
    const bool basisAuto = basis.isEmpty() || basis == QLatin1String("auto");
    const bool rowMain = (m_flexDirection == QLatin1String("row")
                          || m_flexDirection == QLatin1String("row-reverse"));

    // Main axis: flex-basis (or content when basis is auto). Cross axis: auto (stretch).
    const std::string basisCss = lengthCss(basis);
    if (rowMain) {
        s.size.width = wAuto ? (basisAuto ? toLen(pref.width()) : basisCss)
                             : lengthCss(w);
        s.size.height = hAuto ? std::string("auto") : lengthCss(h);
    } else {
        s.size.height = hAuto ? (basisAuto ? toLen(pref.height()) : basisCss)
                              : lengthCss(h);
        s.size.width = wAuto ? std::string("auto") : lengthCss(w);
    }

    // Only apply helper min hints. Do NOT copy MaximumSize here:
    // QQuickLayout max hints are content estimates and would clamp flex-grow.

    // Explicit TaffyLayout min/max override the Layout.* hints above.
    if (!(attached->minWidth() == QLatin1String("auto") || attached->minWidth().isEmpty()))
        s.minSize.width = lengthCss(attached->minWidth());
    if (!(attached->minHeight() == QLatin1String("auto") || attached->minHeight().isEmpty()))
        s.minSize.height = lengthCss(attached->minHeight());
    if (!(attached->maxWidth() == QLatin1String("auto") || attached->maxWidth().isEmpty()))
        s.maxSize.width = lengthCss(attached->maxWidth());
    if (!(attached->maxHeight() == QLatin1String("auto") || attached->maxHeight().isEmpty()))
        s.maxSize.height = lengthCss(attached->maxHeight());
    s.aspectRatio = attached->aspectRatio();

    s.flexBasis = lengthCss(attached->flexBasis());
    s.flexGrow = attached->flexGrow();
    s.flexShrink = attached->flexShrink();

    s.alignSelf = toStd(attached->alignSelf());
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
    s.inset.top = lengthCss(attached->insetTop());
    s.inset.right = lengthCss(attached->insetRight());
    s.inset.bottom = lengthCss(attached->insetBottom());
    s.inset.left = lengthCss(attached->insetLeft());

    setLinePlacement(s.gridRow, attached->gridRow());
    setLinePlacement(s.gridColumn, attached->gridColumn());
    s.boxSizing = toStd(attached->boxSizing());
    s.overflowX = toStd(attached->overflowX());
    s.overflowY = toStd(attached->overflowY());
    s.cssFloat = toStd(attached->cssFloat());
    s.clear = toStd(attached->clear());
    s.textAlign = toStd(attached->textAlign());
    s.itemIsTable = attached->itemIsTable();
    s.itemIsReplaced = attached->itemIsReplaced();
    s.itemIsCompressibleReplaced = attached->itemIsCompressibleReplaced();

    return s;
}

void TaffyLayout::invalidate(QQuickItem* childItem)
{
    m_sizeHintsDirty = true;
    QQuickLayout::invalidate(childItem);
    // QQuickLayout::invalidate() no-ops when m_dirty is already set. That bit
    // starts out true and stays true if we only rearrange via geometryChange /
    // applyChildGeometry (both call rearrange() directly). A later child implicit
    // size change would then be swallowed. Always request a polish.
    polish();
}

void TaffyLayout::applySizeHintsToItem()
{
    // Same effect as QQuickLayoutPrivate::applySizeHints():
    // preferred → implicitWidth/Height, min/max → Layout attached.
    const QSizeF min = sizeHint(Qt::MinimumSize);
    const QSizeF max = sizeHint(Qt::MaximumSize);
    const QSizeF pref = sizeHint(Qt::PreferredSize);
    if (QQuickLayoutAttached* info = attachedLayoutObject(this, true)) {
        info->setMinimumImplicitSize(min);
        info->setMaximumImplicitSize(max);
    }
    setImplicitSize(pref.width(), pref.height());
}

void TaffyLayout::updateLayoutItems()
{
    applySizeHintsToItem();
    invalidate();
}

void TaffyLayout::relayout()
{
    invalidate();
    if (isReady() && width() > 0 && height() > 0)
        rearrange(QSizeF(width(), height()));
}

void TaffyLayout::checkItemAnchors(QQuickItem* item)
{
    if (!item) {
        return;
    }
    QQuickAnchors* anchors = QQuickItemPrivate::get(item)->_anchors;
    if (anchors && anchors->activeDirections()) {
        qmlWarning(item) << "Detected anchors on an item that is managed by a layout."
                            " This is undefined behavior; use TaffyLayout properties instead.";
    }
}

void TaffyLayout::itemSiblingOrderChanged(QQuickItem* item)
{
    if (!isReady())
        return;
    // childItems() order is the layout order — just rebuild.
    invalidate(item);
}

void TaffyLayout::itemVisibilityChanged(QQuickItem* item)
{
    Q_UNUSED(item);
    // Base is empty; hidden children are skipped by shouldIgnoreItem, so
    // toggle them out of / into the flex flow.
    relayout();
}

void TaffyLayout::geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickLayout::geometryChange(newGeometry, oldGeometry);
    // Base skips rearrange when invalidated(); if size just became valid we must
    // still lay out (e.g. anchors.fill resolved after componentComplete).
    if (!isReady())
        return;
    if (newGeometry.width() > 0 && newGeometry.height() > 0)
        rearrange(newGeometry.size());
}

void TaffyLayout::itemChange(ItemChange change, const ItemChangeData& value)
{
    QQuickLayout::itemChange(change, value);
    if (change == ItemChildAddedChange) {
        if (value.item) {
            watchItemStyle(value.item);
            checkItemAnchors(value.item);
        }
        invalidate();
    } else if (change == ItemChildRemovedChange || change == ItemVisibleHasChanged) {
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

void TaffyLayout::applyChildGeometry(QQuickItem* child, const taffy::Layout& l)
{
    const QSizeF oldSize = child->size();
    const QSizeF newSize(l.size.width, l.size.height);

    child->setX(l.location.x);
    child->setY(l.location.y);

    if (oldSize == newSize) {
        // Nested layout: size unchanged but it may still need to rearrange.
        if (auto* layout = qobject_cast<QQuickLayout*>(child)) {
            if (layout->invalidatedArrangement()) {
                layout->rearrange(newSize);
            }
        }
    } else {
        child->setWidth(newSize.width());
        child->setHeight(newSize.height());
        if (auto* layout = qobject_cast<QQuickLayout*>(child)) {
            layout->rearrange(newSize);
        }
    }

    // The item may clamp/expand to its own content (e.g. Button implicit size
    // settles after polish). Positions computed for the taffy size are then wrong.
    if (!qFuzzyCompare(child->width(), newSize.width())
        || !qFuzzyCompare(child->height(), newSize.height())) {
        invalidate();
    }
}

QSizeF TaffyLayout::sizeHint(Qt::SizeHint whichSizeHint) const
{
    const int idx = int(whichSizeHint);
    if (idx >= 0 && idx < Qt::NSizeHints && !m_sizeHintsDirty) {
        return m_cachedSizeHints[idx];
    }

    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        watchItemStyle(itemAt(i));
        checkItemAnchors(itemAt(i));
    }

    // Fill all three hints so the cache is complete.
    static const Qt::SizeHint kAll[] = { Qt::MinimumSize, Qt::PreferredSize, Qt::MaximumSize };
    for (Qt::SizeHint which : kAll) {
        m_engine.clear();
        const std::uint32_t r = m_engine.addNode(containerStyle(QSizeF()));
        for (int i = 0; i < n; ++i) {
            QQuickItem* child = itemAt(i);
            const std::uint32_t node = m_engine.addNode(itemStyle(child));
            m_engine.addChild(r, node);
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
        if (m_engine.layout(r, rootLayout)) {
            m_cachedSizeHints[int(which)] = QSizeF(rootLayout.size.width, rootLayout.size.height);
        } else {
            m_cachedSizeHints[int(which)] = QSizeF(0, 0);
        }
    }

    m_sizeHintsDirty = false;
    if (idx >= 0 && idx < Qt::NSizeHints) {
        return m_cachedSizeHints[idx];
    }
    return m_cachedSizeHints[Qt::PreferredSize];
}

void TaffyLayout::rearrange(const QSizeF& size)
{
    // Same guard as QQuickFlexboxLayout::rearrange — never feed taffy a bogus size.
    if (!size.isValid() || size.width() < 0 || size.height() < 0) {
        return;
    }
    if (size.width() == 0 || size.height() == 0) {
        return;
    }

    m_engine.clear();
    const std::uint32_t root = m_engine.addNode(containerStyle(size));

    QList<QQuickItem*> layoutItems;
    const int n = itemCount();
    for (int i = 0; i < n; ++i) {
        QQuickItem* child = itemAt(i);
        layoutItems.append(child);
        const std::uint32_t node = m_engine.addNode(itemStyle(child));
        m_engine.addChild(root, node);
    }

    if (!m_engine.compute(size.width(), size.height())) {
        return;
    }

    for (int i = 0; i < layoutItems.size(); ++i) {
        taffy::Layout l;
        if (m_engine.layout(std::uint32_t(i + 1), l)) {
            applyChildGeometry(layoutItems[i], l);
        }
    }

    applySizeHintsToItem();
}

static int taffyPropertyOffset(const QMetaObject* mo)
{
    for (const QMetaObject* m = mo; m; m = m->superClass()) {
        if (QLatin1String(m->className()) == QLatin1String("TaffyLayout"))
            return m->propertyOffset();
    }
    return 0;
}

QVariantMap TaffyLayout::taffyStyle() const
{
    QVariantMap map;
    const QMetaObject* mo = metaObject();
    const int start = taffyPropertyOffset(mo);
    for (int i = start; i < mo->propertyCount(); ++i) {
        const QMetaProperty p = mo->property(i);
        if (p.isConstant())
            continue;
        if (QLatin1String(p.name()) == QLatin1String("taffyStyle"))
            continue;
        map.insert(QLatin1String(p.name()), p.read(this));
    }
    return map;
}

QString TaffyLayout::dump() const
{
    QString s;
    QTextStream out(&s);
    out << "TaffyLayout(" << static_cast<const void*>(this) << ")\n";
    out << "  parent = " << static_cast<const void*>(parentItem()) << "\n";
    out << "  size   = " << width() << " x " << height() << "\n";

    const QMetaObject* mo = metaObject();
    const int start = taffyPropertyOffset(mo);
    for (int i = start; i < mo->propertyCount(); ++i) {
        const QMetaProperty p = mo->property(i);
        if (p.isConstant())
            continue;
        if (QLatin1String(p.name()) == QLatin1String("taffyStyle"))
            continue;
        const QVariant v = p.read(this);
        out << "  " << QString::fromLatin1(p.name()).leftJustified(22, ' ')
            << " = " << v.toString() << "\n";
    }
    return s;
}

QString TaffyLayout::dumpTree() const
{
    QString s;
    QTextStream out(&s);
    out << "TaffyLayout(" << static_cast<const void*>(this) << ") children=" << itemCount() << "\n";
    for (int i = 0; i < itemCount(); ++i) {
        QQuickItem* child = itemAt(i);
        out << "\n[" << i << "] " << child->metaObject()->className()
            << " " << static_cast<const void*>(child) << "\n";
        auto* attached = qobject_cast<TaffyLayoutAttached*>(
            qmlAttachedPropertiesObject<TaffyLayout>(child, false));
        if (!attached) {
            out << "  (no TaffyLayout attached)\n";
            continue;
        }
        out << attached->dump();
    }
    return s;
}
