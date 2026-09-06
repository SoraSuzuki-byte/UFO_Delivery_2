/*
    @file   TitleScene.cpp
    @brief  タイトルシーンクラス
    @author 鈴木蒼良
    @date   2026年8月6日
*/

#include "pch.h"
#include "TitleScene.h"
#include "Game/Class/Manager/SceneManager.h"
#include "Game/GameContext.h"
#include "Game/Screen.h"
#include <cassert>

//  -----------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">シーンマネジャーの参照</param>
/// <param name="gameContext">ゲームコンテキストの参照</param>
/// -----------------------------------------------------------------

TitleScene::TitleScene(SceneManager& sceneManager, GameContext& gameContext)
    : m_sceneManager{ sceneManager }
    , m_gameContext{ gameContext }

{
}









//  -----------------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
/// -----------------------------------------------------------------
TitleScene::~TitleScene()
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// -----------------------------------------------------------------
void TitleScene::Initialize()
{   
    m_gameContext.soundManager.StartBgm(SoundManager::Bgm::Bgm_TitleScene);
}
//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void TitleScene::Update()
{
    // キー入力情報を取得する
    const int keyCondition = m_gameContext.inputManager.GetKeyCondition();
    const int keyTrigger = m_gameContext.inputManager.GetKeyTrigger();

    // スペースキーが押されたら
    if (keyTrigger & PAD_INPUT_10)
    {
        // シーンを変更する
        m_sceneManager.RequestNextSceneID(SceneManager::SceneID::SelectScene);
    }



}

//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// -----------------------------------------------------------------
void TitleScene::Render()
{
    int x = 150;
    int y = 300;

    int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
    SetFontSize(80);
    DrawString(300, 500, L"スペースキーで始める", Colors::GRAY);
    SetFontSize(defaultFontSize);// フォントサイズを元に戻す


}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void TitleScene::Finalize()
{
    // タイトルシーン終了時にBGMを停止する
    m_gameContext.soundManager.EndBgm(SoundManager::Bgm::Bgm_TitleScene);
}


