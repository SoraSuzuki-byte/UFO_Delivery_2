/*
    @file   PlayScene.cpp
    @brief  プレイシーンクラス
    @author 鈴木蒼良
    @date   2026/03/01
*/
#include "pch.h"
#include "PlayScene.h"
#include "Game/GameContext.h"
#include "Game/Screen.h"
#include <cassert>
#include "Game/Class/Manager/SceneManager.h"

//  -----------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">シーンマネジャーの参照</param>
/// <param name="gameContext">ゲームコンテキストの参照</param>
/// -----------------------------------------------------------------
PlayScene::PlayScene(SceneManager& sceneManager, GameContext& gameContext)
    : m_sceneManager{ sceneManager }
    , m_gameContext{ gameContext }
    , m_stage{ gameContext }
    , m_player{ gameContext,*this }
    , m_gameState{ GameState::Play }
    , m_titleReturnTimer{}
{
}

//  -----------------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
/// -----------------------------------------------------------------
PlayScene::~PlayScene()
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// -----------------------------------------------------------------
void PlayScene::Initialize()
{
    m_gameState = GameState::Play;
    m_titleReturnTimer = 0;

    m_stage.SetPlayer(m_player);   // Playerの参照をStageに設定

    // Stageにプレイヤーの参照を渡す（CSVロードより前に必要）
    m_stage.SetPlayer(m_player);
    //____________________________________________________________________________________________________
    // StageId(ステージID) → CSVファイル名へ対応
    static const wchar_t* stageNames[] = {
        L"stage_01",   // StageId::Stage1 用
        L"stage_02",   // StageId::Stage2 用
        L"stage_03",   // StageId::Stage3 用
        L"stage_04",   // StageId::Stage4 用
        L"stage_05",   // StageId::Stage5 用
    };

    // 現在選択されているステージIDを取得する
    const StageId selectedStage = m_gameContext.GetSelectedStageId();

    // StageId を配列の添字（int）に変換する
    const int index = static_cast<int>(selectedStage);

    // 対応するファイル名でステージを初期化する
    m_stage.Initialize(stageNames[index]);
//____________________________________________________________________________________________________」

    // ステージごとに家を配置する
    PlaceHouses(selectedStage);

    m_player.Initialize();
}

//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void PlayScene::Update()
{
    // キー入力情報を取得する
    const int keyCondition = m_gameContext.inputManager.GetKeyCondition();
    const int keyTrigger = m_gameContext.inputManager.GetKeyTrigger();


    // プレイ中のときのみ、プレイヤーや敵を 更新
    if (m_gameState == GameState::Play)
    {
        m_stage.Update(); // Stage.cppで「Enemy」「Food」「Bomb」などをUpdateしている
        m_player.Update();
        
        // HPが0になったら、ゲームオーバー状態に切り替える
        if (m_player.GetHp() <= 0) { m_gameState = GameState::GameOver; }

        // すべての家に届け終わったら、クリア状態に切り替える
        if (m_stage.IsAllHousesFulfilled()) { m_gameState = GameState::Clear; }
    }
    else if (m_gameState == GameState::GameOver || m_gameState == GameState::Clear)
    {
        // スペースキーの長押しで戻る
        if (keyCondition & PAD_INPUT_10)
        {
            m_titleReturnTimer++;

            if (m_titleReturnTimer >= TITLE_RETURN_HOLD_TIME)
            {
                m_sceneManager.RequestNextSceneID(SceneManager::SceneID::SelectScene);
            }
        }
        else { m_titleReturnTimer = 0; }// キーを離したらタイマーをリセットする
    }
}    
//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// -----------------------------------------------------------------
void PlayScene::Render()
{
    BackgroundRender();
    m_stage.Render();
    m_player.Render();

   

    // ゲームオーバー中なら、画面に重ねて表示する
    if (m_gameState == GameState::GameOver)
    {
        SetFontSize(100);
        DrawString(280, 300, L"GAME OVER", GetColor(255, 0, 0));

        // 長押しの進捗を 描画
        DrawTitleReturnGauge();
    }
    //　クリア中の表示
    else if (m_gameState == GameState::Clear)
    {
        SetFontSize(100);
        DrawString(320, 300, L"CLEAR!", GetColor(0, 255, 0));

        DrawTitleReturnGauge();
    }

}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void PlayScene::Finalize()
{
}



// ------------------------------------------------------------------
// 背景を描画
// ------------------------------------------------------------------
void PlayScene::BackgroundRender() const
{    
    // 選ばれているステージIDが、Stage1であれば
    if (m_gameContext.GetSelectedStageId() == StageId::Stage1)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_1), TRUE);
    }
    else if (m_gameContext.GetSelectedStageId() == StageId::Stage2)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_2), TRUE);
    }
    else if (m_gameContext.GetSelectedStageId() == StageId::Stage3)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_3), TRUE);
    }
}


// ------------------------------------------------------------------
// タイトルへ戻る長押しゲージの描画
// ------------------------------------------------------------------
void PlayScene::DrawTitleReturnGauge() const
{
    // ゲージの最大長
    const int MAX_WIDTH = 400;

    // 現在の長押し割合（0.0〜1.0）を計算する
    const float ratio = static_cast<float>(m_titleReturnTimer) / static_cast<float>(TITLE_RETURN_HOLD_TIME);

    // ゲージの左上座標（画面中央下寄りに配置）
    POINT offset{ Screen::CENTER_X - MAX_WIDTH / 2, 420 };

    // 案内テキスト
    DrawString(offset.x, offset.y - 30, L"スペースキー長押しでタイトルへ", Colors::WHITE);

    // ゲージの色（黄色で表現）
    const int color = GetColor(255, 255, 0);

    // 棒ゲージ（進捗ぶんだけ塗りつぶす）
    DrawBox(offset.x, offset.y,
        offset.x + static_cast<int>(MAX_WIDTH * ratio), offset.y + 30,
        color, TRUE);

    // 棒ゲージの枠
    DrawBox(offset.x, offset.y, offset.x + MAX_WIDTH, offset.y + 30, Colors::WHITE, FALSE);
}






// ステージごとに家を配置する
void PlayScene::PlaceHouses(StageId stageId)
{
    switch (stageId)
    {
        case StageId::Stage1:
        {
            m_stage.AddHouse(Vector2D{ 200.0f, 680.0f }, 80.0f, 80.0f, Item_Food::FoodType::Food1);
            m_stage.AddHouse(Vector2D{ 1100.0f, 65.0f }, 80.0f, 80.0f, Item_Food::FoodType::Food2);
            break;
        }
        case StageId::Stage2:
        {
            m_stage.AddHouse(Vector2D{ 100.0f, 600.0f }, 80.0f, 80.0f, Item_Food::FoodType::Food1);
            m_stage.AddHouse(Vector2D{ 1100.0f, 65.0f }, 80.0f, 80.0f, Item_Food::FoodType::Food2);
            break;
        }
        case StageId::Stage3:
        {
            m_stage.AddHouse(Vector2D{ 400.0f, 380.0f }, 80.0f, 80.0f, Item_Food::FoodType::Food2);
            m_stage.AddHouse(Vector2D{ 100.0f, 660.0f }, 80.0f, 80.0f, Item_Food::FoodType::Food1);
            break;
        }
        case StageId::Stage4:
        {
            m_stage.AddHouse(Vector2D{ 600.0f, 230.0f }, 80.0f, 80.0f, Item_Food::FoodType::Food2);
            break;
        }
        case StageId::Stage5:
        {
            m_stage.AddHouse(Vector2D{ 1000.0f, 680.0f }, 80.0f, 80.0f, Item_Food::FoodType::Food1);
            break;
        }
        default:
            break;
    }
}