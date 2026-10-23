#pragma once

#include <QtQuickLayouts/private/qquicklayout_p.h>

#include <taffy/TaffyEngine.h>
#include <taffy/TaffyLayoutAttached.h>

class TaffyLayoutPrivate;

/**
 * QQuickLayout laid out by Rust taffy. Style values are CSS strings.
 *
 * Container fields are properties on TaffyLayout.
 * Item fields are attached properties (QML: TaffyLayout.xxx on children).
 */
class TaffyLayout : public QQuickLayout
{
    Q_OBJECT
    QML_ATTACHED(TaffyLayoutAttached)
    Q_PROPERTY(QString display READ display WRITE setDisplay NOTIFY styleChanged)
    Q_PROPERTY(QString flexDirection READ flexDirection WRITE setFlexDirection NOTIFY styleChanged)
    Q_PROPERTY(QString flexWrap READ flexWrap WRITE setFlexWrap NOTIFY styleChanged)
    Q_PROPERTY(int flexLineCount READ flexLineCount WRITE setFlexLineCount NOTIFY styleChanged)
    Q_PROPERTY(QString alignItems READ alignItems WRITE setAlignItems NOTIFY styleChanged)
    Q_PROPERTY(QString alignContent READ alignContent WRITE setAlignContent NOTIFY styleChanged)
    Q_PROPERTY(QString justifyContent READ justifyContent WRITE setJustifyContent NOTIFY styleChanged)
    Q_PROPERTY(qreal gapWidth READ gapWidth WRITE setGapWidth NOTIFY styleChanged)
    Q_PROPERTY(qreal gapHeight READ gapHeight WRITE setGapHeight NOTIFY styleChanged)
    Q_PROPERTY(QString gridAutoFlow READ gridAutoFlow WRITE setGridAutoFlow NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingTop READ paddingTop WRITE setPaddingTop NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingRight READ paddingRight WRITE setPaddingRight NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingBottom READ paddingBottom WRITE setPaddingBottom NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingLeft READ paddingLeft WRITE setPaddingLeft NOTIFY styleChanged)
    Q_PROPERTY(QString direction READ direction WRITE setDirection NOTIFY styleChanged)

public:
    explicit TaffyLayout(QQuickItem* parent = nullptr);
    ~TaffyLayout() override;

    static TaffyLayoutAttached* qmlAttachedProperties(QObject* object);

    QString display() const;
    void setDisplay(const QString& v);
    QString flexDirection() const;
    void setFlexDirection(const QString& v);
    QString flexWrap() const;
    void setFlexWrap(const QString& v);
    int flexLineCount() const;
    void setFlexLineCount(int v);
    QString alignItems() const;
    void setAlignItems(const QString& v);
    QString alignContent() const;
    void setAlignContent(const QString& v);
    QString justifyContent() const;
    void setJustifyContent(const QString& v);
    qreal gapWidth() const;
    void setGapWidth(qreal v);
    qreal gapHeight() const;
    void setGapHeight(qreal v);
    QString gridAutoFlow() const;
    void setGridAutoFlow(const QString& v);
    qreal paddingTop() const;
    void setPaddingTop(qreal v);
    qreal paddingRight() const;
    void setPaddingRight(qreal v);
    qreal paddingBottom() const;
    void setPaddingBottom(qreal v);
    qreal paddingLeft() const;
    void setPaddingLeft(qreal v);
    QString direction() const;
    void setDirection(const QString& v);

    QSizeF sizeHint(Qt::SizeHint whichSizeHint) const override;
    void setAlignment(QQuickItem* item, Qt::Alignment align) override;
    void setStretchFactor(QQuickItem* item, int stretchFactor, Qt::Orientation orient) override;
    void updateLayoutItems() override;
    void rearrange(const QSizeF& size) override;
    QQuickItem* itemAt(int index) const override;
    int itemCount() const override;

signals:
    void styleChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData& value) override;

private:
    Q_DECLARE_PRIVATE(TaffyLayout)

    taffy::Style containerStyle(const QSizeF& size) const;
    taffy::Style itemStyle(QQuickItem* child) const;

    QString m_display = QStringLiteral("flex");
    QString m_flexDirection = QStringLiteral("row");
    QString m_flexWrap = QStringLiteral("nowrap");
    int m_flexLineCount = 1;
    QString m_alignItems = QStringLiteral("normal");
    QString m_alignContent = QStringLiteral("normal");
    QString m_justifyContent = QStringLiteral("normal");
    qreal m_gapWidth = 0.0;
    qreal m_gapHeight = 0.0;
    QString m_gridAutoFlow = QStringLiteral("row");
    qreal m_paddingTop = 0.0;
    qreal m_paddingRight = 0.0;
    qreal m_paddingBottom = 0.0;
    qreal m_paddingLeft = 0.0;
    QString m_direction = QStringLiteral("ltr");
    mutable taffy::Engine m_engine;
};
