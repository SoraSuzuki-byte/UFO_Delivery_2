/*
    @file   Stage.h
    @brief  プレイヤークラス
    @author 鈴木蒼良
    @date   2026年8月16日
*/
#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"
#include <vector>



// 前方宣言 ===============================================================
struct GameContext;
class PlayScene;
class Item_Food_1;


class Player
{
private:
    static constexpr const float SPEED = 0.05f;// 速度
    static constexpr const float WIDTH = 51;  // 横幅
    static constexpr const float HEIGHT = 26; // 縦幅

    static constexpr float ACCEL = 0.27f;  // 加速度（大きくするとクイックに動き出す）
    static constexpr float FRICTION = 0.94f; // 摩擦係数（1に近いほどよく滑る。0.85〜0.95あたりがおすすめ）
    static constexpr float MAX_SPEED = 10.0f;  // 最高速度

    static constexpr float BOUNCE_FACTOR = 1.2f;  // 跳ね返りの強さ（1.0で等倍、大きくすると強く跳ね返る）

    static constexpr int INVINCIBLE_TIME = 60;  // 無敵時間
    static constexpr int BLINK_INTERVAL = 2; // 点滅の速さ(間隔）
    static constexpr int MAX_HP = 5;  // 最大HP

    static constexpr const int MAX_HOLD_COUNT = 5;   // 保有できる最大数

    std::vector<Item_Food_1*> m_heldItems;   // 保有しているアイテムのリスト
    int m_selectedItemIndex;                  // 現在選択中のインデックス


    // ゲームコンテキストのリファレンス　（インスタンス・実体ではない
    GameContext& m_gameContext;

    // ステージクラスのリファレンス
    PlayScene& m_scene;          


    // 現在の位置
    Vector2D m_position;
    // 現在の移動量
    Vector2D m_velocity;
    // 現在の加速度
    Vector2D m_acceleration;

    // 移動のアニメーションに使うTimer
    int m_animationTimer;
    // 現在のHP
    int m_hp;
     // 無敵タイマー（0より大きい間は無敵）
    int m_invincibleTimer;    



public:
    Player(GameContext& gameContext, PlayScene& scene);
    ~Player();

    void Initialize();
    void Update();
    void Render();
    void Finalize();

    // HPのゲッター　プレイシーンで「HPが0になったらリザルト表示」をするため
    int GetHp() const { return m_hp; }
    // m_positionのゲッター
    Vector2D GetPosition() const { return m_position; }

    // UFOを移動させる
    void Move(int keyCondition);

    // 移動時のアニメーション
    void MoveAnimation();

    // ダメージ表現のオーバーレイ
    void DrawDamageOverlay();

    // ItemFood_1と当たると
    void CheckItemFood_1Collision(BoundingBox playerBox);

    // 保有しているアイテムのリストを取得する（表示用）
    const std::vector<Item_Food_1*>& GetHeldItems() const { return m_heldItems; }

    // アイテムの選択切り替え・ドロップ操作
    void UpdateItemHolding(int keyCondition, int keyTrigger);

    // 現在選択中のインデックスを取得する
    int GetSelectedItemIndex() const { return m_selectedItemIndex; }

    // アイテムを保有する（上限に達していたら失敗してfalseを返す）
    bool TryHoldItem(Item_Food_1* item)
    {
        if (static_cast<int>(m_heldItems.size()) >= MAX_HOLD_COUNT) { return false; }
        m_heldItems.push_back(item);
        return true;
    }
};

