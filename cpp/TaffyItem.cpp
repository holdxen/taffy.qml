#include "taffy/TaffyItem.h"

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

TaffyItem::TaffyItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

TaffyItem::~TaffyItem() = default;

QString TaffyItem::display() const
{
    return m_display;
}
void TaffyItem::setDisplay(const QString& v)
{
    if (m_display == v)
        return;
    m_display = v;
    emit displayChanged();
    relayout();
}
QString TaffyItem::flexDirection() const
{
    return m_flexDirection;
}
void TaffyItem::setFlexDirection(const QString& v)
{
    if (m_flexDirection == v)
        return;
    m_flexDirection = v;
    emit flexDirectionChanged();
    relayout();
}
qreal TaffyItem::spacing() const
{
    return m_spacing;
}
void TaffyItem::setSpacing(qreal v)
{
    if (qFuzzyCompare(m_spacing, v))
        return;
    m_spacing = v;
    emit spacingChanged();
    relayout();
}
QColor TaffyItem::color() const
{
    return m_color;
}
void TaffyItem::setColor(const QColor& v)
{
    if (m_color == v)
        return;
    m_color = v;
    emit colorChanged();
    update();
}

void TaffyItem::geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    relayout();
}

void TaffyItem::relayout()
{
    m_engine.clear();

    taffy::Style rootStyle;
    rootStyle.display = m_display.toStdString();
    rootStyle.flexDirection = m_flexDirection.toStdString();
    rootStyle.size.width = std::to_string(width()) + "px";
    rootStyle.size.height = std::to_string(height()) + "px";
    const unsigned root = m_engine.addNode(rootStyle);

    const auto kids = childItems();
    for (int i = 0; i < kids.size(); ++i) {
        taffy::Style childStyle;
        childStyle.display = "flex";
        const qreal w = kids[i]->width();
        const qreal h = kids[i]->height();
        childStyle.size.width = std::to_string(w > 0 ? w : 100.0) + "px";
        childStyle.size.height = std::to_string(h > 0 ? h : 40.0) + "px";
        const unsigned child = m_engine.addNode(childStyle);
        m_engine.addChild(root, child);
    }

    if (!m_engine.compute(width(), height())) {
        return;
    }

    for (int i = 0; i < kids.size(); ++i) {
        taffy::Layout l;
        if (m_engine.layout(unsigned(i + 1), l)) {
            kids[i]->setX(l.location.x);
            kids[i]->setY(l.location.y);
            kids[i]->setSize(QSizeF(l.size.width, l.size.height));
        }
    }
    update();
}

QSGNode* TaffyItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    auto* node = static_cast<QSGGeometryNode*>(oldNode);
    if (!node) {
        node = new QSGGeometryNode;
        auto* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        geometry->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        auto* material = new QSGFlatColorMaterial;
        material->setColor(m_color);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        static_cast<QSGFlatColorMaterial*>(node->material())->setColor(m_color);
    }

    const qreal w = width();
    const qreal h = height();
    QSGGeometry* geometry = node->geometry();
    geometry->allocate(4);
    auto* vertices = geometry->vertexDataAsPoint2D();
    vertices[0].set(0, 0);
    vertices[1].set(float(w), 0);
    vertices[2].set(0, float(h));
    vertices[3].set(float(w), float(h));
    node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return node;
}
