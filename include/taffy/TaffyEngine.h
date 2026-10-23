#pragma once

#include <memory>

#include <taffy/TaffyTypes.h>

namespace taffy {

// Forward-declared to avoid leaking cxx headers into every consumer.
class EngineImpl;

/**
 * C++ facade over the Rust taffy engine (cxx). Qt-free except TaffyTypes enums.
 */
class Engine
{
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    quint32 addNode(const TaffyTypes* styleDefaults = nullptr);
    bool addChild(quint32 parent, quint32 child);
    bool setStyle(quint32 node, TaffyTypes::Display display,
                  TaffyTypes::FlexDirection flexDirection,
                  TaffyTypes::FlexWrap flexWrap,
                  TaffyTypes::AlignItemsKeyword alignItems,
                  TaffyTypes::AlignContentKeyword alignContent,
                  TaffyTypes::AlignContentKeyword justifyContent,
                  qreal width, qreal height,
                  qreal flexGrow = 0.0, qreal flexShrink = 1.0);
    bool compute(qreal availableWidth, qreal availableHeight);
    bool layout(quint32 node, qreal& x, qreal& y, qreal& w, qreal& h) const;
    void clear();

private:
    std::unique_ptr<EngineImpl> d;
};

} // namespace taffy
