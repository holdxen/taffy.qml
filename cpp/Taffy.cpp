#include "taffy/Taffy.h"
#include "taffy/TaffyItem.h"
#include "taffy/TaffyLayout.h"

#include <QtQml/qqml.h>

namespace taffy {

void registerQmlTypes()
{
    qmlRegisterType<TaffyItem>("Taffy", 1, 0, "TaffyItem");
    qmlRegisterType<TaffyLayout>("Taffy", 1, 0, "TaffyLayout");
}

} // namespace taffy
