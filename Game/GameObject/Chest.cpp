/*
    @file   Chest.cpp
    @brief  宝箱クラス
    @author 制作者
    @date   2026/07/03
*/
#include "pch.h"
#include "Chest.h"

// ------------------------------------------------------------------
// コンストラクター
// ------------------------------------------------------------------
Chest::Chest(BoundingBox bb, bool isActive)
    : m_boundingBox{ bb }
    , m_isActive{ isActive }
{
}

// ------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------
void Chest::Render(int scroll) const
{
    // スクロール量を考慮して描画する
    DrawBox(
        static_cast<int>(m_boundingBox.minPosition.x - scroll), // 始点
        static_cast<int>(m_boundingBox.minPosition.y),
        static_cast<int>(m_boundingBox.maxPosition.x - scroll), // 終点
        static_cast<int>(m_boundingBox.maxPosition.y),
        Colors::YELLOW, TRUE
    );
}
