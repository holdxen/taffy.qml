#pragma once

// QQuickLayout is private API in Qt 6 (QtQuickLayouts).
#include <QtQuickLayouts/private/qquicklayout_p.h>

#include <taffy/TaffyEngine.h>
#include <taffy/TaffyLayoutAttached.h>
#include <taffy/TaffyTypes.h>

class TaffyLayoutPrivate;

/**
 * QQuickLayout laid out by the Rust taffy engine.
 *
 * Container-side Style fields are properties on TaffyLayout.
 * Item-side Style fields are attached properties (QML: TaffyLayout.xxx on children).
 */
class TaffyLayout : public QQuickLayout
{
    Q_OBJECT
    QML_ATTACHED(TaffyLayoutAttached)
    // container / parent-side style
    Q_PROPERTY(Display display READ display WRITE setDisplay NOTIFY styleChanged)
    Q_PROPERTY(FlexDirection flexDirection READ flexDirection WRITE setFlexDirection NOTIFY styleChanged)
    Q_PROPERTY(FlexWrap flexWrap READ flexWrap WRITE setFlexWrap NOTIFY styleChanged)
    Q_PROPERTY(qint32 flexLineCount READ flexLineCount WRITE setFlexLineCount NOTIFY styleChanged)
    Q_PROPERTY(AlignItems alignItems READ alignItems WRITE setAlignItems NOTIFY styleChanged)
    Q_PROPERTY(AlignContent alignContent READ alignContent WRITE setAlignContent NOTIFY styleChanged)
    Q_PROPERTY(AlignContent justifyContent READ justifyContent WRITE setJustifyContent NOTIFY styleChanged)
    Q_PROPERTY(qreal gapWidth READ gapWidth WRITE setGapWidth NOTIFY styleChanged)
    Q_PROPERTY(qreal gapHeight READ gapHeight WRITE setGapHeight NOTIFY styleChanged)
    Q_PROPERTY(GridAutoFlow gridAutoFlow READ gridAutoFlow WRITE setGridAutoFlow NOTIFY styleChanged)
    // own box (container chrome)
    Q_PROPERTY(qreal paddingTop READ paddingTop WRITE setPaddingTop NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingRight READ paddingRight WRITE setPaddingRight NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingBottom READ paddingBottom WRITE setPaddingBottom NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingLeft READ paddingLeft WRITE setPaddingLeft NOTIFY styleChanged)
    Q_PROPERTY(StyleDirection direction READ direction WRITE setDirection NOTIFY styleChanged)

public:
    explicit TaffyLayout(QQuickItem* parent = nullptr);
    ~TaffyLayout() override;

    static TaffyLayoutAttached* qmlAttachedProperties(QObject* object);

    Display display() const;
    void setDisplay(Display v);
    FlexDirection flexDirection() const;
    void setFlexDirection(FlexDirection v);
    FlexWrap flexWrap() const;
    void setFlexWrap(FlexWrap v);
    qint32 flexLineCount() const;
    void setFlexLineCount(qint32 v);
    AlignItems alignItems() const;
    void setAlignItems(AlignItems v);
    AlignContent alignContent() const;
    void setAlignContent(AlignContent v);
    AlignContent justifyContent() const;
    void setJustifyContent(AlignContent v);
    qreal gapWidth() const;
    void setGapWidth(qreal v);
    qreal gapHeight() const;
    void setGapHeight(qreal v);
    GridAutoFlow gridAutoFlow() const;
    void setGridAutoFlow(GridAutoFlow v);
    qreal paddingTop() const;
    void setPaddingTop(qreal v);
    qreal paddingRight() const;
    void setPaddingRight(qreal v);
    qreal paddingBottom() const;
    void setPaddingBottom(qreal v);
    qreal paddingLeft() const;
    void setPaddingLeft(qreal v);
    StyleDirection direction() const;
    void setDirection(StyleDirection v);

    // QQuickLayout
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
    QSizeF computeSizeHint(Qt::SizeHint which) const;

    Display m_display = Display::Flex;
    FlexDirection m_flexDirection = FlexDirection::Row;
    FlexWrap m_flexWrap = FlexWrap::NoWrap;
    qint32 m_flexLineCount = 1;
    AlignItems m_alignItems = AlignItems::Normal;
    AlignContent m_alignContent = AlignContent::Normal;
    AlignContent m_justifyContent = AlignContent::Normal;
    qreal m_gapWidth = 0.0;
    qreal m_gapHeight = 0.0;
    GridAutoFlow m_gridAutoFlow = GridAutoFlow::Row;
    qreal m_paddingTop = 0.0;
    qreal m_paddingRight = 0.0;
    qreal m_paddingBottom = 0.0;
    qreal m_paddingLeft = 0.0;
    StyleDirection m_direction = StyleDirection::Ltr;
    mutable taffy::Engine m_engine;
};
