#pragma once

#include <QColor>
#include <QQuickItem>

/**
 * Public QQuickItem shell. Geometry / scene-graph work lives here;
 * layout computation is delegated to the Rust LayoutEngine (cxx-qt).
 *
 * Registered to QML as `Taffy.TaffyItem` via taffy::registerQmlTypes().
 */
class TaffyItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qint32 mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit TaffyItem(QQuickItem* parent = nullptr);
    ~TaffyItem() override;

    qint32 mode() const;
    void setMode(qint32 mode);

    qreal spacing() const;
    void setSpacing(qreal spacing);

    QColor color() const;
    void setColor(const QColor& color);

signals:
    void modeChanged();
    void spacingChanged();
    void colorChanged();

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

private:
    class LayoutEngine* m_engine = nullptr; // owned, created from Rust
    qint32 m_mode = 0;
    qreal m_spacing = 4.0;
    QColor m_color{ 0x3b, 0x82, 0xf6 };
};
