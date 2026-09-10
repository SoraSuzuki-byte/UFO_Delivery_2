/*
    @file   Enemy2.cpp
    @brief  Enemy2クラス
    @author 鈴木蒼良
    @date   2026年9月1日
*/
#include "pch.h"
#include "Enemy2.h"
#include "Game/GameContext.h"
#include "Game/GameObject/Stage.h"
#include "Game/GameObject/Player.h"

Enemy2::Enemy2(GameContext& gameContext, Stage& stage, Player& player, const Vector2D& startPosition)
    : m_gameContext{ gameContext }
    , m_stage{ stage }
    , m_player{ player }
    , m_position{ startPosition }
    , m_isActive{ true }
    , m_degree{}
{
}

Enemy2::~Enemy2()
{
}

void Enemy2::Update()
{
    if (!m_isActive) { return; }

    // 動きの処理
    Move();

    // 当たり判定
    ProcessCollision();
}

void Enemy2::Render() const
{
    if (!m_isActive) { return; }

    const Vector2D playerPos = m_player.GetPosition();
    if (m_position.x < playerPos.x)
    {
        DrawGraph(
            static_cast<int>(m_position.x),
            static_cast<int>(m_position.y),
            m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Enemy_2_1),
            TRUE);
    }
    else
    {
        DrawGraph(
            static_cast<int>(m_position.x),
            static_cast<int>(m_position.y),
            m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Enemy_2),
            TRUE);
    }


    // 2. 当たり判定（BoundingBox）の可視化処理（デバッグ用）
    // =================================================================
    const BoundingBox box = GetBoundingBox();

    const int left = static_cast<int>(box.minPosition.x);
    const int top = static_cast<int>(box.minPosition.y);
    const int right = static_cast<int>(box.maxPosition.x);
    const int bottom = static_cast<int>(box.maxPosition.y);

    // 赤色の枠線を描画 (FALSE = 枠線のみ)
    DrawBox(left, top, right, bottom, GetColor(255, 0, 0), FALSE);
}



// 動きの処理
void Enemy2::Move()
{
    // 1. プレイヤーへ向かう基本ベクトル（正規化）
    const Vector2D playerPos = m_player.GetPosition();
    const Vector2D diff = playerPos - m_position;
    const Vector2D direction = Normalize(diff);

    // 2. 進行方向に対して垂直なベクトル（右方向の垂直ベクトル）を作る
    // Vector2D が (x, y) の場合、(-y, x) は垂直なベクトルになります
    const Vector2D sideDirection = { -direction.y, direction.x };

    // 3. 横揺れの角度と移動量を計算
    m_degree += 5.0f;
    const float wave = sin(m_degree * DX_PI_F / 180.0f) * 3.0f; // 振れ幅 3.0

    // 4. 「基本移動（前進）」＋「垂直方向の揺れ」を元の座標に加算する
    m_position += (direction * SPEED) + (sideDirection * wave);


    // -- 横のみに揺れる場合↓ -------------------------------
    //// 1. プレイヤーへ向かう基本ベクトル（正規化）
    //const Vector2D playerPos = m_player.GetPosition();
    //const Vector2D diff = playerPos - m_position;
    //const Vector2D direction = Normalize(diff);

    //// 2. まずプレイヤーに向かって基本移動させる
    //m_position += direction * SPEED;

    //// 3. 横揺れ（正弦波）の計算
    //m_degree += 5.0f;
    //const float wave = sin(m_degree * DX_PI_F / 180.0f) * 3.0f; // 振れ幅 3.0

    //// 4. 画面の左右（X座標）にだけ揺れを加算する（Y軸は動かさない）
    //m_position.x += wave;

}




// 当たり判定
void Enemy2::ProcessCollision()
{
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
