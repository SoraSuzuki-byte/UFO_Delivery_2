/*
    @file   Stage.h
    @brief  プレイヤークラス
    @author 鈴木蒼良
    @date   2026年8月16日
*/


#include "pch.h"
#include "Player.h"
#include "Game/Screen.h"

#include "Game/GameContext.h"
#include "Game/Class/Scene/PlayScene.h"



Player::Player(GameContext& gameContext, PlayScene& scene)
    : m_gameContext{gameContext}
    , m_scene{scene}
    , m_position{}
	, m_velocity{}
	, m_acceleration{}
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	// 初期位置を設定
    m_position = m_scene.GetStage().GetPlayerStartPosition();
	// 速度を初期化
	m_velocity = Vector2D{ 0.0f, 0.0f };
	// 加速度を初期化
	m_acceleration = Vector2D{ 0.0f, 0.0f };

}

void Player::Update()
{
	Move();
}

void Player::Render()
{
    // UFOの素体
    DrawGraph(m_position.x, m_position.y,
              m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Bass),
              TRUE);

    // UFOのHPがわかるようにオーバーレイ
    DrawGraph(m_position.x, m_position.y,
        m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Damage_Overlay),
        TRUE);

}
   

void Player::Finalize()
{
}

// UFOを動かす
void Player::Move()
{
	// キー入力情報を取得する
	const int keyCondition = m_gameContext.inputManager.GetKeyCondition();
	const int keyTrigger = m_gameContext.inputManager.GetKeyTrigger();


	Vector2D input = { 0.0f,0.0f };//キー入力用の「入力方向ベクトル」を作成

	if (keyCondition & PAD_INPUT_UP)    input.y -= 1.0f;
	if (keyCondition & PAD_INPUT_DOWN)  input.y += 1.0f;
	if (keyCondition & PAD_INPUT_LEFT)  input.x -= 1.0f;
	if (keyCondition & PAD_INPUT_RIGHT) input.x += 1.0f;

	// 入力がない時(0,0)は Normalize(input) も (0,0) になる
	m_velocity += Normalize(input) * ACCEL;

	// 摩擦（減衰）処理 
	m_velocity.x *= FRICTION;
	m_velocity.y *= FRICTION;

	// 最高速度の制限
	if ((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y) > MAX_SPEED * MAX_SPEED)   //if (Length(m_velocity) > MAX_SPEED) { ←これだと「sqrt」を使っており平方根で計算していて、処理が重たい
	{
		// 速度を正規化したあと、最大速度を かけ算
		m_velocity = Normalize(m_velocity) * MAX_SPEED;
	}

	// 座標の更新
	m_position += m_velocity;


	// 自機が画面外へ出ないように位置を修正する
	if (m_position.x < 0.0f) { m_position.x = 0.0f; }
	if (m_position.x + WIDTH > Screen::WIDTH) { m_position.x = Screen::WIDTH - WIDTH; }
	if (m_position.y < 0.0f) { m_position.y = 0.0f; }
	if (m_position.y + HEIGHT > Screen::HEIGHT) { m_position.y = Screen::HEIGHT - HEIGHT; }
}
