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
        m_sceneManager.RequestNextSceneID(SceneManager::SceneID::kTitle);
    }


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
}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void PlayScene::Finalize()
{
}
