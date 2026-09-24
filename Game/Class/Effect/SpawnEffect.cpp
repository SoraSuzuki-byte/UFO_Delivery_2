/*
    @file   Explosion.cpp
    @brief  爆発アニメーションクラス
    @author 鈴木蒼良
    @date   2026年8月27日
*/
#include "SpawnEffect.h"

//  -----------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// -----------------------------------------------------------------
SpawnEffect::SpawnEffect()
    : m_animationState{}
    , m_animationCounter{}
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// -----------------------------------------------------------------
void SpawnEffect::Initialize()
{
    m_animationCounter = 0;
    m_animationState = AnimationState::None;
}

//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void SpawnEffect::Update()
{
    // 非アクティブなら何もしない
    if (!IsActive()) return;
   
    /*
        ・「m_animationCounter」「ANIMATION_INTERVAL」「m_animationState」
        　を使用して、爆発アニメーションの切り替え部分を実装している
    */

    m_animationCounter++; // アニメーションカウンターを更新している

    if (m_animationCounter > ANIMATION_INTERVAL)
    {
        m_animationCounter = 0; //アニメーションカウンターを0にする

        if (m_animationState == AnimationState::Anim9)// 最後の爆発のイラストだったら
        {
            m_animationState = AnimationState::None;// アニメーションを終了
        }
        else {
            //アニメーションを更新させる
            m_animationState = static_cast<AnimationState>(static_cast<int>(m_animationState) + 1);
        }
     }
}

//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// <param name="ghSTG">グラフィクスハンドル</param>
/// -----------------------------------------------------------------
void SpawnEffect::Render(int spawnEffect) const
{
    // 非アクティブなら描画しない
    if (!IsActive()) return;

    // スプライトシート上の座標を計算する
    const int x = EXPLOSION_SPRITES[static_cast<int>(m_animationState)].x;
    const int y = EXPLOSION_SPRITES[static_cast<int>(m_animationState)].y;

    const int centerX = static_cast<int>(m_playerPositionX) + PLAYER_HALF_SIZE_X;
    const int centerY = static_cast<int>(m_playerPositionY) ;

    const int effectHalfSize = SPRITE_SIZE / 2;

    DrawRectExtendGraph(
        centerX - effectHalfSize,
        centerY - effectHalfSize,
        centerX + effectHalfSize,
        centerY + effectHalfSize,
        x,
        0,
        SPRITE_SIZE,
        SPRITE_SIZE,
        spawnEffect,
        TRUE
    );
}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void SpawnEffect::Finalize()
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 爆発開始処理
/// </summary>
/// <param name="position">爆発位置</param>
/// -----------------------------------------------------------------
void SpawnEffect::StartExplosion()
{
    // アニメーション用パラメータを初期化する
    m_animationCounter = 0;
    m_animationState = AnimationState::Anim0;
}

//  -----------------------------------------------------------------
/// <summary>
/// このオブジェクトを使用中か確認する処理</summary>
/// 
/// <returns>true：使用中、false：待機中</returns>
/// -----------------------------------------------------------------
bool SpawnEffect::IsActive() const
{
    // Nonnじゃなければ true が返る　（Noneだったら、falseを 返す）
    return m_animationState != AnimationState::None;
}

void SpawnEffect::SetPlayerPosition(Vector2D playerPosition)
{
    m_playerPositionX = playerPosition.x;
    m_playerPositionY = playerPosition.y;
}
