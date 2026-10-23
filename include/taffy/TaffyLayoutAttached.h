#pragma once


#include <QObject>

#include <taffy/TaffyTypes.h>

/**
 * Item-side taffy::Style fields as attached properties of TaffyLayout.
 * QML uses the TaffyLayout name (not Layout):
 *
 *   TaffyLayout {
 *       Rectangle {
 *           TaffyLayout.width: 100
 *           TaffyLayout.flexGrow: 1
 *       }
 *   }
 */
class TaffyLayoutAttached : public QObject
{
    Q_OBJECT
    // size / constraints (NaN = auto)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal minWidth READ minWidth WRITE setMinWidth NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal minHeight READ minHeight WRITE setMinHeight NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal maxWidth READ maxWidth WRITE setMaxWidth NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal maxHeight READ maxHeight WRITE setMaxHeight NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY itemStyleChanged)
    // flex item
    Q_PROPERTY(qreal flexBasis READ flexBasis WRITE setFlexBasis NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal flexGrow READ flexGrow WRITE setFlexGrow NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal flexShrink READ flexShrink WRITE setFlexShrink NOTIFY itemStyleChanged)
    // alignment override
    Q_PROPERTY(bool hasAlignSelf READ hasAlignSelf WRITE setHasAlignSelf NOTIFY itemStyleChanged)
    Q_PROPERTY(AlignItems alignSelf READ alignSelf WRITE setAlignSelf NOTIFY itemStyleChanged)
    Q_PROPERTY(bool hasJustifySelf READ hasJustifySelf WRITE setHasJustifySelf NOTIFY itemStyleChanged)
    Q_PROPERTY(AlignItems justifySelf READ justifySelf WRITE setJustifySelf NOTIFY itemStyleChanged)
    // box
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
    // position
    Q_PROPERTY(Position position READ position WRITE setPosition NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal insetTop READ insetTop WRITE setInsetTop NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal insetRight READ insetRight WRITE setInsetRight NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal insetBottom READ insetBottom WRITE setInsetBottom NOTIFY itemStyleChanged)
    Q_PROPERTY(qreal insetLeft READ insetLeft WRITE setInsetLeft NOTIFY itemStyleChanged)

public:
    explicit TaffyLayoutAttached(QObject* parent = nullptr);

    qreal width() const;
    void setWidth(qreal v);
    qreal height() const;
    void setHeight(qreal v);
    qreal minWidth() const;
    void setMinWidth(qreal v);
    qreal minHeight() const;
    void setMinHeight(qreal v);
    qreal maxWidth() const;
    void setMaxWidth(qreal v);
    qreal maxHeight() const;
    void setMaxHeight(qreal v);
    qreal aspectRatio() const;
    void setAspectRatio(qreal v);

    qreal flexBasis() const;
    void setFlexBasis(qreal v);
    qreal flexGrow() const;
    void setFlexGrow(qreal v);
    qreal flexShrink() const;
    void setFlexShrink(qreal v);

    bool hasAlignSelf() const;
    void setHasAlignSelf(bool v);
    AlignItems alignSelf() const;
    void setAlignSelf(AlignItems v);
    bool hasJustifySelf() const;
    void setHasJustifySelf(bool v);
    AlignItems justifySelf() const;
    void setJustifySelf(AlignItems v);

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

    Position position() const;
    void setPosition(Position v);
    qreal insetTop() const;
    void setInsetTop(qreal v);
    qreal insetRight() const;
    void setInsetRight(qreal v);
    qreal insetBottom() const;
    void setInsetBottom(qreal v);
    qreal insetLeft() const;
    void setInsetLeft(qreal v);

signals:
    void itemStyleChanged();

private:
    qreal m_width = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_height = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_minWidth = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_minHeight = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_maxWidth = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_maxHeight = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_aspectRatio = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_flexBasis = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_flexGrow = 0.0;
    qreal m_flexShrink = 1.0;
    bool m_hasAlignSelf = false;
    AlignItems m_alignSelf = AlignItems::Normal;
    bool m_hasJustifySelf = false;
    AlignItems m_justifySelf = AlignItems::Normal;
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
    Position m_position = Position::Static;
    qreal m_insetTop = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_insetRight = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_insetBottom = std::numeric_limits<qreal>::quiet_NaN();
    qreal m_insetLeft = std::numeric_limits<qreal>::quiet_NaN();
};

