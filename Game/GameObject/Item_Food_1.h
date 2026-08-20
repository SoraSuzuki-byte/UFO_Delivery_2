/*
    @file   Item_Food_1.h
    @brief  食べ物アイテム1 のクラス
    @author 鈴木蒼良
    @date   2026年8月20日
*/
#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"

class GameContext;
class Stage;   // 前方宣言

class Item_Food_1
{
private:
    // 重力
    static constexpr const float GRAVITY = 9.8f;

    // 落下速度の上限（暴走防止）
    static constexpr const float MAX_FALL_SPEED = 20.0f;

    // 壁に「めり込む」割合（アイテムの高さに対する比率）
    static constexpr const float OVERLAP_RATIO = 0.5f;

    GameContext& m_gameContext;
    Stage& m_stage;

    // 境界ボックス（ワールド座標）
    BoundingBox m_boundingBox;

    // まだ取得されていないか
    bool m_isActive;

    // 現在の位置（境界ボックスの左上）
    Vector2D m_position;
    // 現在の移動量
    Vector2D m_velocity;

    float m_width;
    float m_height;

    // 着地済みかどうか
    bool m_isLanded;

public:
    Item_Food_1(GameContext& gameContext, Stage& stage, const BoundingBox& boundingBox);
    ~Item_Food_1();

    void Update();
    void Render() const;

    bool GetActiveFlag() const { return m_isActive; }
    void SetActiveFlag(bool isActive) { m_isActive = isActive; }

    const BoundingBox& GetBoundingBox() const { return m_boundingBox; }
};