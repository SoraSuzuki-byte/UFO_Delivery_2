/*
    @file   CollisionAABB.h
    @brief  コリジョンヘッダ
    @author 制作者
    @date   2026/07/03
*/
#pragma once
#include "Library/GameMath.h"

// 境界ボックス
struct BoundingBox
{
    Vector2D minPosition;
    Vector2D maxPosition;
};

// 衝突方向を表すスコープ付き列挙型
enum class HitDirection
{
    None   = 0,         // 0
    Top    = 1 << 0,    // 1
    Right  = 1 << 1,    // 2
    Bottom = 1 << 2,    // 4
    Left   = 1 << 3     // 8
};

// ビット演算（| や &）を可能にするための型安全なヘルパー
inline HitDirection operator|(HitDirection lhs, HitDirection rhs)
{
    return static_cast<HitDirection>(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
}
inline HitDirection operator&(HitDirection lhs, HitDirection rhs)
{
    return static_cast<HitDirection>(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
}

// AABB
inline bool CheckHitAABB(const BoundingBox& lhs, const BoundingBox& rhs)
{
    // 衝突しない
    if (lhs.minPosition.x >= rhs.maxPosition.x) return false;
    if (rhs.minPosition.x >= lhs.maxPosition.x) return false;
    if (lhs.minPosition.y >= rhs.maxPosition.y) return false;
    if (rhs.minPosition.y >= lhs.maxPosition.y) return false;
    // 衝突した
    return true;
}

// 当たった方向を検知する
inline HitDirection CheckHitDirection(const BoundingBox& lhs, const BoundingBox& rhs)
{
    // 戻り値用変数
    HitDirection hitDirection = HitDirection::None;

    // 関数内の確認用フラグ（初期値は None）
    HitDirection checkFlag = HitDirection::None;

    // めり込み具合の確認用変数
    float checkXRatio = 0.0f;
    float checkYRatio = 0.0f;

    // 相手(プレイヤー）の幅と高さを計算
    float width  = rhs.maxPosition.x - rhs.minPosition.x;
    float height = rhs.maxPosition.y - rhs.minPosition.y;

    // 各方向のめり込み具合
    float topRatio    = (rhs.maxPosition.y - lhs.minPosition.y) / height;
    float bottomRatio = (lhs.maxPosition.y - rhs.minPosition.y) / height;
    float leftRatio   = (rhs.maxPosition.x - lhs.minPosition.x) / width;
    float rightRatio  = (lhs.maxPosition.x - rhs.minPosition.x) / width;

    // 上下の比率から小さい方を記憶
    if (topRatio < bottomRatio)
    {
        checkFlag = checkFlag | HitDirection::Top;
        checkYRatio = topRatio;
    }
    else
    {
        checkFlag = checkFlag | HitDirection::Bottom;
        checkYRatio = bottomRatio;
    }

    // 左右の比率から小さい方を記憶
    if (leftRatio < rightRatio)
    {
        checkFlag = checkFlag | HitDirection::Left;
        checkXRatio = leftRatio;
    }
    else
    {
        checkFlag = checkFlag | HitDirection::Right;
        checkXRatio = rightRatio;
    }

    // 上下と左右で比率の小さい方を戻り値とする
    // NOTE: 0.15fは補正値
    if (checkXRatio > checkYRatio + 0.15f)
    {
        // 上下のフラグを残す
        hitDirection = checkFlag & (HitDirection::Top | HitDirection::Bottom);
    }
    else if (checkXRatio < checkYRatio)
    {
        // 左右のフラグを残す
        hitDirection = checkFlag & (HitDirection::Left | HitDirection::Right);
    }

    return hitDirection;
}
