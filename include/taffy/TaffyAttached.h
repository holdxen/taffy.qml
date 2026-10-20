#pragma once

#include <QObject>
// QML_ELEMENT etc. Live in QtQml on this Qt install.
// Note: <QtQml/qqmlintegration.h> is NOT in the QtQml framework (it's QtQmlIntegration).
#include <QtQml/qqmlregistration.h>

class TaffyAttached : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    using QObject::QObject;
};
