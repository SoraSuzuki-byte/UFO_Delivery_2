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
    , m_player{ gameContext, &m_stage }
    , m_spawnEffect{}
    , m_gameState{ GameState::Play }
    , m_titleReturnTimer{}
    , m_messageBlinkCounter{}
    , m_stepTimer{}
    , m_step1{false}
    , m_step2{false}
    , m_step3{false}
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
    m_messageBlinkCounter = 0;
    m_stepTimer = 0;
    m_step1 = false;
    m_step2 = false;
    m_step3 = false;


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
        L"stage_06",   // StageId::Stage6 用
        L"stage_07",   // StageId::Stage7 用
        L"stage_08",   // StageId::Stage8 用
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
    m_spawnEffect.Initialize();
    m_spawnEffect.SetPlayerPosition(m_player.GetPosition());
    m_spawnEffect.StartAnimation();
    
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


    // プレイ状態中に行う処理
    if (m_gameState == GameState::Play)
    {
        // チュートリアルを更新
        if (StageId::Stage1 == m_gameContext.GetSelectedStageId()) 
        { 
            UpdateStage1(keyCondition); 
        }

        // Stage.cppで「Enemy」「Food」「Bomb」などをUpdateしている
        m_stage.Update(); 
        m_player.Update();
        m_spawnEffect.Update();


        // HPが0になったら、ゲームオーバー状態に切り替える
        if (m_player.GetHp() <= 0) { m_gameState = GameState::GameOver; } 
        // ステージクラス内で ゲームオーバー判定になると
        if (m_stage.IsGameOver()) { m_gameState = GameState::GameOver; }


        // すべての家に届け終わり、敵が出現したことがあり、敵を全滅させるとクリア状態に切り替える
        if ((m_stage.IsAllHousesFulfilled()) && (m_stage.HasAnyEnemy()) && (m_stage.IsAllEnemiesDefeated()))
        { 
            // 現在のステージをクリア済みにする
            m_gameContext.SetStageCleared(m_gameContext.GetSelectedStageId());
            m_gameState = GameState::Clear;
        }
    }
    // リザルト中の更新処理
    else if (m_gameState == GameState::GameOver || m_gameState == GameState::Clear)
    {
        // スペースキーの長押しで戻る
        if (keyCondition & PAD_INPUT_10)
        {
            m_titleReturnTimer++;

            // 長押し中はSEをループ再生する
            m_gameContext.soundManager.LoopSe(SoundManager::Se::Se_ResultBar);

            // シーンを切り替える
            if (m_titleReturnTimer >= TITLE_RETURN_HOLD_TIME)
            {
                m_sceneManager.RequestNextSceneID(SceneManager::SceneID::SelectScene);
            }
        }
        // キーを離したら
        else { 
            // タイマーをリセットする
            m_titleReturnTimer = 0; 
            // SEを止める
            m_gameContext.soundManager.StopSe(SoundManager::Se::Se_ResultBar);
        }

        // 指定の値を越えると、リセット
        if (m_messageBlinkCounter > RESULT_MESSAGE_TIME + RESULT_MESSAGE_TO_HIDE_TIME)
        {
            m_messageBlinkCounter = 0;
        }
        // 毎フレーム増やす
        m_messageBlinkCounter++;
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
    const int spawnEffectHandle = m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::SpawnEffect);
    m_spawnEffect.Render(spawnEffectHandle);

    // チュートリアル表示
    if (StageId::Stage1 == m_gameContext.GetSelectedStageId())
    {
        RenderStage1();
    }

   

    // リザルトを描画
    if (m_gameState == GameState::GameOver)
    {
        DrawClearResultBackground();
        int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
        SetFontSize(100);
        DrawString(360, 220, L"GAME OVER", GetColor(255, 0, 0));
        SetFontSize(defaultFontSize);// フォントサイズを元に戻す

        // 長押しの進捗を 描画
        DrawTitleReturnGauge();
    }
    else if (m_gameState == GameState::Clear)
    {
        DrawClearResultBackground();
        int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
        SetFontSize(100);
        DrawString(480, 220, L"CLEAR!", GetColor(0, 255, 0));
        SetFontSize(defaultFontSize);// フォントサイズを元に戻す

        // 長押しの進捗を 描画
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
    m_gameContext.soundManager.StopSe(SoundManager::Se::Se_ResultBar);
}



// ------------------------------------------------------------------
// 背景の描画
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
    else if (m_gameContext.GetSelectedStageId() == StageId::Stage4)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_4), TRUE);
    }
    else if (m_gameContext.GetSelectedStageId() == StageId::Stage6)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_6), TRUE);
    }
    else if (m_gameContext.GetSelectedStageId() == StageId::Stage7)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_7), TRUE);
    }
    else if (m_gameContext.GetSelectedStageId() == StageId::Stage8)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_8), TRUE);
    }

    // ステージ５をクリアすると、背景が描画されるようになる
    if (m_gameContext.IsStageCleared(StageId::Stage5)) 
    {
        if (m_gameContext.GetSelectedStageId() == StageId::Stage5)
        {
            DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_5), TRUE);
        }
    }
}


// ------------------------------------------------------------------
// リザルト表示（ゲージと「長押し」の文字の描画）
// ------------------------------------------------------------------
void PlayScene::DrawTitleReturnGauge() const
{
    // ゲージの最大長
    const int MAX_WIDTH = 400;

    // 現在の長押し割合（0.0〜1.0）を計算する
    const float ratio = static_cast<float>(m_titleReturnTimer) / static_cast<float>(TITLE_RETURN_HOLD_TIME);

    // ゲージの左上座標（画面中央下寄りに配置）
    POINT offset{ Screen::CENTER_X - MAX_WIDTH / 2, 420 };

    int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
    SetFontSize(50);
    if (m_messageBlinkCounter < RESULT_MESSAGE_TIME)
    {
        DrawString(offset.x, offset.y - 60, L"Spaceキーを長押し", Colors::WHITE);
    }
    SetFontSize(defaultFontSize);// フォントサイズを元に戻す

    // ゲージの色（黄色で表現）
    const int color = GetColor(255, 255, 0);

    // 棒ゲージ（進捗ぶんだけ塗りつぶす）
    DrawBox(offset.x, offset.y,
        offset.x + static_cast<int>(MAX_WIDTH * ratio), offset.y + 30,
        color, TRUE);

    // 棒ゲージの枠
    DrawBox(offset.x, offset.y, offset.x + MAX_WIDTH, offset.y + 30, Colors::WHITE, FALSE);
}


// ------------------------------------------------------------------
// リザルト表示で、半透明の黒い四角を描画
// ------------------------------------------------------------------
void PlayScene::DrawClearResultBackground()
{
    // 半透明描画モードに設定 (アルファ値を128/255に設定: 約50%の透過度)
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

    DrawBox(200, 100, 1100, 600, GetColor(0, 0, 0), TRUE);

    // 描画モードを通常に戻す 
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}





// ------------------------------------------------------------------
// ステージ1(チュートリアル)の説明文の更新
// ------------------------------------------------------------------
void PlayScene::UpdateStage1(int keyCondition)
{
    if (!m_step1)
    {
        if (keyCondition & PAD_INPUT_UP ||
            keyCondition & PAD_INPUT_DOWN ||
            keyCondition & PAD_INPUT_LEFT ||
            keyCondition & PAD_INPUT_RIGHT)
        {
            m_stepTimer++;
        }
        if (m_stepTimer > 180) { 
            m_step1 = true; 
            m_stepTimer = 0;
        }
    }

    if (!m_step1) { return; }

    if (!m_step2)
    {
        if (m_stage.IsAnyFoodPulled() || m_stage.IsAnyBombPulled())
        {
            m_stepTimer++;
        }

        if ((m_stepTimer > 60) || (m_stage.IsAllHousesFulfilled()))
        {
            m_step2 = true;
            m_stepTimer = 0;
        }
    }

    if (!m_step2) { return; }

    if (!m_step3)
    {
        if (m_stage.IsAllHousesFulfilled())//全ての家に配達できたら
        {
            m_step3 = true;

            // step3に切り替わった瞬間に、敵を1体だけ出現させる
            m_stage.AddEnemy1(Vector2D{ -50.0f, 440.0f }); // 座標は出現させたい位置に調整

        }
    }
}

// ------------------------------------------------------------------
// ステージ1(チュートリアル)の説明文の描画
// ------------------------------------------------------------------
void PlayScene::RenderStage1()
{
    // 半透明描画モードに設定 (アルファ値を128/255に設定: 約50%の透過度)
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(0,0, 860, 160, GetColor(0, 0, 0), TRUE);    
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);// 描画モードを通常に戻す 

    int text_X = 20;
    int text_Y_1 = 20;
    int text_Y_2 = 100;
    int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
    SetFontSize(40);


    if (!m_step1)
    {
        DrawString(text_X, text_Y_1, L"矢印キーで移動します",Colors::WHITE, TRUE);
    }
    else if (!m_step2)
    {
        DrawString(text_X, text_Y_1, L"アイテムの真上で Spaceキーを押して下さい", Colors::WHITE, TRUE);
    }
    else if (!m_step3)
    {
        DrawString(text_X, text_Y_1, L"アイテムの真上で Spaceキーを押して下さい", Colors::WHITE, TRUE);
        DrawString(text_X, text_Y_2, L"食べ物を家に運んで下さい", Colors::WHITE, TRUE);
    }
    else
    {
        DrawString(text_X, text_Y_1, L"爆弾を 敵に当てて倒し、", Colors::WHITE, TRUE);
        DrawString(text_X, text_Y_2, L"全ての家に食べ物を届けると、配達完了です", Colors::WHITE, TRUE);
    }

    SetFontSize(defaultFontSize);// フォントサイズを元に戻す
}






// ------------------------------------------------------------------
// ステージごとに家を配置する
// ------------------------------------------------------------------
void PlayScene::PlaceHouses(StageId stageId)
{
    switch (stageId)
    {
        case StageId::Stage1:
        {
            m_stage.AddHouse(Vector2D{ 200.0f, 660.0f },Item_Food::FoodType::Food1);
            m_stage.AddHouse(Vector2D{ 1050.0f, 480.0f },Item_Food::FoodType::Food2);
            break;
        }
        case StageId::Stage2:
        {
            m_stage.AddHouse(Vector2D{ 100.0f, 600.0f },Item_Food::FoodType::Food1);
            m_stage.AddHouse(Vector2D{ 1100.0f, 65.0f },Item_Food::FoodType::Food3);
            break;
        }
        case StageId::Stage3:
        {
            m_stage.AddHouse(Vector2D{ 400.0f, 380.0f },Item_Food::FoodType::Food2);
            m_stage.AddHouse(Vector2D{ 100.0f, 660.0f },Item_Food::FoodType::Food1);
            break;
        }
        case StageId::Stage4:
        {
            m_stage.AddHouse(Vector2D{ 600.0f, 230.0f },Item_Food::FoodType::Food4);
            break;
        }
        case StageId::Stage5:
        {
            m_stage.AddHouse(Vector2D{ 1000.0f, 660.0f }, Item_Food::FoodType::Food5);
            break;
        }
        case StageId::Stage6:
        {
            m_stage.AddHouse(Vector2D{ 100.0f, 220.0f }, Item_Food::FoodType::Food6);
            m_stage.AddHouse(Vector2D{ 1200.0f, 660.0f }, Item_Food::FoodType::Food6);
            break;
        }
        case StageId::Stage7:
        {
            m_stage.AddHouse(Vector2D{ 0.0f, 0.0f }, Item_Food::FoodType::Food7);
            m_stage.AddHouse(Vector2D{ 800.0f, 660.0f }, Item_Food::FoodType::Food7);
            m_stage.AddHouse(Vector2D{ 1200.0f, 660.0f }, Item_Food::FoodType::Food8);
            break;
        }
        case StageId::Stage8:
        {
            m_stage.AddHouse(Vector2D{ 0.0f, 0.0f }, Item_Food::FoodType::Food7);
            m_stage.AddHouse(Vector2D{ 500.0f, 0.0f }, Item_Food::FoodType::Food8);
            m_stage.AddHouse(Vector2D{ 800.0f, 0.0f }, Item_Food::FoodType::Food8);
            m_stage.AddHouse(Vector2D{ 1200.0f, 0.0f }, Item_Food::FoodType::Food8);
            break;
        }
        default:
            break;
    }
}