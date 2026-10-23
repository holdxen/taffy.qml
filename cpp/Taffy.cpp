#include "taffy/Taffy.h"
#include "taffy/TaffyLayout.h"

#include <QtQml/qqml.h>

namespace taffy {

void registerQmlTypes()
{
    qmlRegisterType<TaffyLayout>("Taffy", 1, 0, "TaffyLayout");
}

} // namespace taffy
