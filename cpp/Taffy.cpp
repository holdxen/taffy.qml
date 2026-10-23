#include "taffy/Taffy.h"
#include "taffy/TaffyItem.h"
#include "taffy/TaffyTypes.h"

#include <QtQml/qqml.h>

namespace taffy {

void registerQmlTypes()
{
    qmlRegisterUncreatableType<TaffyTypes>("Taffy", 1, 0, "TaffyTypes",
                                           QStringLiteral("enum holder"));
    qmlRegisterType<TaffyItem>("Taffy", 1, 0, "TaffyItem");
}

} // namespace taffy
