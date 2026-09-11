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
    , m_isStageSelected{false}
    , m_isConfirming{false}
    , m_selectionBlinkCounter{}
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
    m_isStageSelected = false;
    m_isConfirming = false;
    m_selectionBlinkCounter = 0;
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


    {
        // ステージの合計の数を取得
        const int maxStages = static_cast<int>(StageId::Max);

        if ((!m_isStageSelected) && (m_gameContext.IsStageCleared(StageId::Stage1))) {

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
        }
    }

    MovePlayer();


    // シーン変更の処理
    if (!m_isStageSelected)
    {
        if (keyTrigger & PAD_INPUT_10) { m_isStageSelected = true; }
    }
    else
    {
        ChangeScene(keyTrigger);
    }
}

//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// -----------------------------------------------------------------
void SelectScene::Render()
{
    DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_SelectScene), TRUE);

    int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
    //SetFontSize(80);
    //if (!m_isStageSelected) {
    //    DrawString(MESSAGE_DIALOG_BOX_POS_X, MESSAGE_DIALOG_BOX_POS_Y, L"配達先を選びましょう", Colors::WHITE);
    //}
    SetFontSize(25);
    DrawString(MESSAGE_UI_POS_X, MESSAGE_UI_POS_Y, L"Spaceキーで決定", Colors::WHITE);
    SetFontSize(defaultFontSize);// フォントサイズを元に戻す


    RenderStageImage();
    RenderPlayer();
    RenderArrowUi();
    RenderConfirmingUi();


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
// シーン変更に関する処理
//  -----------------------------------------------------------------
void SelectScene::ChangeScene(int keyTrigger)
{
    // 右キーが押されたら
    if (keyTrigger & PAD_INPUT_RIGHT) { 
        m_selectionBlinkCounter = 0;// 点滅カウントを0に
        m_isConfirming = false; 
    }
    // 左キーが押されたら
    else if (keyTrigger & PAD_INPUT_LEFT) { 
        m_selectionBlinkCounter = 0;// 点滅カウントを0に
        m_isConfirming = true; 
    }

    if (m_isConfirming) 
    {   
        if (keyTrigger & PAD_INPUT_10) { m_sceneManager.RequestNextSceneID(SceneManager::SceneID::PlayScene); }
    }
    else {
        if (keyTrigger & PAD_INPUT_10) {
            m_selectionBlinkCounter = 0; // カウントを0に
            m_isStageSelected = false; }
    }
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



//  -----------------------------------------------------------------
// UI(左右キー)を描画
//  -----------------------------------------------------------------
void SelectScene::RenderArrowUi()
{
    const int keyCondition = m_gameContext.inputManager.GetKeyCondition();

    if (keyCondition & PAD_INPUT_LEFT)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::LeftArrow_Push), TRUE);
    }
    else
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::LeftArrow_None), TRUE);
    }

    if (keyCondition & PAD_INPUT_RIGHT)
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::RightArrow_Push), TRUE);
    }
    else
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::RightArrow_None), TRUE);
    }

    //押してる時は、「Push」の画像を表示して、、押していない時「None」の画像を描画する
    //    ↑
    //    設計をAIに効く

}



//  -----------------------------------------------------------------
// UI（ステージ選択の確認ダイアログ）を描画
//  -----------------------------------------------------------------
void SelectScene::RenderConfirmingUi()
{
    if (!m_isStageSelected) { return; } // 早期リターン


        int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
        SetFontSize(80);
        DrawString(MESSAGE_DIALOG_BOX_POS_X, MESSAGE_DIALOG_BOX_POS_Y, L"配達先が決まりましたか？", Colors::WHITE, TRUE);
        DrawDialogBackground();
        DrawString(MESSAGE_YES_POS_X, MESSAGE_YES_POS_Y, L"はい", Colors::GRAY, TRUE);
        DrawString(MESSAGE_NO_POS_X, MESSAGE_NO_POS_Y, L"いいえ", Colors::GRAY, TRUE);

        m_selectionBlinkCounter ++;
        if (m_selectionBlinkCounter >= 180) { 
            m_selectionBlinkCounter = 0; 
        }

        if (m_selectionBlinkCounter < 120)
        {
            if (m_isConfirming) {
                DrawString(MESSAGE_YES_POS_X, MESSAGE_YES_POS_Y, L"はい", Colors::WHITE, TRUE);
                DrawString(MESSAGE_YES_POS_X, MESSAGE_YES_POS_Y, L"はい", Colors::WHITE, FALSE); // 文字の縁取り
            }
            else {
                DrawString(MESSAGE_NO_POS_X, MESSAGE_NO_POS_Y, L"いいえ", Colors::WHITE, TRUE);
                DrawString(MESSAGE_NO_POS_X, MESSAGE_NO_POS_Y, L"いいえ", Colors::WHITE, FALSE); // 文字の縁取り
            }
        }
        SetFontSize(defaultFontSize);// フォントサイズを元に戻す
}




// ------------------------------------------------------------------
// ステージ選択の確認ダイアログ "半透明の黒い四角"を描画
// ------------------------------------------------------------------
void SelectScene::DrawDialogBackground()
{
    // 半透明描画モードに設定 (アルファ値を128/255に設定: 約50%の透過度)
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);

    DrawBox(100, 40, 1200, 600, GetColor(0, 0, 0), TRUE);

    // 描画モードを通常に戻す 
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}