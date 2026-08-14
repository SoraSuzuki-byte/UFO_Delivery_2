/*
    @file   SceneManager.cpp
    @brief  シーンマネージャクラス
    @author 鈴木蒼良
    @date   2026年8月6日
*/


#include "pch.h"
#include "SceneManager.h"
#include "Game/GameContext.h"
#include "Game/Screen.h"

SceneManager::SceneManager(GameContext& gameContext)
    : m_gameContext {gameContext}
    , m_titleScene { *this, gameContext}
    , m_selectScene{ *this, gameContext }
    , m_playScene{ *this, gameContext }
    , m_currentSceneID{}
    , m_requestedSceneID{}
{
}

void SceneManager::Initialize()
{
    // メンバ変数を初期化する
    m_currentSceneID = SceneID::TitleScene;
    m_requestedSceneID = SceneID::None;

    // 現在シーンを初期化する
    InitializeCurrentScene();
}

void SceneManager::Update()
{
    // 現在シーンの更新
    UpdateCurrentScene();

    // 「シーン切り替えリクエスト発生」
    if (m_requestedSceneID != SceneID::None)
    {
        // シーン変更
        ChangeScene();
    }
}

void SceneManager::Render()
{
    // 現在シーンの描画
    RenderCurrentScene();
}

void SceneManager::Finalize()
{
    // 現在シーンの終了処理
    FinalizeCurrentScene();
}







//  -----------------------------------------------------------------
/// <summary>
/// 次のシーンをリクエストする
/// </summary>
/// <param name="requestSceneID">リクエストするシーンID</param>
/// -----------------------------------------------------------------
void SceneManager::RequestNextSceneID(SceneID requestSceneID)
{
    m_requestedSceneID = requestSceneID;
}

//  -----------------------------------------------------------------
/// <summary>
/// シーン変更処理
/// </summary>
/// -----------------------------------------------------------------
void SceneManager::ChangeScene()
{
    // 現在シーンの終了処理
    FinalizeCurrentScene();

    // シーンIDの更新
    m_currentSceneID = m_requestedSceneID;
    m_requestedSceneID = SceneID::None;

    // 次のシーンの初期化
    InitializeCurrentScene();
}








//  -----------------------------------------------------------------
/// <summary>
/// 現在実行中のシーンを初期化する
/// </summary>
/// -----------------------------------------------------------------
void SceneManager::InitializeCurrentScene()
{
    switch (m_currentSceneID)
    {
        case SceneID::TitleScene:   m_titleScene.Initialize();  break;
        case SceneID::SelectScene:   m_selectScene.Initialize();  break;
        case SceneID::PlayScene:    m_playScene.Initialize();   break;
        default:      assert(!"シーンIDが不正です");
    }
}

//  -----------------------------------------------------------------
/// <summary>
/// 現在実行中のシーンを更新する
/// </summary>
/// -----------------------------------------------------------------
void SceneManager::UpdateCurrentScene()
{
    switch (m_currentSceneID)
    {
        case SceneID::TitleScene:   m_titleScene.Update(); break;
        case SceneID::SelectScene:   m_selectScene.Update(); break;
        case SceneID::PlayScene:    m_playScene.Update();  break;
        default:      assert(!"シーンIDが不正です");
    }
}

//  -----------------------------------------------------------------
/// <summary>
/// 現在実行中のシーンを描画する
/// </summary>
/// -----------------------------------------------------------------
void SceneManager::RenderCurrentScene()
{
    switch (m_currentSceneID)
    {
        case SceneID::TitleScene:   m_titleScene.Render();  break;
        case SceneID::SelectScene:   m_selectScene.Render();  break;
        case SceneID::PlayScene:    m_playScene.Render();   break;
        default:      assert(!"シーンIDが不正です");
    }
}

//  -----------------------------------------------------------------
/// <summary>
/// 現在実行中のシーンを終了する
/// </summary>
/// -----------------------------------------------------------------
void SceneManager::FinalizeCurrentScene()
{
    switch (m_currentSceneID)
    {
        case SceneID::TitleScene:   m_titleScene.Finalize();  break;
        case SceneID::SelectScene:   m_selectScene.Finalize();  break;
        case SceneID::PlayScene:    m_playScene.Finalize();   break;
        default:      assert(!"シーンIDが不正です");
    }
}
