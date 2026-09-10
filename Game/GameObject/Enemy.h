/*
    @file   Enemy.h
    @brief  Enemyクラス
    @author 鈴木蒼良
    @date   2026年8月25日
*/
#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"

struct GameContext;
class Stage;
class Player; 

class Enemy
{
private:
    // 移動速度
    static constexpr const float SPEED = 1.0f;

    // 見た目・当たり判定のサイズ
    static constexpr const float WIDTH = 64.0f;
    static constexpr const float HEIGHT = 74.0f;

    GameContext& m_gameContext;
    Stage& m_stage;
    Player& m_player;

    Vector2D m_position;
    bool m_isActive;

public:
    Enemy(GameContext& gameContext, Stage& stage, Player& player, const Vector2D& startPosition);
    ~Enemy();

    void Update();
    void Render() const;

    bool GetActiveFlag() const { return m_isActive; }
    void SetActiveFlag(bool isActive) { m_isActive = isActive; }

    Vector2D GetPosition() const { return m_position; }
    Vector2D GetCenterPosition() const
    {
        return Vector2D{ m_position.x + WIDTH / 2.0f, m_position.y + HEIGHT / 2.0f };
    }

    // 当たり判定用の境界ボックスを取得する
    BoundingBox GetBoundingBox() const
    {
        return BoundingBox{
            m_position,
            Vector2D{ m_position.x + WIDTH, m_position.y + HEIGHT }
        };
    }
};

