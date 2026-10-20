#pragma once

#include <QtQml/qqml.h>

namespace taffy {

/**
 * Register taffy QML types under URI "Taffy" 1.0.
 * Call once before loading any QML that uses them.
 */
void registerQmlTypes();

} // namespace taffy
