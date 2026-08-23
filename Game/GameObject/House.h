/*
    @file   House.h
    @brief  ハウスクラス（このオブジェクトへ食べ物を届ける）
    @author 鈴木蒼良
    @date   2026年8月22日
*/
#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"
#include "Game/GameObject/Item_Food_1.h"
class GameContext;


class House
{
private:
    GameContext& m_gameContext;

    // 境界ボックス（ワールド座標の）
    BoundingBox m_boundingBox;


    // この家が欲しがっている食べ物の種類
    Item_Food_1::FoodType m_wantedFoodType;

    // すでに届け終わったかどうか（true = アイコン非表示）
    bool m_isFulfilled;

public:
    House(GameContext& gameContext, const BoundingBox& boundingBox, Item_Food_1::FoodType wantedFoodType);
    ~House();

    void Render() const;

    // 欲しがっている食べ物を、家の横に表示
    void RenderWantedItemIcon() const;


    const BoundingBox& GetBoundingBox() const { return m_boundingBox; }
    //「欲しがっている食べ物の種類」を取得する
    Item_Food_1::FoodType GetWantedFoodType() const { return m_wantedFoodType; }

    // 届け終わったかどうか
    bool GetIsFulfilled() const { return m_isFulfilled; }
    void SetIsFulfilled(bool isFulfilled) { m_isFulfilled = isFulfilled; }
};

