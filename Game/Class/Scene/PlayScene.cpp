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
    //____________________________________________________________________________________________________
    // StageId(ステージID) → CSVファイル名へ対応
    static const wchar_t* stageNames[] = {
        L"stage_01",   // StageId::Stage1 用
        L"stage_02",   // StageId::Stage2 用
        L"stage_03",   // StageId::Stage3 用
    };

    // 現在選択されているステージIDを取得する
    const StageId selectedStage = m_gameContext.GetSelectedStageId();

    // StageId を配列の添字（int）に変換する
    const int index = static_cast<int>(selectedStage);

    // 対応するファイル名でステージを初期化する
    m_stage.Initialize(stageNames[index]);
//____________________________________________________________________________________________________」

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

    // スペースキーが押されたら
    if (keyTrigger & PAD_INPUT_10)
    {
        // シーンを変更する
        m_sceneManager.RequestNextSceneID(SceneManager::SceneID::TitleScene);
    }

    m_player.Update();
}    
//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// -----------------------------------------------------------------
void PlayScene::Render()
{
    int x = 150;
    int y = 300;

    int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
    SetFontSize(80);
    DrawString(300, 500, L"プレイシーン", Colors::GRAY);
    SetFontSize(defaultFontSize);// フォントサイズを元に戻す


    m_stage.Render();
    m_player.Render();
}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void PlayScene::Finalize()
{
}
