/*
    @file   SelectScene.cpp
    @brief  セレクトシーンクラス
    @author 鈴木蒼良
    @date   2026年8月6日
*/



#include "pch.h"
#include "SelectScene.h"
#include "Game/Class/Manager/SceneManager.h"
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
    , m_playerPosition{}
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

    // 初期位置を現在の選択中ステージ座標に合わせる
    const size_t selectedIndex = static_cast<size_t>(m_gameContext.GetSelectedStageId());
    if (selectedIndex < STAGE_POSITIONS.size())
    {
        m_playerPosition.x = STAGE_POSITIONS[selectedIndex].x;
        m_playerPosition.y = STAGE_POSITIONS[selectedIndex].y;
    }
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

        if (m_gameContext.selectedStageIndex < 0)// 0より小さいステージを選択することになった場合
        {
            m_gameContext.selectedStageIndex = 0; // [一番小さな ステージID]に変える
        }
    }

    MovePlayer();

}

//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// -----------------------------------------------------------------
void SelectScene::Render()
{
    DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_SelectScene), TRUE);

    RenderStageImage();
    RenderPlayer();

    int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
    SetFontSize(80);
    DrawFormatString(300, 100, Colors::GRAY, L"選択中のステージID: %d", m_gameContext.selectedStageIndex);
    DrawString(300, 500, L"スペースキーを押して", Colors::GRAY);
    DrawString(300, 650, L"配送先を決定", Colors::GRAY);
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



//  -----------------------------------------------------------------
// プレイヤーの移動
//  -----------------------------------------------------------------
void SelectScene::MovePlayer()
{
    const size_t stageIndex = static_cast<size_t>(m_gameContext.GetSelectedStageId());
    if (stageIndex < STAGE_POSITIONS.size())
    {
        const Vector2D& targetPos = STAGE_POSITIONS[stageIndex];

        // イージング移動
        m_playerPosition.x += (targetPos.x - m_playerPosition.x) * MOVE_SPEED;
        m_playerPosition.y += (targetPos.y - m_playerPosition.y) * MOVE_SPEED;
    }
}



//  -----------------------------------------------------------------
// プレイヤーの描画
//  -----------------------------------------------------------------
void SelectScene::RenderPlayer()
{
    const int drawX = static_cast<int>(m_playerPosition.x);
    const int drawY = static_cast<int>(m_playerPosition.y);

    DrawGraph(drawX, drawY, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Bass), TRUE);
    DrawGraph(drawX, drawY, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Damage_Overlay), TRUE);
}



//  -----------------------------------------------------------------
// ステージの星の画像を描画
//  -----------------------------------------------------------------
void SelectScene::RenderStageImage()
{
    if (m_gameContext.IsStageCleared(StageId::Stage1)) {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage1_a), TRUE);
    }
    else {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage1_b), TRUE);
    }
    if (m_gameContext.IsStageCleared(StageId::Stage2)) {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage2_a), TRUE);
    }
    else {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage2_b), TRUE);
    }

    if (m_gameContext.IsStageCleared(StageId::Stage3)) {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage3_a), TRUE);
    }
    else {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage3_b), TRUE);
    }

    if (m_gameContext.IsStageCleared(StageId::Stage4)) {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage4_a), TRUE);
    }
    else {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage4_b), TRUE);
    }

    if (m_gameContext.IsStageCleared(StageId::Stage5)) {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage5_a), TRUE);
    }
    else {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage5_b), TRUE);
    }
}

void SelectScene::RenderUi()
{
}
