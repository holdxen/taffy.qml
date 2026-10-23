#pragma once

#include <QColor>
#include <QQuickItem>
#include <QString>

#include <taffy/TaffyEngine.h>

/**
 * Simple QQuickItem that uses the taffy engine (CSS-string style).
 */
class TaffyItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString display READ display WRITE setDisplay NOTIFY displayChanged)
    Q_PROPERTY(QString flexDirection READ flexDirection WRITE setFlexDirection NOTIFY flexDirectionChanged)
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit TaffyItem(QQuickItem* parent = nullptr);
    ~TaffyItem() override;

    QString display() const;
    void setDisplay(const QString& v);
    QString flexDirection() const;
    void setFlexDirection(const QString& v);
    qreal spacing() const;
    void setSpacing(qreal v);
    QColor color() const;
    void setColor(const QColor& v);

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
    QString m_display = QStringLiteral("flex");
    QString m_flexDirection = QStringLiteral("row");
    qreal m_spacing = 4.0;
    QColor m_color{ 0x3b, 0x82, 0xf6 };
};
