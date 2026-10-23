#pragma once

#include <QColor>
#include <QQuickItem>

#include <taffy/TaffyEngine.h>
#include <taffy/TaffyTypes.h>

/**
 * QQuickItem that asks the Rust taffy engine for child layout.
 * All Qt logic lives in C++; Rust only does layout math.
 */
class TaffyItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Display display READ display WRITE setDisplay NOTIFY displayChanged)
    Q_PROPERTY(FlexDirection flexDirection READ flexDirection WRITE setFlexDirection NOTIFY flexDirectionChanged)
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit TaffyItem(QQuickItem* parent = nullptr);
    ~TaffyItem() override;

    Display display() const;
    void setDisplay(Display display);

    FlexDirection flexDirection() const;
    void setFlexDirection(FlexDirection direction);

    qreal spacing() const;
    void setSpacing(qreal spacing);

    QColor color() const;
    void setColor(const QColor& color);

signals:
    void displayChanged();
    void flexDirectionChanged();
    void spacingChanged();
    void colorChanged();

protected:
    void geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry) override;
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

private:
    void relayout();

    taffy::Engine m_engine;
    Display m_display = Display::Flex;
    FlexDirection m_flexDirection = FlexDirection::Row;
    qreal m_spacing = 4.0;
    QColor m_color{ 0x3b, 0x82, 0xf6 };
};
