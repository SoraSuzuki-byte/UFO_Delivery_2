/*
    @file   Item_Food_1.h
    @brief  食べ物アイテム1 のクラス
    @author 鈴木蒼良
    @date   2026年8月20日
*/
#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"

struct GameContext;
class Stage;
class Player;


class Item_Food_1
{
public:
    // 食べ物の種類
    enum class FoodType
    {
        Food1,
        Food2,
        Food3,
        Food4,
        Food5,
        Food6,
        Food7,
        Food8,
        Food9,
        Food10
    };
private:

    // 重力
    static constexpr const float GRAVITY = 9.8f;
    // プレイヤーに引き寄せられる速さ
    static constexpr const float ATTRACT_SPEED = 3.0f;

    //「真上にいる」とみなすX座標の許容範囲（左右何ピクセルまでOKか）
    static constexpr const float ABOVE_X_RANGE = 30.0f;

    // 落とした直後、再吸引を無効にする時間（フレーム数）
    static constexpr const int PICKUP_COOLDOWN_TIME = 30;   // 約0.5秒
    // 落とした後の、再吸引禁止のタイマー
    int m_pickupCooldownTimer;

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

    bool m_isHeld;   // 保有中かどうか



    float m_width;
    float m_height;

    // このインスタンスの、食べ物の種類
    FoodType m_foodType;

public:
    Item_Food_1(GameContext& gameContext, Stage& stage, Player* player, const BoundingBox& boundingBox, FoodType foodType);
    ~Item_Food_1();

    void Initialize();
    void Update();
    void Render() const;

    // 取得済みかどうか
    bool GetActiveFlag() const { return m_isActive; }
    void SetActiveFlag(bool isActive) { m_isActive = isActive; }

    // 保有中かどうか
    bool GetIsHeld() const { return m_isHeld; }

    // 種類を取得する
    FoodType GetFoodType() const { return m_foodType; }



    // 落とされた時の処理
    void Drop(const Vector2D& dropPosition)
    {
        m_isHeld = false;
        m_isPulled = false;
        m_position = dropPosition;
        m_velocity = Vector2D{ 0.0f, 0.0f };

        m_boundingBox.minPosition = m_position;
        m_boundingBox.maxPosition = Vector2D{ m_position.x + m_width, m_position.y + m_height };

        // 落とした直後は、しばらく再吸引を禁止する
        m_pickupCooldownTimer = PICKUP_COOLDOWN_TIME;
    }

    const BoundingBox& GetBoundingBox() const { return m_boundingBox; }
};
