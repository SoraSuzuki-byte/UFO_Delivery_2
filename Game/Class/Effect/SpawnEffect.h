/*
    @file   Explosion.h
    @brief  爆発アニメーションクラス
    @author 鈴木蒼良
    @date   2026年8月27日
*/
// 多重インクルードの防止 =====================================================
#pragma once

// ヘッダファイルの読み込み ===================================================
#include "Library/GameMath.h"

// 前方宣言 ===================================================================
struct GameContext;


// クラスの定義 ===============================================================
class SpawnEffect
{
// 定数や列挙型の宣言 ---------------------------------------
private:
    // UFOの大きさ（半分）
    static constexpr int PLAYER_HALF_SIZE_Y = 50;
    static constexpr int PLAYER_HALF_SIZE_X = 25;



    // 爆発アニメーションの状態
    enum class AnimationState
    {
        None = (-1),
        Anim0, Anim1, Anim2, Anim3, Anim4, Anim5, Anim6, Anim7, Anim8, Anim9
    };

    // アニメーションの切り替え間隔
    static constexpr int ANIMATION_INTERVAL = 2;

    // 爆発スプライトのテクスチャ上の大きさ
    static constexpr int SPRITE_SIZE = 180;

    // 描画のオフセット値
    static constexpr int OFFSET = 140;

    // 爆発スプライトの切り抜き位置
    static constexpr POINT EXPLOSION_SPRITES[10]{
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim0), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim1), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim2), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim3), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim4), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim5), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim6), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim7), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim8), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim9), 32 },
    };

// データメンバの宣言 -----------------------------------------------
private:
    // 描画位置
    float m_playerPositionX;
    float m_playerPositionY;

    // アニメーションステート
    AnimationState m_animationState;

    // アニメーションカウンタ
    int m_animationCounter;


// メンバ関数の宣言 -------------------------------------------------
// コンストラクタ/デストラクタ
public:
    SpawnEffect();
    ~SpawnEffect() = default;

// 操作
public:
    // 初期化処理
    void Initialize();

    // 更新処理
    void Update();

    // 描画処理
    void Render(int ghSTG) const;

    // 終了処理
    void Finalize();

    // 爆発開始処理
    void StartExplosion();

    // このオブジェクトを使用中か確認する処理
    bool IsActive() const;

    // 弾が衝突した敵の位置を得る
    void SetPlayerPosition(Vector2D playerPosition);
};
