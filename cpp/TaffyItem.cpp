#include "taffy/TaffyItem.h"

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

TaffyItem::TaffyItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

TaffyItem::~TaffyItem() = default;

Display TaffyItem::display() const
{
    return m_display;
}

void TaffyItem::setDisplay(Display display)
{
    if (m_display == display) {
        return;
    }
    m_display = display;
    emit displayChanged();
    relayout();
}

FlexDirection TaffyItem::flexDirection() const
{
    return m_flexDirection;
}

void TaffyItem::setFlexDirection(FlexDirection direction)
{
    if (m_flexDirection == direction) {
        return;
    }
    m_flexDirection = direction;
    emit flexDirectionChanged();
    relayout();
}

qreal TaffyItem::spacing() const
{
    return m_spacing;
}

void TaffyItem::setSpacing(qreal spacing)
{
    if (qFuzzyCompare(m_spacing, spacing)) {
        return;
    }
    m_spacing = spacing;
    emit spacingChanged();
    relayout();
}

QColor TaffyItem::color() const
{
    return m_color;
}

void TaffyItem::setColor(const QColor& color)
{
    if (m_color == color) {
        return;
    }
    m_color = color;
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
    rootStyle.display = static_cast<qint32>(m_display);
    rootStyle.flexDirection = static_cast<qint32>(m_flexDirection);
    rootStyle.alignItems = static_cast<qint32>(AlignItems::Stretch);
    rootStyle.size.width = taffy::LengthValue::length(width());
    rootStyle.size.height = taffy::LengthValue::length(height());
    const quint32 root = m_engine.addNode(rootStyle);

    const auto kids = childItems();
    for (int i = 0; i < kids.size(); ++i) {
        taffy::Style childStyle;
        childStyle.display = static_cast<qint32>(Display::Flex);
        const qreal w = kids[i]->width();
        const qreal h = kids[i]->height();
        childStyle.size.width = taffy::LengthValue::length(w > 0 ? w : 100.0);
        childStyle.size.height = taffy::LengthValue::length(h > 0 ? h : 40.0);
        const quint32 child = m_engine.addNode(childStyle);
        m_engine.addChild(root, child);
    }

    if (!m_engine.compute(width(), height())) {
        return;
    }

    for (int i = 0; i < kids.size(); ++i) {
        taffy::Layout childLayout;
        // child nodes were added at indices 1..n
        if (m_engine.layout(quint32(i + 1), childLayout)) {
            kids[i]->setX(childLayout.location.x);
            kids[i]->setY(childLayout.location.y);
            kids[i]->setSize(QSizeF(childLayout.size.width, childLayout.size.height));
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
