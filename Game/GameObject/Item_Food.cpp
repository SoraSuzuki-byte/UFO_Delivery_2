/*
    @file   Item_Food_1.cpp
    @brief  食べ物アイテム のクラス
    @author 鈴木蒼良
    @date   2026年8月20日
*/


#include "pch.h"
#include "Item_Food.h"
#include "Game/GameContext.h"
#include "Game/GameObject/Stage.h"
#include "Game/GameObject/Player.h"

Item_Food::Item_Food(GameContext& gameContext, Stage& stage, Player* player, const BoundingBox& boundingBox, FoodType foodType)
    : m_gameContext{ gameContext }
    , m_stage{ stage }
    , m_player{ player }
    , m_boundingBox{ boundingBox }
    , m_isActive{ true }
    , m_position{}
    , m_velocity{}
    , m_gravity{}
    , m_restitution{}
    , m_friction{}
    , m_isPulled{ false }
    , m_foodType{ foodType }
    , m_width{ boundingBox.maxPosition.x - boundingBox.minPosition.x }
    , m_height{ boundingBox.maxPosition.y - boundingBox.minPosition.y }

{
}

Item_Food::~Item_Food()
{
}

void Item_Food::Initialize()
{
    m_position = m_boundingBox.minPosition;
    // 速度を初期化
    m_velocity = Vector2D{ 0.0f, 0.0f };

    // 重力加速度の設定
    m_gravity = (GRAVITY * 100.0f) / (60.0f * 60.0f);
    // 摩擦係数の 設定
    m_friction = 0.98f;
    // 反発係数の設定
    m_restitution = 0.5f;

    m_isActive = true;
    m_isPulled = false;

}

void Item_Food::Update()
{
    // キー入力情報を取得する
    const int keyCondition = m_gameContext.inputManager.GetKeyCondition();
    const int keyTrigger = m_gameContext.inputManager.GetKeyTrigger();


    if (!m_isActive) { return; }

    //----------------------------------------------------------------------------------
    // プレイヤーが自分の真上にいるかどうかを調べ、吸引フラグを更新
    //----------------------------------------------------------------------------------
    if (m_player != nullptr)// ★安全対策：Playerがまだ設定されていない場合 → elseへ
    {
        const Vector2D playerPos = m_player->GetPosition();
        const bool isAboveY = (playerPos.y < m_position.y); // 右側の比較が true か false なのか

        // X座標が近いか
        const float diffX = playerPos.x - m_position.x; // diffX：プレイヤーのX座標とアイテムのX座標の差
        const bool isNearX = (diffX > -ABOVE_X_RANGE) && (diffX < ABOVE_X_RANGE);

        // 両方の条件を満たした時だけ「真上にいる」とみなす
        m_isPulled = isAboveY && isNearX;

    }
    else { m_isPulled = false; }

    // ------------------------------------------------------------------
    // 吸引中 かつ スペースキーを押していれば：プレイヤーへ向かって直進する
    // ------------------------------------------------------------------
    if (m_isPulled && (keyCondition & PAD_INPUT_10))
    {
        const Vector2D playerPos = m_player->GetPosition();
        const Vector2D diff = playerPos - m_position;
        const float distance = Length(diff);

        // 十分近づいたら、連れている状態に切り替える
        const float HOLD_DISTANCE = 20.0f;
        if (distance < HOLD_DISTANCE)
        {
            // 近づいた時の処理を、ここに書く
        }
        else { m_isPulled = false; }

        const Vector2D direction = Normalize(diff);

        const float ATTRACT_SPEED = 3.0f;
        m_position += direction * ATTRACT_SPEED;

        m_velocity = Vector2D{ 0.0f, 0.0f };
    }
    else // 吸引されていないとき：重力の影響を受ける
    {
        m_velocity.y += m_gravity;
        m_position += m_velocity;

        // 足元が壁（草）かどうかを調べる
        const float centerX = m_position.x + m_width * 0.5f;
        const float footY = m_position.y + m_height;

        // マップ外への落下対策
        const float mapBottom = static_cast<float>(m_stage.GetMapHeight() * m_stage.GetChipSize());
        if (footY > mapBottom)
        {
            m_isActive = false;
            return;
        }

        const Stage::Type footType = m_stage.GetChipType(Vector2D{ centerX, footY });

        if (footType == Stage::Type::Wall)
        {
            const POINT mapPos = m_stage.ConvertWorldPositionToMapPosition(Vector2D{ centerX, footY });
            const float wallTopY = static_cast<float>(mapPos.y * m_stage.GetChipSize());

            m_position.y = wallTopY - m_height;

            // 摩擦の影響（横方向の速度を減衰）
            m_velocity.x *= m_friction;

            // 反発の影響（跳ね返り）
            m_velocity.y *= -m_restitution;
        }
    }
    // ------------------------------------------------------------------
   // 境界ボックスを、現在位置に合わせて更新する
   // ------------------------------------------------------------------
    m_boundingBox.minPosition = m_position;
    m_boundingBox.maxPosition = Vector2D{ m_position.x + m_width, m_position.y + m_height };

}


void Item_Food::Render() const//-----------------------------------------------------
{

    if (!m_isActive) { return; }


    GhManager::Textures texture{}; //  GhManager::Textures型の、空のオブジェクトを宣言

    // タイプに合わせて、空のオブジェクトに代入
    switch (m_foodType) 
{
    case FoodType::Food1: texture = GhManager::Textures::Item_Food_1; break;
    case FoodType::Food2: texture = GhManager::Textures::Item_Food_2; break;
    case FoodType::Food3: texture = GhManager::Textures::Item_Food_3; break;
    case FoodType::Food4: texture = GhManager::Textures::Item_Food_4; break;
    case FoodType::Food5: texture = GhManager::Textures::Item_Food_5; break;
    default:
        // 想定外のタイプに対するエラーハンドリング
        break;
}
    DrawGraph(
        static_cast<int>(m_boundingBox.minPosition.x),
        static_cast<int>(m_boundingBox.minPosition.y),
        m_gameContext.ghManager.GetGraphicHandle(texture),
        TRUE);
}