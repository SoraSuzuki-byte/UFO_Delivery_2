/*
    @file   ShootingStar.cpp
    @brief  流れ星アニメーションクラス
    @author 鈴木蒼良
    @date   2026年9月25日
*/
#include "ShootingStar.h"

//  -----------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// -----------------------------------------------------------------
ShootingStar::ShootingStar()
    : m_animationState{}
    , m_animationCounter{}
    , m_position{}
    , m_spawnCounter{}
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// -----------------------------------------------------------------
void ShootingStar::Initialize()
{
    m_animationCounter = 0;
    m_animationState = AnimationState::None;
    m_position.x = START_POS_X;
    m_position.y = START_POS_Y;
    m_spawnCounter = 0;
}

//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void ShootingStar::Update()
{
    if (IsActive())
    {
        // 移動
        m_position.x += MOVE_SPEED_X;
        m_position.y += MOVE_SPEED_Y;

        m_animationCounter++; // アニメーションカウンター
        if (m_animationCounter > ANIMATION_INTERVAL)
        {
            m_animationCounter = 0;

            if (m_animationState == AnimationState::Anim19)
            {
                m_animationState = AnimationState::None;// アニメーションを終了
            }
            else
            {
                //アニメーションを更新させる
                m_animationState = static_cast<AnimationState>(static_cast<int>(m_animationState) + 1);
            }
        }
        return;
    }
    // 非アクティブであれば出現までの時間を数える
    m_spawnCounter++;

    if (m_spawnCounter >= SPAWN_INTERVAL)
    {
        m_spawnCounter = 0;
        StartAnimation();
    }
}

//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// <param name="ghSTG">グラフィクスハンドル</param>
/// -----------------------------------------------------------------
void ShootingStar::Render(int ShootingStar) const
{
    // 非アクティブなら描画しない
    if (!IsActive()) return;

    // スプライトシート上の座標を計算する
    const int x = SHOOTING_STAR_SPRITES[static_cast<int>(m_animationState)].x;
    const int y = SHOOTING_STAR_SPRITES[static_cast<int>(m_animationState)].y;

    // 描画の位置
    const int centerX = static_cast<int>(m_position.x) ;
    const int centerY = static_cast<int>(m_position.y) ;


    DrawRectExtendGraph(
        centerX - drawHalfSize,
        centerY - drawHalfSize,
        centerX + drawHalfSize,
        centerY + drawHalfSize,
        x,
        y,
        SPRITE_SIZE,
        SPRITE_SIZE,
        ShootingStar,
        TRUE
    );
}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void ShootingStar::Finalize()
{
}

//  -----------------------------------------------------------------
/// <summary>
/// アニメーションの開始処理
/// </summary>
/// <param name="position">アニメーションの位置</param>
/// -----------------------------------------------------------------
void ShootingStar::StartAnimation()
{
    // 開始位置に戻す
    m_position.x = START_POS_X;
    m_position.y = START_POS_Y;

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
bool ShootingStar::IsActive() const
{
    // Nonnじゃなければ true が返る　（Noneだったら、falseを 返す）
    return m_animationState != AnimationState::None;
}