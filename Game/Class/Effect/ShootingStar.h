/*
    @file   ShootingStar.h
    @brief  流れ星アニメーションクラス
    @author 鈴木蒼良
    @date   2026年9月25日
*/
// 多重インクルードの防止 =====================================================
#pragma once

// ヘッダファイルの読み込み ===================================================
#include "Library/GameMath.h"

// 前方宣言 ===================================================================
struct GameContext;


// クラスの定義 ===============================================================
class ShootingStar
{
// 定数や列挙型の宣言 ---------------------------------------
private:
    // 流れ星の移動速度
    static constexpr float MOVE_SPEED_X = -6.0f;
    static constexpr float MOVE_SPEED_Y = 6.5f;
    // 初期位置
    static constexpr int START_POS_X = 1100;
    static constexpr int START_POS_Y = 100;

    // 流れ星を出現させる間隔
    static constexpr int SPAWN_INTERVAL = 180;

    // 画面上で表示させる 大きさ
    static constexpr int drawHalfSize = 300;




    // アニメーションの状態
    enum class AnimationState
    {
        None = (-1),
        Anim0, Anim1, Anim2, Anim3, Anim4, Anim5, Anim6, Anim7, Anim8, Anim9, Anim10, Anim11, Anim12, Anim13, Anim14, Anim15, Anim16, Anim17, Anim18, Anim19
    };

    // アニメーションの切り替え間隔
    static constexpr int ANIMATION_INTERVAL = 2;

    // 爆発スプライトのテクスチャ上の大きさ
    static constexpr int SPRITE_SIZE = 480;

    // 描画のオフセット値
    static constexpr int OFFSET = 100;

    // 爆発スプライトの切り抜き位置
    static constexpr POINT SHOOTING_STAR_SPRITES[20]
    {
        { SPRITE_SIZE * 0, SPRITE_SIZE * 0 },
        { SPRITE_SIZE * 1, SPRITE_SIZE * 0 },
        { SPRITE_SIZE * 2, SPRITE_SIZE * 0 },
        { SPRITE_SIZE * 3, SPRITE_SIZE * 0 },
        { SPRITE_SIZE * 4, SPRITE_SIZE * 0 },

        { SPRITE_SIZE * 0, SPRITE_SIZE * 1 },
        { SPRITE_SIZE * 1, SPRITE_SIZE * 1 },
        { SPRITE_SIZE * 2, SPRITE_SIZE * 1 },
        { SPRITE_SIZE * 3, SPRITE_SIZE * 1 },
        { SPRITE_SIZE * 4, SPRITE_SIZE * 1 },

        { SPRITE_SIZE * 0, SPRITE_SIZE * 2 },
        { SPRITE_SIZE * 1, SPRITE_SIZE * 2 },
        { SPRITE_SIZE * 2, SPRITE_SIZE * 2 },
        { SPRITE_SIZE * 3, SPRITE_SIZE * 2 },
        { SPRITE_SIZE * 4, SPRITE_SIZE * 2 },

        { SPRITE_SIZE * 0, SPRITE_SIZE * 3 },
        { SPRITE_SIZE * 1, SPRITE_SIZE * 3 },
        { SPRITE_SIZE * 2, SPRITE_SIZE * 3 },
        { SPRITE_SIZE * 3, SPRITE_SIZE * 3 },
        { SPRITE_SIZE * 4, SPRITE_SIZE * 3 },
    };
// データメンバの宣言 -----------------------------------------------
private:
    // 描画位置
    Vector2D m_position;

    // アニメーションステート
    AnimationState m_animationState;

    // アニメーションカウンタ
    int m_animationCounter;

    // 流れ星の出現カウンター
    int m_spawnCounter;


// メンバ関数の宣言 -------------------------------------------------
// コンストラクタ/デストラクタ
public:
    ShootingStar();
    ~ShootingStar() = default;

// 操作
public:
    // 初期化処理
    void Initialize();

    // 更新処理
    void Update();

    // 描画処理
    void Render(int ShootingStar) const;

    // 終了処理
    void Finalize();

    // アニメーションの開始処理
    void StartAnimation();

    // このオブジェクトを使用中か確認する処理
    bool IsActive() const;
};
