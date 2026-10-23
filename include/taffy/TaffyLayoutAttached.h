#pragma once

#include <limits>

#include <QObject>
#include <QString>
#include <QVariantMap>

/**
 * Item-side style as attached properties of TaffyLayout.
 *
 *   TaffyLayout {
 *       Rectangle {
 *           TaffyLayout.width: "100px"
 *           TaffyLayout.flexGrow: 1
 *       }
 *   }
 *
 * CSS-like strings: "auto", "100px", "50%", "min-content"
 */
class TaffyLayoutAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString width READ width WRITE setWidth NOTIFY itemStyleChanged)
    Q_PROPERTY(QString height READ height WRITE setHeight NOTIFY itemStyleChanged)
    Q_PROPERTY(QString minWidth READ minWidth WRITE setMinWidth NOTIFY itemStyleChanged)
    Q_PROPERTY(QString minHeight READ minHeight WRITE setMinHeight NOTIFY itemStyleChanged)
    Q_PROPERTY(QString maxWidth READ maxWidth WRITE setMaxWidth NOTIFY itemStyleChanged)
    Q_PROPERTY(QString maxHeight READ maxHeight WRITE setMaxHeight NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY itemStyleChanged)
    Q_PROPERTY(QString flexBasis READ flexBasis WRITE setFlexBasis NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal flexGrow READ flexGrow WRITE setFlexGrow NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal flexShrink READ flexShrink WRITE setFlexShrink NOTIFY itemStyleChanged)
    Q_PROPERTY(QString alignSelf READ alignSelf WRITE setAlignSelf NOTIFY itemStyleChanged)
    Q_PROPERTY(QString justifySelf READ justifySelf WRITE setJustifySelf NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal marginTop READ marginTop WRITE setMarginTop NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal marginRight READ marginRight WRITE setMarginRight NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal marginBottom READ marginBottom WRITE setMarginBottom NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal marginLeft READ marginLeft WRITE setMarginLeft NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal paddingTop READ paddingTop WRITE setPaddingTop NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal paddingRight READ paddingRight WRITE setPaddingRight NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal paddingBottom READ paddingBottom WRITE setPaddingBottom NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal paddingLeft READ paddingLeft WRITE setPaddingLeft NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal borderTop READ borderTop WRITE setBorderTop NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal borderRight READ borderRight WRITE setBorderRight NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal borderBottom READ borderBottom WRITE setBorderBottom NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal borderLeft READ borderLeft WRITE setBorderLeft NOTIFY itemStyleChanged)
    Q_PROPERTY(QString position READ position WRITE setPosition NOTIFY itemStyleChanged)
    Q_PROPERTY(QString gridRow READ gridRow WRITE setGridRow NOTIFY itemStyleChanged)
    Q_PROPERTY(QString gridColumn READ gridColumn WRITE setGridColumn NOTIFY itemStyleChanged)
    Q_PROPERTY(QString boxSizing READ boxSizing WRITE setBoxSizing NOTIFY itemStyleChanged)
    Q_PROPERTY(QString overflowX READ overflowX WRITE setOverflowX NOTIFY itemStyleChanged)
    Q_PROPERTY(QString overflowY READ overflowY WRITE setOverflowY NOTIFY itemStyleChanged)
    Q_PROPERTY(QString cssFloat READ cssFloat WRITE setCssFloat NOTIFY itemStyleChanged)
    Q_PROPERTY(QString clear READ clear WRITE setClear NOTIFY itemStyleChanged)
    Q_PROPERTY(QString textAlign READ textAlign WRITE setTextAlign NOTIFY itemStyleChanged)
    Q_PROPERTY(bool itemIsTable READ itemIsTable WRITE setItemIsTable NOTIFY itemStyleChanged)
    Q_PROPERTY(bool itemIsReplaced READ itemIsReplaced WRITE setItemIsReplaced NOTIFY itemStyleChanged)
    Q_PROPERTY(bool itemIsCompressibleReplaced READ itemIsCompressibleReplaced WRITE setItemIsCompressibleReplaced NOTIFY itemStyleChanged)
    Q_PROPERTY(QString insetTop READ insetTop WRITE setInsetTop NOTIFY itemStyleChanged)
    Q_PROPERTY(QString insetRight READ insetRight WRITE setInsetRight NOTIFY itemStyleChanged)
    Q_PROPERTY(QString insetBottom READ insetBottom WRITE setInsetBottom NOTIFY itemStyleChanged)
    Q_PROPERTY(QString insetLeft READ insetLeft WRITE setInsetLeft NOTIFY itemStyleChanged)
    // Debug: all taffy-related QML properties, via QMetaObject reflection
    Q_PROPERTY(QVariantMap taffyStyle READ taffyStyle NOTIFY itemStyleChanged)

public:
    explicit TaffyLayoutAttached(QObject* parent = nullptr);

    QString width() const;
    void setWidth(const QString& v);
    QString height() const;
    void setHeight(const QString& v);
    QString minWidth() const;
    void setMinWidth(const QString& v);
    QString minHeight() const;
    void setMinHeight(const QString& v);
    QString maxWidth() const;
    void setMaxWidth(const QString& v);
    QString maxHeight() const;
    void setMaxHeight(const QString& v);
    qreal aspectRatio() const;
    void setAspectRatio(qreal v);

    QString flexBasis() const;
    void setFlexBasis(const QString& v);
    qreal flexGrow() const;
    void setFlexGrow(qreal v);
    qreal flexShrink() const;
    void setFlexShrink(qreal v);

    QString alignSelf() const;
    void setAlignSelf(const QString& v);
    QString justifySelf() const;
    void setJustifySelf(const QString& v);

    qreal marginTop() const;
    void setMarginTop(qreal v);
    qreal marginRight() const;
    void setMarginRight(qreal v);
    qreal marginBottom() const;
    void setMarginBottom(qreal v);
    qreal marginLeft() const;
    void setMarginLeft(qreal v);

    qreal paddingTop() const;
    void setPaddingTop(qreal v);
    qreal paddingRight() const;
    void setPaddingRight(qreal v);
    qreal paddingBottom() const;
    void setPaddingBottom(qreal v);
    qreal paddingLeft() const;
    void setPaddingLeft(qreal v);

    qreal borderTop() const;
    void setBorderTop(qreal v);
    qreal borderRight() const;
    void setBorderRight(qreal v);
    qreal borderBottom() const;
    void setBorderBottom(qreal v);
    qreal borderLeft() const;
    void setBorderLeft(qreal v);

    QString position() const;
    void setPosition(const QString& v);
    QString gridRow() const;
    void setGridRow(const QString& v);
    QString gridColumn() const;
    void setGridColumn(const QString& v);
    QString boxSizing() const;
    void setBoxSizing(const QString& v);
    QString overflowX() const;
    void setOverflowX(const QString& v);
    QString overflowY() const;
    void setOverflowY(const QString& v);
    QString cssFloat() const;
    void setCssFloat(const QString& v);
    QString clear() const;
    void setClear(const QString& v);
    QString textAlign() const;
    void setTextAlign(const QString& v);
    bool itemIsTable() const;
    void setItemIsTable(bool v);
    bool itemIsReplaced() const;
    void setItemIsReplaced(bool v);
    bool itemIsCompressibleReplaced() const;
    void setItemIsCompressibleReplaced(bool v);
    QString insetTop() const;
    void setInsetTop(const QString& v);
    QString insetRight() const;
    void setInsetRight(const QString& v);
    QString insetBottom() const;
    void setInsetBottom(const QString& v);
    QString insetLeft() const;
    void setInsetLeft(const QString& v);

    /** Reflect all Q_PROPERTY values into a map (taffyStyle). */
    QVariantMap taffyStyle() const;
    /** Human-readable dump of all taffy-related properties. */
    Q_INVOKABLE QString dump() const;

signals:
    void itemStyleChanged();

private:
    QString m_width = QStringLiteral("auto");
    QString m_height = QStringLiteral("auto");
    QString m_minWidth = QStringLiteral("auto");
    QString m_minHeight = QStringLiteral("auto");
    QString m_maxWidth = QStringLiteral("auto");
    QString m_maxHeight = QStringLiteral("auto");
    qreal m_aspectRatio = std::numeric_limits<qreal>::quiet_NaN();
    QString m_flexBasis = QStringLiteral("auto");
    qreal m_flexGrow = 0.0;
    qreal m_flexShrink = 1.0;
    QString m_alignSelf; // empty or "auto" = unset
    QString m_justifySelf; // empty or "auto" = unset
    qreal m_marginTop = 0.0;
    qreal m_marginRight = 0.0;
    qreal m_marginBottom = 0.0;
    qreal m_marginLeft = 0.0;
    qreal m_paddingTop = 0.0;
    qreal m_paddingRight = 0.0;
    qreal m_paddingBottom = 0.0;
    qreal m_paddingLeft = 0.0;
    qreal m_borderTop = 0.0;
    qreal m_borderRight = 0.0;
    qreal m_borderBottom = 0.0;
    qreal m_borderLeft = 0.0;
    QString m_position = QStringLiteral("static");
    QString m_gridRow = QStringLiteral("auto");
    QString m_gridColumn = QStringLiteral("auto");
    QString m_boxSizing = QStringLiteral("border-box");
    QString m_overflowX = QStringLiteral("visible");
    QString m_overflowY = QStringLiteral("visible");
    QString m_cssFloat = QStringLiteral("none");
    QString m_clear = QStringLiteral("none");
    QString m_textAlign = QStringLiteral("auto");
    bool m_itemIsTable = false;
    bool m_itemIsReplaced = false;
    bool m_itemIsCompressibleReplaced = false;
    QString m_insetTop = QStringLiteral("auto");
    QString m_insetRight = QStringLiteral("auto");
    QString m_insetBottom = QStringLiteral("auto");
    QString m_insetLeft = QStringLiteral("auto");
};
