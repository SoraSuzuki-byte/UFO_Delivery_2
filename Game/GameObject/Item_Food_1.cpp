/*
    @file   Item_Food_1.cpp
    @brief  食べ物アイテム1 のクラス
    @author 鈴木蒼良
    @date   2026年8月20日
*/
#include "pch.h"
#include "Item_Food_1.h"
#include "Game/GameContext.h"
#include "Game/GameObject/Stage.h"

Item_Food_1::Item_Food_1(GameContext& gameContext, Stage& stage, const BoundingBox& boundingBox)
    : m_gameContext{ gameContext }
    , m_stage{ stage }
    , m_boundingBox{ boundingBox }
    , m_isActive{ true }
    , m_position{ boundingBox.minPosition }
    , m_velocity{ 0.0f, 0.0f }
    , m_width{ boundingBox.maxPosition.x - boundingBox.minPosition.x }
    , m_height{ boundingBox.maxPosition.y - boundingBox.minPosition.y }
    , m_isLanded{ false }
{
}

Item_Food_1::~Item_Food_1()
{
}

void Item_Food_1::Update()
{
    if (!m_isActive) { return; }
    if (m_isLanded) { return; }

    // 重力を加える
    m_velocity.y += GRAVITY;
    if (m_velocity.y > MAX_FALL_SPEED)
    {
        m_velocity.y = MAX_FALL_SPEED;
    }

    // 座標の更新
    m_position.y += m_velocity.y;

    const float centerX = m_position.x + m_width * 0.5f;
    const float footY = m_position.y + m_height;

    // ★安全対策：マップの縦幅を超えて落ち続けたら、非表示にして処理を止める
    const float mapBottom = static_cast<float>(m_stage.GetMapHeight() * m_stage.GetChipSize());
    if (footY > mapBottom)
    {
        m_isActive = false;
        return;
    }

    // 足元のマスが壁かどうかを調べる
    const Stage::Type footType = m_stage.GetChipType(Vector2D{ centerX, footY });

    if (footType == Stage::Type::Wall)
    {
        const POINT mapPos = m_stage.ConvertWorldPositionToMapPosition(Vector2D{ centerX, footY });
        const float wallTopY = static_cast<float>(mapPos.y * m_stage.GetChipSize());

        const float overlapAmount = m_height * OVERLAP_RATIO;
        m_position.y = wallTopY - m_height + overlapAmount;

        m_velocity.y = 0.0f;
        m_isLanded = true;
    }

    // 境界ボックスを、現在位置に合わせて更新する
    m_boundingBox.minPosition = m_position;
    m_boundingBox.maxPosition = Vector2D{ m_position.x + m_width, m_position.y + m_height };
}

void Item_Food_1::Render() const
{
    if (!m_isActive) { return; }

    DrawGraph(
        static_cast<int>(m_position.x),
        static_cast<int>(m_position.y),
        m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Item_Food_1),
        TRUE);
}