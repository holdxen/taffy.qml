#include "taffy/TaffyLayoutAttached.h"
#include "taffy/TaffyEngine.h"

TaffyLayoutAttached::TaffyLayoutAttached(QObject* parent)
    : QObject(parent)
{
}

QString TaffyLayoutAttached::width() const
{
    return m_width;
}
void TaffyLayoutAttached::setWidth(const QString& v)
{
    m_width = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::height() const
{
    return m_height;
}
void TaffyLayoutAttached::setHeight(const QString& v)
{
    m_height = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::minWidth() const
{
    return m_minWidth;
}
void TaffyLayoutAttached::setMinWidth(const QString& v)
{
    m_minWidth = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::minHeight() const
{
    return m_minHeight;
}
void TaffyLayoutAttached::setMinHeight(const QString& v)
{
    m_minHeight = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::maxWidth() const
{
    return m_maxWidth;
}
void TaffyLayoutAttached::setMaxWidth(const QString& v)
{
    m_maxWidth = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::maxHeight() const
{
    return m_maxHeight;
}
void TaffyLayoutAttached::setMaxHeight(const QString& v)
{
    m_maxHeight = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::flexBasis() const
{
    return m_flexBasis;
}
void TaffyLayoutAttached::setFlexBasis(const QString& v)
{
    m_flexBasis = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::alignSelf() const
{
    return m_alignSelf;
}
void TaffyLayoutAttached::setAlignSelf(const QString& v)
{
    m_alignSelf = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::justifySelf() const
{
    return m_justifySelf;
}
void TaffyLayoutAttached::setJustifySelf(const QString& v)
{
    m_justifySelf = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::position() const
{
    return m_position;
}
void TaffyLayoutAttached::setPosition(const QString& v)
{
    m_position = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::insetTop() const
{
    return m_insetTop;
}
void TaffyLayoutAttached::setInsetTop(const QString& v)
{
    m_insetTop = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::insetRight() const
{
    return m_insetRight;
}
void TaffyLayoutAttached::setInsetRight(const QString& v)
{
    m_insetRight = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::insetBottom() const
{
    return m_insetBottom;
}
void TaffyLayoutAttached::setInsetBottom(const QString& v)
{
    m_insetBottom = v;
    emit itemStyleChanged();
}

QString TaffyLayoutAttached::insetLeft() const
{
    return m_insetLeft;
}
void TaffyLayoutAttached::setInsetLeft(const QString& v)
{
    m_insetLeft = v;
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


QString TaffyLayoutAttached::gridRow() const { return m_gridRow; }
void TaffyLayoutAttached::setGridRow(const QString& v) { m_gridRow = v; emit itemStyleChanged(); }
QString TaffyLayoutAttached::gridColumn() const { return m_gridColumn; }
void TaffyLayoutAttached::setGridColumn(const QString& v) { m_gridColumn = v; emit itemStyleChanged(); }
QString TaffyLayoutAttached::boxSizing() const { return m_boxSizing; }
void TaffyLayoutAttached::setBoxSizing(const QString& v) { m_boxSizing = v; emit itemStyleChanged(); }
QString TaffyLayoutAttached::overflowX() const { return m_overflowX; }
void TaffyLayoutAttached::setOverflowX(const QString& v) { m_overflowX = v; emit itemStyleChanged(); }
QString TaffyLayoutAttached::overflowY() const { return m_overflowY; }
void TaffyLayoutAttached::setOverflowY(const QString& v) { m_overflowY = v; emit itemStyleChanged(); }
QString TaffyLayoutAttached::cssFloat() const { return m_cssFloat; }
void TaffyLayoutAttached::setCssFloat(const QString& v) { m_cssFloat = v; emit itemStyleChanged(); }
QString TaffyLayoutAttached::clear() const { return m_clear; }
void TaffyLayoutAttached::setClear(const QString& v) { m_clear = v; emit itemStyleChanged(); }
QString TaffyLayoutAttached::textAlign() const { return m_textAlign; }
void TaffyLayoutAttached::setTextAlign(const QString& v) { m_textAlign = v; emit itemStyleChanged(); }
bool TaffyLayoutAttached::itemIsTable() const { return m_itemIsTable; }
void TaffyLayoutAttached::setItemIsTable(bool v) { m_itemIsTable = v; emit itemStyleChanged(); }
bool TaffyLayoutAttached::itemIsReplaced() const { return m_itemIsReplaced; }
void TaffyLayoutAttached::setItemIsReplaced(bool v) { m_itemIsReplaced = v; emit itemStyleChanged(); }
bool TaffyLayoutAttached::itemIsCompressibleReplaced() const { return m_itemIsCompressibleReplaced; }
void TaffyLayoutAttached::setItemIsCompressibleReplaced(bool v) { m_itemIsCompressibleReplaced = v; emit itemStyleChanged(); }

#include <QMetaProperty>
#include <QTextStream>

QVariantMap TaffyLayoutAttached::taffyStyle() const
{
    QVariantMap map;
    const QMetaObject* mo = metaObject();
    for (int i = 0; i < mo->propertyCount(); ++i) {
        const QMetaProperty p = mo->property(i);
        if (p.isConstant())
            continue;
        // Skip the debug aggregate itself to avoid recursion / noise.
        if (QLatin1String(p.name()) == QLatin1String("taffyStyle"))
            continue;
        map.insert(QLatin1String(p.name()), p.read(this));
    }
    return map;
}

QString TaffyLayoutAttached::dump() const
{
    QString s;
    QTextStream out(&s);
    out << "TaffyLayoutAttached(" << static_cast<const void*>(this) << ")\n";
    out << "  parent = " << static_cast<const void*>(parent()) << "\n";

    const QMetaObject* mo = metaObject();
    for (int i = 0; i < mo->propertyCount(); ++i) {
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
