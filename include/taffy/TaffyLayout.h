#pragma once

// QQuickLayout is private API in Qt 6 (QtQuickLayouts).
#include <QtQuickLayouts/private/qquicklayout_p.h>

#include <taffy/TaffyEngine.h>
#include <taffy/TaffyTypes.h>

class TaffyLayoutPrivate;

/**
 * QQuickLayout that lays out children with the Rust taffy engine.
 * Style enums come from cxx-qt generated Q_ENUMs (TaffyTypes).
 */
class TaffyLayout : public QQuickLayout
{
    Q_OBJECT
    Q_PROPERTY(Display display READ display WRITE setDisplay NOTIFY displayChanged)
    Q_PROPERTY(FlexDirection flexDirection READ flexDirection WRITE setFlexDirection NOTIFY flexDirectionChanged)
    Q_PROPERTY(FlexWrap flexWrap READ flexWrap WRITE setFlexWrap NOTIFY flexWrapChanged)
    Q_PROPERTY(AlignItems alignItems READ alignItems WRITE setAlignItems NOTIFY alignItemsChanged)
    Q_PROPERTY(AlignContent alignContent READ alignContent WRITE setAlignContent NOTIFY alignContentChanged)

public:
    explicit TaffyLayout(QQuickItem* parent = nullptr);
    ~TaffyLayout() override;

    Display display() const;
    void setDisplay(Display display);

    FlexDirection flexDirection() const;
    void setFlexDirection(FlexDirection direction);

    FlexWrap flexWrap() const;
    void setFlexWrap(FlexWrap wrap);

    AlignItems alignItems() const;
    void setAlignItems(AlignItems align);

    AlignContent alignContent() const;
    void setAlignContent(AlignContent align);

    // QQuickLayout
    QSizeF sizeHint(Qt::SizeHint whichSizeHint) const override;
    void setAlignment(QQuickItem* item, Qt::Alignment align) override;
    void setStretchFactor(QQuickItem* item, int stretchFactor, Qt::Orientation orient) override;
    void updateLayoutItems() override;
    void rearrange(const QSizeF& size) override;

    // iterator
    QQuickItem* itemAt(int index) const override;
    int itemCount() const override;

signals:
    void displayChanged();
    void flexDirectionChanged();
    void flexWrapChanged();
    void alignItemsChanged();
    void alignContentChanged();

protected:
    void itemChange(ItemChange change, const ItemChangeData& value) override;

private:
    Q_DECLARE_PRIVATE(TaffyLayout)

    void relayout(const QSizeF& size);
    QSizeF computeSizeHint(Qt::SizeHint which) const;

    Display m_display = Display::Flex;
    FlexDirection m_flexDirection = FlexDirection::Row;
    FlexWrap m_flexWrap = FlexWrap::NoWrap;
    AlignItems m_alignItems = AlignItems::Normal;
    AlignContent m_alignContent = AlignContent::Normal;
    mutable taffy::Engine m_engine;
    QSizeF m_cachedMin;
    QSizeF m_cachedPref;
    QSizeF m_cachedMax;
};
