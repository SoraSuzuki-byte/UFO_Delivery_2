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
	, m_animationTimer{}
	, m_hp{}
	, m_invincibleTimer{}
	, m_heldItems{}
	, m_selectedItemIndex{}
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

	m_animationTimer = 0;
	m_hp = MAX_HP;
	m_invincibleTimer = 0;
}

void Player::Update()
{
	// キー入力情報を取得する
	const int keyCondition = m_gameContext.inputManager.GetKeyCondition();
	const int keyTrigger = m_gameContext.inputManager.GetKeyTrigger();

	// 移動
	Move(keyCondition);

	// ★アイテムの選択切り替え・ドロップ操作
	UpdateItemHolding(keyCondition, keyTrigger);
	
}

void Player::Render()
{
    // 素体の画像
    DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Bass), TRUE);

	// 移動時のアニメーション
	MoveAnimation();

	// ダメージ表現のオーバーレイ
	DrawDamageOverlay();



	// ★追加：HPを画面に文字で表示する（動作確認用）
	DrawFormatString(10, 40, GetColor(255, 255, 0), L"HP: %d / %d", m_hp, MAX_HP);

}
   

void Player::Finalize()
{
}






//item.SetActiveFlag(false);   // 目的地に運んで、加点されると（消える）




// UFOを移動させる
void Player::Move(int keyCondition)
{
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


	// プレイヤーの境界ボックスを作成する
	BoundingBox playerBox{
		Vector2D{ m_position.x, m_position.y },
		Vector2D{ m_position.x + WIDTH, m_position.y + HEIGHT }
	};

	// ItemFood_1と当たると
	CheckItemFood_1Collision(playerBox);

	// ステージの草との当たり判定
	{
		// 壁にめり込んでいたら、押し戻し量を計算する
		const Vector2D correction = m_scene.GetStage().ResolveWallCollision(playerBox);

		// 押し戻し量をプレイヤーの座標に反映する
		m_position += correction;

		// 壁に当たった方向の速度を反転させる（跳ね返り）
		if (correction.x != 0.0f) { m_velocity.x *= -BOUNCE_FACTOR; }
		if (correction.y != 0.0f) { m_velocity.y *= -BOUNCE_FACTOR; }

		// 壁に当たっていて、かつ無敵時間中でなければダメージを受ける
		const bool isHit = (correction.x != 0.0f) || (correction.y != 0.0f);
		if (isHit && m_invincibleTimer <= 0)
		{
			m_hp--;
			m_invincibleTimer = INVINCIBLE_TIME;
		}

		// 無敵タイマーを減らす
		if (m_invincibleTimer > 0)
		{
			m_invincibleTimer--;
		}
	}


	// 自機が画面外へ出ないように位置を修正する
	if (m_position.x < 0.0f) { m_position.x = 0.0f; }
	if (m_position.x + WIDTH > Screen::WIDTH) { m_position.x = Screen::WIDTH - WIDTH; }
	if (m_position.y < 0.0f) { m_position.y = 0.0f; }
	if (m_position.y + HEIGHT > Screen::HEIGHT) { m_position.y = Screen::HEIGHT - HEIGHT; }
}


// 移動時のアニメーション
void Player::MoveAnimation()
{
	// キー入力情報を取得する
	const int keyCondition = m_gameContext.inputManager.GetKeyCondition();

	// 指定のカウント時間
	const int VERTICAL_MOVE_ANIM_INTERVAL = 30; // たて移動
	const int HORIZONTAL_MOVE_ANIM_INTERVAL = 15;//よこ移動
	// 差分の画像を映す時間
	const int UFO_VARIANT_HOLD_TIME = 30;

	// いずれかの移動キーが押されているか確認
	const bool isMoving = (keyCondition & (PAD_INPUT_UP | PAD_INPUT_DOWN | PAD_INPUT_LEFT | PAD_INPUT_RIGHT)) != 0;

	// 1. キーを押していなければタイマーリセットして終了
	if (!isMoving)
	{
		m_animationTimer = 0;
		return;
	}

	// 2. タイマー加算
	m_animationTimer++;

	// 3. アニメーションの描画処理
	if (keyCondition & (PAD_INPUT_UP | PAD_INPUT_DOWN))
	{
		// 上下入力時の処理
		if (m_animationTimer >= VERTICAL_MOVE_ANIM_INTERVAL + UFO_VARIANT_HOLD_TIME)
		{
			m_animationTimer = 0;
		}

		if (m_animationTimer >= VERTICAL_MOVE_ANIM_INTERVAL)
		{
			// 全部の窓がオレンジになる
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Orange_All), TRUE);
		}
	}
	// 左入力時の処理
	else if (keyCondition & PAD_INPUT_LEFT)
	{		
		const int MAX_LEFT_TIME = HORIZONTAL_MOVE_ANIM_INTERVAL * 3 + UFO_VARIANT_HOLD_TIME;

		// カウントを0に
		if (m_animationTimer >= MAX_LEFT_TIME) m_animationTimer = 0;


		if (m_animationTimer >= HORIZONTAL_MOVE_ANIM_INTERVAL * 3)
		{
			// ひだりが点灯
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Orange_Left), TRUE);
		}
		else if (m_animationTimer >= HORIZONTAL_MOVE_ANIM_INTERVAL * 2)
		{
			// 中央が点灯
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Orange_Middle), TRUE);
		}
		else if (m_animationTimer >= HORIZONTAL_MOVE_ANIM_INTERVAL)
		{
			// みぎが点灯
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Orange_Right), TRUE);
		}
	}
	// 右入力時の処理
	else if (keyCondition & PAD_INPUT_RIGHT)
	{
		const int MAX_RIGHT_TIME = HORIZONTAL_MOVE_ANIM_INTERVAL * 3 + UFO_VARIANT_HOLD_TIME;

		// カウントを0に
		if (m_animationTimer >= MAX_RIGHT_TIME) m_animationTimer = 0; 
			

		if (m_animationTimer >= HORIZONTAL_MOVE_ANIM_INTERVAL * 3)
		{
			// みぎが点灯
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Orange_Right), TRUE);
		}
		else if (m_animationTimer >= HORIZONTAL_MOVE_ANIM_INTERVAL * 2)
		{
			// 中央が点灯
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Orange_Middle), TRUE);
		}
		else if (m_animationTimer >= HORIZONTAL_MOVE_ANIM_INTERVAL)
		{
			// ひだりが点灯
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Orange_Left), TRUE);
		}
	}
}


// ダメージ表現のオーバーレイ
void Player::DrawDamageOverlay()
{
	// HPが満タンなら 255（完全に見える）、HPが0なら 0（完全に透明）
	const float hpRatio = static_cast<float>(m_hp) / static_cast<float>(MAX_HP);
	const int alpha = static_cast<int>(255 * hpRatio);

	// 透明度を設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// UFOのHPがわかるようにオーバーレイ
	DrawGraph(m_position.x, m_position.y,
		m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Damage_Overlay),
		TRUE);

	// 透明度を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

}


// ItemFood_1と当たると
void Player::CheckItemFood_1Collision(BoundingBox playerBox)
{
	for (auto& item : m_scene.GetStage().GetItems())
	{
		if (!item.GetActiveFlag()) { continue; }

		if (CheckHitAABB(playerBox, item.GetBoundingBox()))
		{
			// TODO: ここに取得時の効果（HP回復など）を後で追加
		}
	}
}


// ------------------------------------------------------------------
// 保有アイテムの選択切り替え・ドロップ操作
// ------------------------------------------------------------------
void Player::UpdateItemHolding(int keyCondition, int keyTrigger)
{
	// 左SHIFTキーの判定は、DXライブラリのCheckHitKeyで直接行う
	const bool isShiftPressed = (CheckHitKey(KEY_INPUT_LSHIFT) != 0);

	// 左SHIFTキーを押しながら、左右矢印で選択を切り替える
	if (isShiftPressed)
	{
		if (keyTrigger & PAD_INPUT_LEFT)
		{
			m_selectedItemIndex--;
			if (m_selectedItemIndex < 0) { m_selectedItemIndex = 0; }
		}
		if (keyTrigger & PAD_INPUT_RIGHT)
		{
			m_selectedItemIndex++;
			const int maxIndex = static_cast<int>(m_heldItems.size()) - 1;
			if (m_selectedItemIndex > maxIndex) { m_selectedItemIndex = maxIndex < 0 ? 0 : maxIndex; }
		}
	}

	// スペースキーで、選択中のアイテムを落とす
	if (keyTrigger & PAD_INPUT_10)   // これまでスペースキーとして使ってきた定数
	{
		if (!m_heldItems.empty() && m_selectedItemIndex < static_cast<int>(m_heldItems.size()))
		{
			Item_Food_1* dropItem = m_heldItems[m_selectedItemIndex];
			dropItem->Drop(m_position);

			m_heldItems.erase(m_heldItems.begin() + m_selectedItemIndex);

			// 選択インデックスが範囲外にならないよう調整
			if (m_selectedItemIndex >= static_cast<int>(m_heldItems.size()))
			{
				m_selectedItemIndex = static_cast<int>(m_heldItems.size()) - 1;
				if (m_selectedItemIndex < 0) { m_selectedItemIndex = 0; }
			}
		}
	}
}