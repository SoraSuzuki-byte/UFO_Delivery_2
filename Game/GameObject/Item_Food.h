/*
    @file   Item_Food.h
    @brief  食べ物アイテム のクラス
    @author 鈴木蒼良
    @date   2026年8月20日
*/
#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"

struct GameContext;
class Stage;
class Player;


class Item_Food
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
        Food6
    };
private:
    // 重力
    static constexpr const float GRAVITY = 9.8f;
    // プレイヤーに引き寄せられる速さ
    static constexpr const float ATTRACT_SPEED = 3.0f;

    //「真上にいる」とみなすX座標の許容範囲（左右何ピクセルまでOKか）
    static constexpr const float ABOVE_X_RANGE = 30.0f;

    static constexpr float HOLD_OFFSET_Y = 5.0f; // 吸引中の描画オフセット
   

    // Stageがない場面（タイトル画面など）で使う、着地とみなすY座標
    float m_demoGroundY = 0.0f;
    bool  m_hasDemoGround = false;// タイトルシーンの重力フラグ


    GameContext& m_gameContext;
    Stage* m_stage;
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

    // このインスタンスの、食べ物の種類
    FoodType m_foodType;

public:
    // stage は nullptr でも良い（例：タイトル画面など、床判定が不要な場面）
    Item_Food(GameContext& gameContext, Stage* stage, Player* player,
        const BoundingBox& boundingBox, FoodType foodType);
    ~Item_Food();

    void Initialize();
    void Update();
    void Render() const;

    void SetStage(Stage* stage) { m_stage = stage; }

    // 取得済みかどうか
    bool GetActiveFlag() const { return m_isActive; }
    void SetActiveFlag(bool isActive) { m_isActive = isActive; }


    // 種類を取得する
    FoodType GetFoodType() const { return m_foodType; }

    // 吸引されて付いてきているかどうかを取得する
    bool IsPulled() const { return m_isPulled; }


    // 落とされた時の処理
    void Drop(const Vector2D& dropPosition)
    {
        m_isPulled = false;
        m_position = dropPosition;
        m_velocity = Vector2D{ 0.0f, 0.0f };

        m_boundingBox.minPosition = m_position;
        m_boundingBox.maxPosition = Vector2D{ m_position.x + m_width, m_position.y + m_height };

    }

    // 当たり判定ボックスを取得
    const BoundingBox& GetBoundingBox() const { return m_boundingBox; }


    // 放物線を描いて飛んでいく
    void LaunchInRandomDiagonalDirection();
    // 飛んでいく向きを ランダムに再設定
    void Launch(const Vector2D& launchPosition);


    // 食べ物が 画面外へ出ないように位置を修正する
    void ClampPositionToScreen();



    // タイトル画面などStageがない場面で「ここまで落ちたら止まる」地面のY座標を設定する
        void SetDemoGroundY(float groundY)
    {
        m_demoGroundY = groundY;
        m_hasDemoGround = true;
    }
};
