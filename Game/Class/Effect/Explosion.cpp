/*
    @file   Explosion.cpp
    @brief  爆発アニメーションクラス
    @author 鈴木蒼良
    @date   2026年8月27日
*/
#include "Explosion.h"

//  -----------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// -----------------------------------------------------------------
Explosion::Explosion()
    : m_animationState{}
    , m_animationCounter{}
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// -----------------------------------------------------------------
void Explosion::Initialize()
{
    m_animationCounter = 0;
    m_animationState = AnimationState::None;
}

//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void Explosion::Update()
{
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
void Explosion::Render(int explosion) const
{
    // スプライトシート上の座標を計算する
    const int x = EXPLOSION_SPRITES[static_cast<int>(m_animationState)].x;
    const int y = EXPLOSION_SPRITES[static_cast<int>(m_animationState)].y;

    // 爆発の描画
    DrawRectExtendGraph(
        static_cast<int>(m_enemyPositionX) - OFFSET, static_cast<int>(m_enemyPositionY) - OFFSET,   // 描画位置の始点
        static_cast<int>(m_enemyPositionX) + OFFSET, static_cast<int>(m_enemyPositionY) + OFFSET,   // 描画位置の終点
        x, y, SPRITE_SIZE, SPRITE_SIZE,                 // テクスチャーの切り抜き位置
        explosion, TRUE
    );
}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void Explosion::Finalize()
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 爆発開始処理
/// </summary>
/// <param name="position">爆発位置</param>
/// -----------------------------------------------------------------
void Explosion::StartExplosion()
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
bool Explosion::IsActive() const
{
    // Nonnじゃなければ true が返る　（Noneだったら、falseを 返す）
    return m_animationState != AnimationState::None;
}

void Explosion::SetEnemyPosition(Vector2D enemyPosition)
{
    m_enemyPositionX = enemyPosition.x;
    m_enemyPositionY = enemyPosition.y;
}
