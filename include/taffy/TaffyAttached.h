#pragma once

#include <QObject>
// QML_ELEMENT etc. Live in QtQml on this Qt install.
// Note: <QtQml/qqmlintegration.h> is NOT in the QtQml framework (it's QtQmlIntegration).
#include <QtQml/qqmlregistration.h>

#include <taffy_rust/src/utils.cxxqt.h>

class Taffy : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_ATTACHED(TaffyAttached)

public:
    using QObject::QObject;

    static TaffyAttached* qmlAttachedProperties(QObject* target)
    {
        auto* attached = new TaffyAttached();
        attached->setParent(target);
        return attached;
    }
};
