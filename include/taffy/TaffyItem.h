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
    Q_PROPERTY(TaffyTypes::Display display READ display WRITE setDisplay NOTIFY displayChanged)
    Q_PROPERTY(TaffyTypes::FlexDirection flexDirection READ flexDirection WRITE setFlexDirection NOTIFY flexDirectionChanged)
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit TaffyItem(QQuickItem* parent = nullptr);
    ~TaffyItem() override;

    TaffyTypes::Display display() const;
    void setDisplay(TaffyTypes::Display display);

    TaffyTypes::FlexDirection flexDirection() const;
    void setFlexDirection(TaffyTypes::FlexDirection direction);

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
    TaffyTypes::Display m_display = TaffyTypes::DisplayFlex;
    TaffyTypes::FlexDirection m_flexDirection = TaffyTypes::FlexDirectionRow;
    qreal m_spacing = 4.0;
    QColor m_color{ 0x3b, 0x82, 0xf6 };
};
