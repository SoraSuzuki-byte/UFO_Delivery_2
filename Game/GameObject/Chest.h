/*
    @file   Chest.h
    @brief  宝箱クラス
    @author 制作者
    @date   2026/07/03
*/
#pragma once
#include "Game/CollisionAABB.h"

class Chest
{
private:
    // データメンバ
    BoundingBox m_boundingBox;  // 境界ボックス
    bool        m_isActive;     // 有効化フラグ

public:
    // コンストラクター、デストラクター
    Chest(BoundingBox bb, bool isActive);
    ~Chest() = default;

    // 描画処理
    void Render(int scroll) const;

    // Getter/Setter
    bool GetActiveFlag() const { return m_isActive; }
    void SetActiveFlag(bool flag) { m_isActive = flag; }
    const BoundingBox& GetBoundingBox() const { return m_boundingBox; }
};