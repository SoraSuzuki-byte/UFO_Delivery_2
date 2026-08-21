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
class Stage;
class Player;


class Item_Food_1
{
private:

    // 重力
    static constexpr const float GRAVITY = 9.8f;
    // プレイヤーに引き寄せられる速さ
    static constexpr const float ATTRACT_SPEED = 3.0f;
    //「真上にいる」とみなすX座標の許容範囲（左右何ピクセルまでOKか）
    static constexpr const float ABOVE_X_RANGE = 30.0f;

    // 壁（草）への めり込み割合
    static constexpr const float OVERLAP_RATIO = 0.5f;

    GameContext& m_gameContext;
    Stage& m_stage;
    Player* m_player;

    // 境界ボックス（ワールド座標）
    BoundingBox m_boundingBox;

    // まだ取得されていないか（true = 表示・当たり判定あり）
    bool m_isActive;

    // 現在の位置
    Vector2D m_position;
    // 現在の移動量
    Vector2D m_velocity;

    // 重力
    float m_gravity;
    // 反発係数
    float m_restitution;
    // 摩擦係数
    float m_friction;
    // 吸引されているか
    bool m_isPulled;


    float m_width;
    float m_height;

public:
    Item_Food_1(GameContext& gameContext, Stage& stage, Player* player, const BoundingBox& boundingBox);
    ~Item_Food_1();

    void Initialize();
    void Update();
    void Render() const;

    // 取得済みかどうか
    bool GetActiveFlag() const { return m_isActive; }
    void SetActiveFlag(bool isActive) { m_isActive = isActive; }

    const BoundingBox& GetBoundingBox() const { return m_boundingBox; }
};
