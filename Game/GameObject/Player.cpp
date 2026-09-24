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



Player::Player(GameContext& gameContext, Stage* stage)
    : m_gameContext{gameContext}
    , m_stage{stage}
    , m_position{}
	, m_velocity{}
	, m_acceleration{}
	, m_animationTimer{}
	, m_hp{}
	, m_invincibleTimer{}
	, m_controlMode{ControlMode::Manual}
	, m_demoState {DemoState::MoveLeft}
	, m_demoTimer {}
	, m_isPullingInput{false}
{
}


Player::~Player()
{
}

void Player::Initialize(const Vector2D& startPosition)
{
	// Stageがあればステージ指定の開始位置、なければ引数の位置を使う
	if (m_stage != nullptr) 
	{
		m_position = m_stage->GetPlayerStartPosition();
	}
	else {
		m_position = startPosition;
	}

	// 速度を初期化
	m_velocity = Vector2D{ 0.0f, 0.0f };
	// 加速度を初期化
	m_acceleration = Vector2D{ 0.0f, 0.0f };

	m_animationTimer = 0;
	m_hp = MAX_HP;
	m_invincibleTimer = 0;
	m_controlMode = ControlMode::Manual;
	m_demoState = DemoState::MoveLeft;
	m_demoTimer = 0;
	m_isPullingInput = false;
}

void Player::Update()
{
	int keyCondition{};
	int keyTrigger{};
	
	if (m_controlMode == ControlMode::AutoDemo)
	{
		keyCondition = GenerateAutoInput();
		keyTrigger = 0;
	}
	else
	{
		keyCondition = m_gameContext.inputManager.GetKeyCondition();
		keyTrigger = m_gameContext.inputManager.GetKeyTrigger();
	}

	// 「今のフレームの吸引入力状態」を記録しておく
	m_isPullingInput = { (keyCondition & PAD_INPUT_10) != 0 };

	// 移動
	Move(keyCondition);	
	// 画面外に出ないようにする
	ClampPositionToScreen();
	// 当たった時の処理
	OnCollision();
}

void Player::Render()
{																										
	// m_invincibleTimerが0より大きいときは「無敵時間中」// 点滅させる間隔を、bool値で切り替える	
																										//「BLINK_INTERVALの数値のフレームごとに1段階進む」ゆっくりとした周期を作ります
	const bool isBlinking = (m_invincibleTimer > 0) && ((m_invincibleTimer / BLINK_INTERVAL) % 2 == 0);	// % 2 == 0：その値が偶数か奇数かで、true / falseを交互に繰り返します

	if (!isBlinking)
	{
		if (m_hp <= 0)
		{
			// 割れたUFOの画像
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Dead), TRUE);
		}
		else
		{
			// 素体の画像
			DrawGraph(m_position.x, m_position.y, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Bass), TRUE);
			// 移動時のアニメーション
			MoveAnimation();
			// ダメージ表現のオーバーレイ
			DrawDamageOverlay();
		}


	}


	// ★追加：HPを画面に文字で表示する（動作確認用）
	//DrawFormatString(10, 40, GetColor(255, 255, 0), L"HP: %d / %d", m_hp, MAX_HP);

}
   

void Player::Finalize()
{
}







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

	// タイトルデモ中だけ、最高速度を落とす（本編のMAX_SPEEDはそのまま）
	const float currentMaxSpeed = (m_controlMode == ControlMode::AutoDemo)// 条件式
		? MAX_SPEED * DEMO_SPEED_SCALE // 条件がtrueのとき、採用される値
		: MAX_SPEED; // 条件がfalseのとき、採用される値
		
	// 最高速度の制限
	if ((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y) > currentMaxSpeed * currentMaxSpeed)   //if (Length(m_velocity) > currentMaxSpeed) { ←これだと「sqrt」を使っており平方根で計算していて、処理が重たい
	{
		// 速度を正規化したあと、最大速度を かけ算
		m_velocity = Normalize(m_velocity) * currentMaxSpeed;
	}

	// 座標の更新
	m_position += m_velocity;
}
// 自機が画面外へ出ないように位置を修正する
void Player::ClampPositionToScreen()
{
	if (m_position.x < 0.0f) { m_position.x = 0.0f; }
	if (m_position.x + WIDTH > Screen::WIDTH) { m_position.x = Screen::WIDTH - WIDTH; }
	if (m_position.y < 0.0f) { m_position.y = 0.0f; }
	if (m_position.y + HEIGHT > Screen::HEIGHT) { m_position.y = Screen::HEIGHT - HEIGHT; }
}
// 当たった時の処理
void Player::OnCollision()
{
	// Stageがない（タイトル画面など）場合は壁判定を行わない
	if (m_stage == nullptr)
	{
		if (m_invincibleTimer > 0) { m_invincibleTimer--; }
		return;
	}


	// プレイヤーの境界ボックスを作成する
	BoundingBox playerBox{
		Vector2D{ m_position.x, m_position.y },
		Vector2D{ m_position.x + WIDTH, m_position.y + HEIGHT }
	};


	// ステージの草との当たり判定
	{
		// 壁にめり込んでいたら、押し戻し量を計算する
		const Vector2D correction = m_stage->ResolveWallCollision(playerBox);
		// 押し戻し量をプレイヤーの座標に反映する
		m_position += correction;

		// 壁に当たった方向の速度を反転させる（跳ね返り）
		if(correction.x != 0.0f) { m_velocity.x *= -BOUNCE_FACTOR; }
		if(correction.y != 0.0f) { m_velocity.y *= -BOUNCE_FACTOR; }

		// 壁に衝突するとダメージを受ける
		const bool isHit = (correction.x != 0.0f) || (correction.y != 0.0f);
		if(isHit) { TakeDamage(); }

		// 無敵タイマーを減らす
		if(m_invincibleTimer > 0) { m_invincibleTimer--; }
	}
}
// HPが減る処理
void Player::TakeDamage()
{
	if (m_invincibleTimer <= 0)
	{
		m_hp -= 1;
		m_invincibleTimer = INVINCIBLE_TIME;
		m_gameContext.soundManager.StartSe(SoundManager::Se::Se_TakeDamage);
	}

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




// タイトルシーンでのUFOの動きを処理
int Player::GenerateAutoInput()
{
	int input{};

	switch (m_demoState)
	{
		case DemoState::MoveLeft:
		{
			// まだ目標位置に届いていなければ、左キー入力を出す
			if (m_position.x > DEMO_LEFT_TARGET_X + DEMO_POSITION_TOLERANCE)
			{
				input |= PAD_INPUT_LEFT;
			}
			else
			{
				// 目標位置に到着 → 吸引ステートへ
				m_demoState = DemoState::Pulling;
				m_demoTimer = 0;
			}
			break;
		}

		case DemoState::Pulling:
		{
			// その場に留まりつつ、吸引ボタンを押し続ける
			input |= PAD_INPUT_10;

			m_demoTimer++;
			if (m_demoTimer >= DEMO_PULL_DURATION)
			{
				// 一定時間吸引したら、右移動ステートへ
				m_demoState = DemoState::MoveRight;
			}
			break;
		}

		case DemoState::MoveRight:
		{
			// 移動しながらも吸引ボタンは押し続ける → 食べ物を運びながら移動できる
			input |= PAD_INPUT_10;

			if (m_position.x < DEMO_RIGHT_TARGET_X - DEMO_POSITION_TOLERANCE)
			{
				input |= PAD_INPUT_RIGHT;
			}
			else
			{
				// 目標位置に到着 → 吸引をやめて停止ステートへ
				m_demoState = DemoState::Idle;
			}
			break;
		}

		case DemoState::Idle:
		{
			// 何も入力しない（吸引もしない＝Item_Food側の isPulled は false になる）
			break;
		}

	}
		return input;
}

