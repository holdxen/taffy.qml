//
// Created by zhouguiqing on 2026/10/20.
//



#pragma once


#include <QtQml/qqmlintegration.h>

#include <QObject>


class TaffyAttached : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    using QObject::QObject;
};