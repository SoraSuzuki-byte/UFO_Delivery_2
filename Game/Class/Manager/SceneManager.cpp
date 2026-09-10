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
    , m_transitionState{}
    , m_fadeFrameCounter{}
{
}

void SceneManager::Initialize()
{
    // メンバ変数を初期化する
    m_currentSceneID = SceneID::TitleScene;
    m_requestedSceneID = SceneID::None;

    // シーン遷移に関する初期化
    m_transitionState = TransitionState::None;
    m_fadeFrameCounter = 0;

    // 現在シーンを初期化する
    InitializeCurrentScene();
}




//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void SceneManager::Update()
{
    switch (m_transitionState)
    {
        // 通常時
        case TransitionState::None:
        {
            // 現在シーンの更新
            UpdateCurrentScene();

            // 「シーン切り替えリクエスト発生」
            if (m_requestedSceneID != SceneID::None)
            {
                m_transitionState = TransitionState::FadeOut;
                m_fadeFrameCounter = 0;
            }
            break;
        }
        // フェードアウト状態
        case TransitionState::FadeOut:
        {
            m_fadeFrameCounter++;
            if (m_fadeFrameCounter >= FADE_FRAMES)
            {
                m_transitionState = TransitionState::ChangeScene;
            }
            break;
        }
        // シーン変更状態
        case TransitionState::ChangeScene:
        {
            ChangeScene();
            m_transitionState = TransitionState::FadeIn;
            break;
        }
        // フェードイン状態
        case TransitionState::FadeIn:
        {
            m_fadeFrameCounter--;
            if (m_fadeFrameCounter <= 0)
            {
                m_fadeFrameCounter = 0;
                m_transitionState = TransitionState::None;
            }
            break;
        }
        default:
            assert(!"シーンのフェード状態が不正です");
    }
}



//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// -----------------------------------------------------------------
void SceneManager::Render()
{
    // 現在シーンの描画
    RenderCurrentScene();

    // フェードの描画
    if (m_transitionState != TransitionState::None)
    {
        DrawFade();
    }
}


//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
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


//  -----------------------------------------------------------------
/// <summary>
/// フェードの描画処理
/// </summary>
/// -----------------------------------------------------------------
void SceneManager::DrawFade()
{
    // 描画に使用するアルファ値を計算する
    const float fadeRate = static_cast<float>(m_fadeFrameCounter) / FADE_FRAMES;
    const int   alpha = static_cast<int>(255 * fadeRate);

    // アルファブレンドを設定し、画面を覆う四角形を描画する
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, Colors::WHITE, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
