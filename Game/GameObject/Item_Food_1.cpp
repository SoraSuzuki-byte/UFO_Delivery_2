#include "pch.h"
#include "Item_Food_1.h"
#include "Game/GameContext.h"

Item_Food_1::Item_Food_1(GameContext& gameContext, const BoundingBox& boundingBox)
    : m_gameContext{ gameContext }
    , m_boundingBox{ boundingBox }
    , m_isActive{ true }
{
}

Item_Food_1::~Item_Food_1()
{
}

void Item_Food_1::Render() const
{
    if (!m_isActive) { return; }

    DrawGraph(
        static_cast<int>(m_boundingBox.minPosition.x),
        static_cast<int>(m_boundingBox.minPosition.y),
        m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Item_Food_1),  // ←仮の名前
        TRUE);
}