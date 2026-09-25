/*
    @file   SpawnEffect.h
    @brief  スポーンアニメーションクラス
    @author 鈴木蒼良
    @date   2026年9月24日
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
    static constexpr int PLAYER_HALF_SIZE_Y = 25;
    static constexpr int PLAYER_HALF_SIZE_X = 25;



    // アニメーションの状態
    enum class AnimationState
    {
        None = (-1),
        Anim0, Anim1, Anim2, Anim3, Anim4, Anim5, Anim6, Anim7, Anim8, Anim9, Anim10, Anim11, Anim12, Anim13
    };

    // アニメーションの切り替え間隔
    static constexpr int ANIMATION_INTERVAL = 4;

    // スプライトのテクスチャ上の大きさ
    static constexpr int SPRITE_SIZE = 240;

    // 描画のオフセット値
    static constexpr int OFFSET = 100;

    // スプライトの切り抜き位置
    static constexpr POINT SPAWN_EFFECT_SPRITES[14]{
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
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim10), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim11), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim12), 32 },
        {SPRITE_SIZE * static_cast<int>(AnimationState::Anim13), 32 },
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
    void Render(int spawnEffect) const;

    // 終了処理
    void Finalize();

    // アニメーションの開始処理
    void StartAnimation();

    // このオブジェクトを使用中か確認する処理
    bool IsActive() const;

    // 弾が衝突した敵の位置を得る
    void SetPlayerPosition(Vector2D playerPosition);
};
