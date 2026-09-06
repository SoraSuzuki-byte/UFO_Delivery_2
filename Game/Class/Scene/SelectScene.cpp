/*
    @file   SelectScene.cpp
    @brief  セレクトシーンクラス
    @author 鈴木蒼良
    @date   2026年8月6日
*/



#include "pch.h"
#include "SelectScene.h"
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
SelectScene::SelectScene(SceneManager& sceneManager, GameContext& gameContext)
    : m_sceneManager{ sceneManager }
    , m_gameContext{ gameContext }
{
}

//  -----------------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
/// -----------------------------------------------------------------
SelectScene::~SelectScene()
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// -----------------------------------------------------------------
void SelectScene::Initialize()
{
    m_gameContext.soundManager.StartBgm(SoundManager::Bgm::Bgm_SelectScene);
}

//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void SelectScene::Update()

{    // キー入力情報を取得する
    const int keyCondition = m_gameContext.inputManager.GetKeyCondition();
    const int keyTrigger = m_gameContext.inputManager.GetKeyTrigger();

    // スペースキーが押されたら
    if (keyTrigger & PAD_INPUT_10)
    {
        // シーンを変更する
        m_sceneManager.RequestNextSceneID(SceneManager::SceneID::PlayScene);
    }




    // ステージの合計の数を取得
    const int maxStages = static_cast<int>(StageId::Max);

    // みぎ矢印キーが押されたら
    if (keyTrigger & PAD_INPUT_RIGHT)
    {
        // 選択中のステージIDを、増加
        m_gameContext.selectedStageIndex++;

        if (m_gameContext.selectedStageIndex >= maxStages)// ステージの合計数より大きくなったら
        {
            m_gameContext.selectedStageIndex = maxStages - 1; // 合計数から1を引くことで、[一番大きい ステージID]に変える
        }
    }

    // ひだり矢印キーが押されたら
    if (keyTrigger & PAD_INPUT_LEFT)
    {
        // 選択中のステージIDを、減少
        m_gameContext.selectedStageIndex--;

        if (m_gameContext.selectedStageIndex <= 0)// 0より小さいステージを選択することになった場合
        {
            m_gameContext.selectedStageIndex = 0; // [一番小さな ステージID]に変える
        }
    }



}

//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// -----------------------------------------------------------------
void SelectScene::Render()
{
    int x = 150;
    int y = 300;

    int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
    SetFontSize(80);
    DrawFormatString(300, 300, Colors::GRAY, L"選択中のステージID: %d", m_gameContext.selectedStageIndex);
    DrawString(300, 500, L"セレクトシーン", Colors::GRAY);
    SetFontSize(defaultFontSize);// フォントサイズを元に戻す
}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void SelectScene::Finalize()
{
}
