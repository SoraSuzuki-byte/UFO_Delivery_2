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

class Item_Food_1
{
private:
    GameContext& m_gameContext;

    // 境界ボックス（ワールド座標）
    BoundingBox m_boundingBox;

    // まだ取得されていないか（true = 表示・当たり判定あり）
    bool m_isActive;

public:
    Item_Food_1(GameContext& gameContext, const BoundingBox& boundingBox);
    ~Item_Food_1();

    void Render() const;

    // 取得済みかどうか
    bool GetActiveFlag() const { return m_isActive; }
    void SetActiveFlag(bool isActive) { m_isActive = isActive; }

    const BoundingBox& GetBoundingBox() const { return m_boundingBox; }
};
