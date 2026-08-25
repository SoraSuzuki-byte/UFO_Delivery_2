#include "pch.h"
#include "Item_Food_1.h"
#include "Game/GameContext.h"
#include "Game/GameObject/Stage.h"
#include "Game/GameObject/Player.h"

Item_Food_1::Item_Food_1(GameContext& gameContext, Stage& stage, Player* player, const BoundingBox& boundingBox, FoodType foodType)
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
    , m_isHeld{ false }
    , m_pickupCooldownTimer{}
    , m_foodType{ foodType }
    , m_width{ boundingBox.maxPosition.x - boundingBox.minPosition.x }
    , m_height{ boundingBox.maxPosition.y - boundingBox.minPosition.y }

{
}

Item_Food_1::~Item_Food_1()
{
}

void Item_Food_1::Initialize()
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

    m_isPulled = false;

}

void Item_Food_1::Update()
{
    if (!m_isActive) { return; }
    if (m_isHeld)
    {
        return;   // 座標更新は Player 側（PlayScene::Render）で行うので、ここでは何もしない
    }
    // 落とした後のクールダウン中はタイマーを減らし、吸引判定を行わない
    if (m_pickupCooldownTimer > 0)
    {
        m_pickupCooldownTimer--;
    }
    //----------------------------------------------------------------------------------
    // プレイヤーが自分の真上にいるかどうかを調べ、吸引フラグを更新
    //----------------------------------------------------------------------------------
    if (m_player != nullptr && m_pickupCooldownTimer <= 0)// ★安全対策：Playerがまだ設定されていない場合かつ、吸引クールダウン中でない時 → elseへ
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
    // 吸引中：プレイヤーへ向かって直進する
    // ------------------------------------------------------------------
    if (m_isPulled)
    {
        const Vector2D playerPos = m_player->GetPosition();
        const Vector2D diff = playerPos - m_position;
        const float distance = Length(diff);

        // 十分近づいたら、保有状態に切り替える
        const float HOLD_DISTANCE = 20.0f;
        if (distance < HOLD_DISTANCE)
        {
            if (m_player->TryHoldItem(this))
            {
                m_isHeld = true;
                m_isPulled = false;
            }
            return;   // 保有した場合は、これ以降の移動処理をスキップ
        }

        const Vector2D direction = Normalize(diff);

        const float ATTRACT_SPEED = 3.0f;
        m_position += direction * ATTRACT_SPEED;

        m_velocity = Vector2D{ 0.0f, 0.0f };
    }
    else // 吸引されていないとき：重力の影響を受ける
    {
        m_velocity.y += m_gravity;
        m_position += m_velocity;

        // 家との当たり判定
        for (auto& house : m_stage.GetHouses())
        {
            if (CheckHitAABB(m_boundingBox, house.GetBoundingBox()))
            {
                if (house.GetIsFulfilled()) { continue; }   // 届け済みの家はもう判定しない

                // 食べ物の種類が、家と一致した時だけ命中とする
                if (house.GetWantedFoodType() == m_foodType)
                {
                    m_isActive = false;       // 食べ物は消える
                    house.SetIsFulfilled(true);   // 家の付近にある、欲しい食べ物のアイコンを消す
                    return;

                }
                // 種類が違う場合は何もしない（すり抜けてそのまま落下し続ける）
            }
        }

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

            const float overlapAmount = m_height * OVERLAP_RATIO;
            m_position.y = wallTopY - m_height + overlapAmount;

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


void Item_Food_1::Render() const//-----------------------------------------------------
{

    if (!m_isActive) { return; }
    if (m_isHeld) { return; }   // 保有中はマップ上に描画しない（左上UIは別途PlayScene側で描画）


    GhManager::Textures texture{}; //  GhManager::Textures型の、空のオブジェクトを宣言

    // タイプに合わせて、空のオブジェクトに代入
    switch (m_foodType) 
{
    case FoodType::Food1: texture = GhManager::Textures::Item_Food_1; break;
    case FoodType::Food2: texture = GhManager::Textures::Item_Food_2; break;
    case FoodType::Food3: texture = GhManager::Textures::Item_Food_3; break;
    case FoodType::Food4: texture = GhManager::Textures::Item_Food_4; break;
    case FoodType::Food5: texture = GhManager::Textures::Item_Food_5; break;
    case FoodType::Food6: texture = GhManager::Textures::Item_Food_6; break;
    case FoodType::Food7: texture = GhManager::Textures::Item_Food_7; break;
    case FoodType::Food8: texture = GhManager::Textures::Item_Food_8; break;
    case FoodType::Food9: texture = GhManager::Textures::Item_Food_9; break;
    case FoodType::Food10: texture = GhManager::Textures::Item_Food_10; break;
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