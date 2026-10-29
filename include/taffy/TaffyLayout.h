#pragma once

#include <QStringList>

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
    Q_PROPERTY(qreal gapWidth READ gapWidth WRITE setGapWidth RESET resetGapWidth NOTIFY styleChanged)
    Q_PROPERTY(qreal gapHeight READ gapHeight WRITE setGapHeight RESET resetGapHeight NOTIFY styleChanged)
    Q_PROPERTY(QString gridAutoFlow READ gridAutoFlow WRITE setGridAutoFlow NOTIFY styleChanged)
    Q_PROPERTY(QString gridTemplateRows READ gridTemplateRows WRITE setGridTemplateRows NOTIFY styleChanged)
    Q_PROPERTY(QString gridTemplateColumns READ gridTemplateColumns WRITE setGridTemplateColumns NOTIFY styleChanged)
    Q_PROPERTY(QString gridAutoRows READ gridAutoRows WRITE setGridAutoRows NOTIFY styleChanged)
    Q_PROPERTY(QString gridAutoColumns READ gridAutoColumns WRITE setGridAutoColumns NOTIFY styleChanged)
    Q_PROPERTY(QString gridTemplateAreas READ gridTemplateAreas WRITE setGridTemplateAreas NOTIFY styleChanged)
    Q_PROPERTY(QStringList gridTemplateRowNames READ gridTemplateRowNames WRITE setGridTemplateRowNames NOTIFY styleChanged)
    Q_PROPERTY(QStringList gridTemplateColumnNames READ gridTemplateColumnNames WRITE setGridTemplateColumnNames NOTIFY styleChanged)
    Q_PROPERTY(QString boxSizing READ boxSizing WRITE setBoxSizing NOTIFY styleChanged)
    Q_PROPERTY(QString overflowX READ overflowX WRITE setOverflowX NOTIFY styleChanged)
    Q_PROPERTY(QString overflowY READ overflowY WRITE setOverflowY NOTIFY styleChanged)
    Q_PROPERTY(qreal scrollbarWidth READ scrollbarWidth WRITE setScrollbarWidth NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingTop READ paddingTop WRITE setPaddingTop NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingRight READ paddingRight WRITE setPaddingRight NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingBottom READ paddingBottom WRITE setPaddingBottom NOTIFY styleChanged)
    Q_PROPERTY(qreal paddingLeft READ paddingLeft WRITE setPaddingLeft NOTIFY styleChanged)
    Q_PROPERTY(QString direction READ direction WRITE setDirection NOTIFY styleChanged)
    // Debug: TaffyLayout's own style properties, via QMetaObject reflection
    Q_PROPERTY(QVariantMap taffyStyle READ taffyStyle NOTIFY styleChanged)

public:
    explicit TaffyLayout(QQuickItem* parent = nullptr);
    ~TaffyLayout() override;

    static TaffyLayoutAttached* qmlAttachedProperties(QObject* object);
    /** Reflect all own taffy Q_PROPERTY values. */
    QVariantMap taffyStyle() const;
    /** Human-readable dump of this layout's taffy properties. */
    Q_INVOKABLE QString dump() const;
    /** Dump TaffyLayout attached properties of every child (debug). */
    Q_INVOKABLE QString dumpTree() const;

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
    void resetGapWidth();
    qreal gapHeight() const;
    void setGapHeight(qreal v);
    void resetGapHeight();
    QString gridAutoFlow() const;
    void setGridAutoFlow(const QString& v);
    QString gridTemplateRows() const;
    void setGridTemplateRows(const QString& v);
    QString gridTemplateColumns() const;
    void setGridTemplateColumns(const QString& v);
    QString gridAutoRows() const;
    void setGridAutoRows(const QString& v);
    QString gridAutoColumns() const;
    void setGridAutoColumns(const QString& v);
    QString gridTemplateAreas() const;
    void setGridTemplateAreas(const QString& v);
    QStringList gridTemplateRowNames() const;
    void setGridTemplateRowNames(const QStringList& v);
    QStringList gridTemplateColumnNames() const;
    void setGridTemplateColumnNames(const QStringList& v);
    QString boxSizing() const;
    void setBoxSizing(const QString& v);
    QString overflowX() const;
    void setOverflowX(const QString& v);
    QString overflowY() const;
    void setOverflowY(const QString& v);
    qreal scrollbarWidth() const;
    void setScrollbarWidth(qreal v);
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

public slots:
    /** Drop cached size hints and re-run layout with the current size. */
    void relayout();

public:
    void componentComplete() override;
    void geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry) override;
    void invalidate(QQuickItem* childItem = nullptr) override;
    QSizeF sizeHint(Qt::SizeHint whichSizeHint) const override;
    void setAlignment(QQuickItem* item, Qt::Alignment align) override;
    void setStretchFactor(QQuickItem* item, int stretchFactor, Qt::Orientation orient) override;
    void updateLayoutItems() override;
    void rearrange(const QSizeF& size) override;
    QQuickItem* itemAt(int index) const override;
    int itemCount() const override;

signals:
    void styleChanged();

public:
    void itemChange(ItemChange change, const ItemChangeData& value) override;
    void itemSiblingOrderChanged(QQuickItem* item) override;

private:
    Q_DECLARE_PRIVATE(TaffyLayout)

    taffy::Style containerStyle(const QSizeF& size) const;
    taffy::Style itemStyle(QQuickItem* child) const;
    void watchItemStyle(QQuickItem* child) const;
    /** Sync implicitWidth/Height (and Layout min/max) like QQuickLayout::applySizeHints. */
    void applySizeHintsToItem();
    void onItemStyleChanged();
    void onChildImplicitSizeChanged();
    static void checkItemAnchors(QQuickItem* item);
    void applyChildGeometry(QQuickItem* child, const taffy::Layout& layout);

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
    QString m_gridTemplateRows;
    QString m_gridTemplateColumns;
    QString m_gridAutoRows;
    QString m_gridAutoColumns;
    QString m_gridTemplateAreas;
    QStringList m_gridTemplateRowNames;
    QStringList m_gridTemplateColumnNames;
    QString m_boxSizing = QStringLiteral("border-box");
    QString m_overflowX = QStringLiteral("visible");
    QString m_overflowY = QStringLiteral("visible");
    qreal m_scrollbarWidth = 0.0;
    qreal m_paddingTop = 0.0;
    qreal m_paddingRight = 0.0;
    qreal m_paddingBottom = 0.0;
    qreal m_paddingLeft = 0.0;
    QString m_direction = QStringLiteral("ltr");
    mutable taffy::Engine m_engine;
    mutable bool m_sizeHintsDirty = true;
    mutable QSizeF m_cachedSizeHints[Qt::NSizeHints];
};
