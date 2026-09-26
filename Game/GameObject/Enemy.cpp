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

    const Vector2D playerPos = m_player.GetPosition();

    GhManager::Textures texture;
    if (m_gameContext.isOtherStarSystem)
    {
        texture = GhManager::Textures::Enemy1_OtherStarSystem;
    }
    else if ((m_gameContext.isOtherStarSystem) && (m_position.x < playerPos.x))
    {
        texture = GhManager::Textures::Enemy1_1_OtherStarSystem;
    }
    else if (m_position.x < playerPos.x)
    {
        texture = GhManager::Textures::Enemy_1_1;
    }
    else
    {
        texture = GhManager::Textures::Enemy_1;
    }

    DrawGraph(
        static_cast<int>(m_position.x),
        static_cast<int>(m_position.y),
        m_gameContext.ghManager.GetGraphicHandle(texture),
        TRUE);


    //// 2. 当たり判定（BoundingBox）の可視化処理（デバッグ用）
    //// =================================================================
    //const BoundingBox box = GetBoundingBox();

    //const int left = static_cast<int>(box.minPosition.x);
    //const int top = static_cast<int>(box.minPosition.y);
    //const int right = static_cast<int>(box.maxPosition.x);
    //const int bottom = static_cast<int>(box.maxPosition.y);

    //// 赤色の枠線を描画 (FALSE = 枠線のみ)
    //DrawBox(left, top, right, bottom, GetColor(255, 0, 0), FALSE);
}
