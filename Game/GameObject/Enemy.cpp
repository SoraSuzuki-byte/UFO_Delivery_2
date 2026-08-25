/*
    @file   Enemy.cpp
    @brief  Enemyクラス
    @author 鈴木蒼良
    @date   2026年8月25日
*/
#include "pch.h"
#include "Enemy.h"
#include "Game/GameContext.h"
#include "Game/GameObject/Stage.h"
#include "Game/GameObject/Player.h"

Enemy::Enemy(GameContext& gameContext, Stage& stage, Player& player, const Vector2D& startPosition)
    : m_gameContext{ gameContext }
    , m_stage{ stage }
    , m_player{ player }
    , m_position{ startPosition }
    , m_isActive{ true }
{
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
    if (!m_isActive) { return; }

    // プレイヤーへ向かう方向を計算する
    const Vector2D playerPos = m_player.GetPosition();
    const Vector2D diff = playerPos - m_position;
    const Vector2D direction = Normalize(diff);

    // 一定速度でプレイヤーに向かって移動する
    m_position += direction * SPEED;

    // 壁（草）との当たり判定：通り抜けられないように押し戻す
    BoundingBox enemyBox = GetBoundingBox();
    const Vector2D correction = m_stage.ResolveWallCollision(enemyBox);
    m_position += correction;

    // プレイヤーとの当たり判定：触れたらダメージを与える
    if (CheckHitAABB(GetBoundingBox(), m_player.GetHurtBox()))   
    {
        m_player.TakeDamage();   
    }
}

void Enemy::Render() const
{
    if (!m_isActive) { return; }

    DrawGraph(
        static_cast<int>(m_position.x),
        static_cast<int>(m_position.y),
        m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Enemy_1),
        TRUE);
}
