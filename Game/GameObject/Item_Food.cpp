/*
    @file   Item_Food.cpp
    @brief  食べ物アイテム のクラス
    @author 鈴木蒼良
    @date   2026年8月20日
*/


#include "pch.h"
#include "Item_Food.h"
#include "Game/Screen.h"
#include "Game/GameContext.h"
#include "Game/GameObject/Stage.h"
#include "Game/GameObject/Player.h"

#include <random> //乱数に使う


// 放物線の初速レンジ
namespace
{
    constexpr float MIN_LAUNCH_SPEED_X = 4.0f;   // 横方向の最小初速
    constexpr float MAX_LAUNCH_SPEED_X = 7.0f;   // 横方向の最大初速
    constexpr float MIN_LAUNCH_SPEED_Y = 10.0f;  // 上方向の最小初速（絶対値）
    constexpr float MAX_LAUNCH_SPEED_Y = 14.0f;  // 上方向の最大初速（絶対値）
}

Item_Food::Item_Food(GameContext& gameContext, Stage* stage, Player* player, const BoundingBox& boundingBox, FoodType foodType)
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
    m_width = 50.0f;
    m_height =50.0f;
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
    //// キー入力情報を取得する
    //const int keyCondition = m_gameContext.inputManager.GetKeyCondition();
    //const int keyTrigger = m_gameContext.inputManager.GetKeyTrigger();
    if (!m_isActive) { return; }

    //----------------------------------------------------------------------------------
    // プレイヤーが自分の真上にいるかどうかを調べ、吸引フラグを更新
    //----------------------------------------------------------------------------------
    if (m_player != nullptr)// ★安全対策：Playerがまだ設定されていない場合 → elseへ
    {
        const Vector2D playerPos = m_player->GetPosition();
        const bool isAboveY = (playerPos.y < m_position.y); // この比較が true か false なのか

        // X座標が近いか
        const float diffX = playerPos.x - m_position.x; // diffX：プレイヤーのX座標とアイテムのX座標の差
        const bool isNearX = (diffX > -ABOVE_X_RANGE) && (diffX < ABOVE_X_RANGE);

        // 吸引の入力中かどうか
        const bool isPullingPressed = m_player->IsPullingInput();

        // 全ての条件を満たした時だけ「真上にいる」とみなす
        m_isPulled = {isAboveY && isNearX && isPullingPressed };

    }
    else { m_isPulled = false; }

    // ------------------------------------------------------------------
    // 吸引中：プレイヤーへ向かって直進し、近づいたらくっつく
    // ------------------------------------------------------------------
    if (m_isPulled)
{
    const Vector2D playerCenterPos = m_player->GetCenterPosition();

    // UFOの少し下の位置を「追いかける目標地点」にする
    // UFOの中心から、食べ物自身の当たり判定の半幅ぶん左にずらす
    const Vector2D targetPos = playerCenterPos + Vector2D{ 0.0f, HOLD_OFFSET_Y } - Vector2D{ m_width * 0.5f, 0.0f };

    const Vector2D diff = targetPos - m_position;
    const float distance = Length(diff);

    const float ATTRACT_SPEED = 3.0f;

    if (distance <= ATTRACT_SPEED)
    {
        // 残りわずかなら、行き過ぎないようにピタッと合わせる
        m_position = targetPos;
    }
    else
    {
        // 常にx/y同時に、正規化した方向で一定速度で近づく（爆弾と同じ考え方）
        const Vector2D direction = Normalize(diff);
        m_position += direction * ATTRACT_SPEED;
    }

    m_velocity = Vector2D{ 0.0f, 0.0f };
}
    else // 吸引されていないとき：重力の影響を受ける
    {
        if (m_stage == nullptr)
        {
            // Stageがない場面（タイトル画面）：簡易的な自由落下
            m_velocity.y += m_gravity;
            m_position += m_velocity;

            // 指定の地面Yに到達したら停止させる
            if (m_hasDemoGround && (m_position.y + m_height >= m_demoGroundY))
            {
                m_position.y = m_demoGroundY - m_height;
                m_velocity = Vector2D{ 0.0f, 0.0f };
            }
        }
        else
        {
            m_velocity.y += m_gravity;
            m_position += m_velocity;

            // 足元が壁かどうかを調べる
            const float centerX = m_position.x + m_width * 0.5f;
            const float footY = m_position.y + m_height;

            // マップ外への落下対策
            const float mapBottom = static_cast<float>(m_stage->GetMapHeight() * m_stage->GetChipSize());
            if (footY > mapBottom)
            {
                m_isActive = false;
                return;
            }

            const Stage::Type footType = m_stage->GetChipType(Vector2D{ centerX, footY });

            if (footType == Stage::Type::Wall)
            {
                const POINT mapPos = m_stage->ConvertWorldPositionToMapPosition(Vector2D{ centerX, footY });
                const float wallTopY = static_cast<float>(mapPos.y * m_stage->GetChipSize());

                m_position.y = wallTopY - m_height;

                // 摩擦の影響（横方向の速度を減衰）
                m_velocity.x *= m_friction;

                // 反発の影響（跳ね返り）
                m_velocity.y *= -m_restitution;
            }
        }
    }

    // 食べ物が 画面外へ出ないように位置を修正する
    ClampPositionToScreen();

    // ------------------------------------------------------------------
   // 境界ボックスを、現在位置に合わせて更新する
   // ------------------------------------------------------------------
    m_boundingBox.minPosition = m_position;
    m_boundingBox.maxPosition = Vector2D{ m_position.x + m_width, m_position.y + m_height };

}


void Item_Food::Render() const
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





// 放物線を描いて飛んでいく 
void Item_Food::LaunchInRandomDiagonalDirection()
{
    // 乱数生成器（関数が呼ばれるたびに再構築しないよう static に）
    static std::mt19937 rng{ std::random_device{}() };
    static std::uniform_int_distribution<int> dirDist(0, 1);              // 0:左上 1:右上
    static std::uniform_real_distribution<float> speedXDist(MIN_LAUNCH_SPEED_X, MAX_LAUNCH_SPEED_X);
    static std::uniform_real_distribution<float> speedYDist(MIN_LAUNCH_SPEED_Y, MAX_LAUNCH_SPEED_Y);

    const int direction = dirDist(rng);
    const float speedX = speedXDist(rng);
    const float speedY = speedYDist(rng);

    // 左上なら-x、右上なら+x
    m_velocity.x = (direction == 0) ? -speedX : speedX;
    // 上方向は-yなのでマイナスを付ける
    m_velocity.y = -speedY;

    // 吸引状態や重力での挙動と競合しないようリセットしておく
    m_isPulled = false;
}



// 飛んでいく向きを ランダムに決める
void Item_Food::Launch(const Vector2D& launchPosition)
{
    m_position = launchPosition;
    m_isActive = true;
    m_isPulled = false;

    LaunchInRandomDiagonalDirection(); // ここで毎回ランダムに再設定される

    m_boundingBox.minPosition = m_position;
    m_boundingBox.maxPosition = Vector2D{ m_position.x + m_width, m_position.y + m_height };
}








// 食べ物が 画面外へ出ないように位置を修正する
void Item_Food::ClampPositionToScreen()
{
    if (m_position.x < 0.0f) { m_position.x = 0.0f; }
    if (m_position.x + m_width > Screen::WIDTH) { m_position.x = Screen::WIDTH - m_width; }
    if (m_position.y < 0.0f) { m_position.y = 0.0f; }
    if (m_position.y + m_height > Screen::HEIGHT) { m_position.y = Screen::HEIGHT - m_height; }
}

