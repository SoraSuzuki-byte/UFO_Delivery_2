/*
    @file   SelectScene.cpp
    @brief  セレクトシーンクラス
    @author 鈴木蒼良
    @date   2026年8月6日
*/



#include "pch.h"
#include "SelectScene.h"
#include "Game/Class/Manager/SceneManager.h"
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
    , m_confirmFadeAlpha{ 0 }
    , m_changeStarSystemCounter{}
    , m_progress{}
    , m_effectTimer{}
    , m_isOtherStarSystem{ false }
    , m_starSystemChangeCooldown{}
    , m_canChangeStarSystem{false}
    , m_floatCount{}
    , m_playerFloatCount{}
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
    m_changeStarSystemCounter = 0;
    m_progress = 0;
    m_effectTimer = 0;
    m_starSystemChangeCooldown = 0;
    m_floatCount = 0;
    m_canChangeStarSystem = false;
    m_playerFloatCount = 0;
    // Stage1～5をクリアしている場合
    if (m_gameContext.IsStageCleared(StageId::Stage1) &&
        m_gameContext.IsStageCleared(StageId::Stage2) &&
        m_gameContext.IsStageCleared(StageId::Stage3) &&
        m_gameContext.IsStageCleared(StageId::Stage4) &&
        m_gameContext.IsStageCleared(StageId::Stage5))
    {
        m_canChangeStarSystem = true;
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



    //if ((!m_isStageSelected))
    if ((!m_isStageSelected) && (m_gameContext.IsStageCleared(StageId::Stage1))) //{デバッグデバッグデバッグデバッグデバッグデバッグデバッグデバッグ変更変更変更変更変更変更変更
    {
        // ステージの合計の数を取得
        //const int maxStages = static_cast<int>(StageId::Max);
        const int maxStages = 5;

        if (!m_isOtherStarSystem)// 星系を変えていなければ
        {
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
        else// 星系を変えていたならば
        {
            const int maxStages = static_cast<int>(StageId::Max);

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

                if (m_gameContext.selectedStageIndex < 5)// 5より小さいステージを選択することになった場合
                {
                    m_gameContext.selectedStageIndex = 5; // [一番小さな ステージID]に変える
                }
            }

        }

        // 1～5をクリアし、星系を変えられるなら
        if (m_canChangeStarSystem)
        {
            // 下矢印キーが押されている場合（星系を変える処理）
            if (keyCondition & PAD_INPUT_DOWN)
            {
                // クールダウン中は増加させない
                if (m_starSystemChangeCooldown <= 0)
                {
                    // エフェクトの進行度を増加
                    if (m_changeStarSystemCounter < MAX_CHANGE_STAR_SYSTEM_FRAME)
                    {
                        m_changeStarSystemCounter++;
                    }

                    // 閾値を越えると、「星系」フラグを反転し、エフェクトを止める
                    if (m_changeStarSystemCounter >= STAR_SYSTEM_CHANGE_THRESHOLD)
                    {
                        m_isOtherStarSystem = !m_isOtherStarSystem;// 星系変更フラグを反転させる
                        m_changeStarSystemCounter = 0;               // カウンターをリセット
                        m_starSystemChangeCooldown = STAR_SYSTEM_CHANGE_COOLDOWN_FRAME;// にクールダウンを開始

                        // trueに切り替わった時だけ、ステージ6にする
                        if (m_isOtherStarSystem)
                        {
                            m_gameContext.selectedStageIndex = 5;
                        }
                        // falseになったら、ステージ1に設定
                        if (!m_isOtherStarSystem)
                        {
                            m_gameContext.selectedStageIndex = 0;
                        }
                    }
                }
                // エフェクトの時間を進める
                m_effectTimer++;

            }
            // 離している間は、元に戻っていく
            else
            {
                if (m_changeStarSystemCounter > 0) { m_changeStarSystemCounter -= 3; }
            }

            // クールダウンはキー入力に関係なく毎フレーム減らす
            if (m_starSystemChangeCooldown > 0)
            {
                m_starSystemChangeCooldown--;
            }

            // エフェクトの進行度を計算
            m_progress = static_cast<float>(m_changeStarSystemCounter)
                / MAX_CHANGE_STAR_SYSTEM_FRAME;
        }
    }






    // ステージに行く処理
    if (!m_isStageSelected)
    {
        if (keyTrigger & PAD_INPUT_10)
        {
            m_isStageSelected = true;
            m_confirmFadeAlpha = 0;
        }
    }
    else
    {
        ChangeScene(keyTrigger);
    }

    // 確認ダイアログのフェード
    if (m_isStageSelected)
    {
        if (m_confirmFadeAlpha < CONFIRM_FADE_MAX_ALPHA)
        {
            m_confirmFadeAlpha += CONFIRM_FADE_ALPHA_STEP;
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
    if (!m_isOtherStarSystem)// 星系が最初のものであれば
    {
        RenderFirstStarSystem();
    }
    else// 星系が変わったら
    {
        RenderSecondStarSystem();
    }

    RenderArrowUi();
    RenderConfirmingUi();
    RenderPlayer();



    // 「星系」移動時に、画面エフェクトを描画する
    if (m_progress > 0.0f)
    {
        // 時間経過によるウネウネとした揺らぎ（波）
        float wave1 = sinf(m_effectTimer * 0.1f);
        float wave2 = cosf(m_effectTimer * 0.1f);

        int shiftX = (int)(m_progress * 20.0f + wave1 * 10.0f * m_progress);
        int shiftY = (int)(wave2 * 5.0f * m_progress);
        const float scale = 1.0f + (m_progress * 0.1f);
        double angle = (double)(m_progress * 0.03f + wave2 * 0.02f * m_progress);

        // 背景などのメイン画像を直接「色ズレ加算描画」する（超軽量）
        int bgHandle = m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_SelectScene);

        SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, (int)(m_progress * 255));

        // R (右・上)
        SetDrawBright(255, 0, 0);
        DrawRotaGraph(Screen::WIDTH / 2 + shiftX, Screen::HEIGHT / 2 + shiftY, scale, angle, bgHandle, TRUE);

        // G (中央)
        SetDrawBright(0, 255, 0);
        DrawRotaGraph(Screen::WIDTH / 2, Screen::HEIGHT / 2, scale, 0.0, bgHandle, TRUE);

        // B (左・下)
        SetDrawBright(0, 0, 255);
        DrawRotaGraph(Screen::WIDTH / 2 - shiftX, Screen::HEIGHT / 2 - shiftY, scale, -angle, bgHandle, TRUE);

        // 描画状態をリセット
        SetDrawBright(255, 255, 255);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
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
    // ふわふわ移動
    m_playerFloatCount += 0.02f;
    const float offsetY = sinf(m_playerFloatCount) * Player_FLOAT_HEIGHT;

    const int drawY = static_cast<int>(m_playerPosition.y + offsetY);
    const int drawX = static_cast<int>(m_playerPosition.x);



    DrawGraph(drawX, drawY, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Bass), TRUE);
    DrawGraph(drawX, drawY, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::UFO_Damage_Overlay), TRUE);
}



//  -----------------------------------------------------------------
// 1つ目の星系を描画
//  -----------------------------------------------------------------
void SelectScene::RenderFirstStarSystem()
{
    DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_SelectScene), TRUE);

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
// 2つ目の星系を描画
//  -----------------------------------------------------------------
void SelectScene::RenderSecondStarSystem()
{
    if (m_gameContext.IsStageCleared(StageId::Stage6) &&
        m_gameContext.IsStageCleared(StageId::Stage7) &&
        m_gameContext.IsStageCleared(StageId::Stage8))
    {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_SelectScene2_after), TRUE);
    }
    else
    {
    DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_SelectScene2_before), TRUE);
    }


    if (m_gameContext.IsStageCleared(StageId::Stage6)) {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage6_a), TRUE);
    }
    else {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage6_b), TRUE);
    }
    if (m_gameContext.IsStageCleared(StageId::Stage7)) {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage7_a), TRUE);
    }
    else {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage7_b), TRUE);
    }

    if (m_gameContext.IsStageCleared(StageId::Stage8)) {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage8_a), TRUE);
    }
    else {
        DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Stage8_b), TRUE);
    }

}



//  -----------------------------------------------------------------
// UI(矢印キー)を描画
//  -----------------------------------------------------------------
void SelectScene::RenderArrowUi()
{
    const int keyCondition = m_gameContext.inputManager.GetKeyCondition();

    // 1. カウントを少しずつ進める（速度の調整）
    m_floatCount += 0.02f;

    // 2. ふわふわの振幅（上下に何ピクセル動かすか）を設定してY座標のオフセットを計算
    //    sin(m_floatCount) は -1.0 〜 1.0 を往復するため、5.0f を掛けると上下 5ピクセル（計10px間）動きます
    float offsetY = sinf(m_floatCount) * 5.0f;

    // 3. 計算したオフセットを DOWN_ARROW_POSITION_Y に加えて描画
    int drawY = static_cast<int>(DOWN_ARROW_POSITION_Y + offsetY);



    // Stage1～5をクリアしている場合
    if (m_canChangeStarSystem)
    {
        int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
        SetFontSize(25);
        DrawString(50, drawY, L"長押し", Colors::WHITE);
        SetFontSize(defaultFontSize);// フォントサイズを元に戻す

        if (keyCondition & PAD_INPUT_DOWN)
        {
            DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::DownArrow_Push), TRUE);
        }
        else
        {
            DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::DownArrow_None), TRUE);
        }
        
    }
    // まだ、Stage1～5をクリアしていなければ ↓
    else
    {
        int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
        SetFontSize(25);
        DrawString(MESSAGE_UI_POS_X, MESSAGE_UI_POS_Y, L"Spaceキー：決定", Colors::WHITE);
        SetFontSize(defaultFontSize);// フォントサイズを元に戻す

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
    }
}



//  -----------------------------------------------------------------
// UI（ステージ選択の確認ダイアログ）を描画
//  -----------------------------------------------------------------
void SelectScene::RenderConfirmingUi()
{
    if (!m_isStageSelected) { return; } // 早期リターン


        int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく

        {// 黒い四角の描画
            const int dialogBackgroundAlpha = m_confirmFadeAlpha * CONFIRM_DIALOG_BACKGROUND_MAX_ALPHA / CONFIRM_FADE_MAX_ALPHA; // 黒い四角のアルファ値を計算
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, dialogBackgroundAlpha);
            DrawDialogBackground();
        }

        // ダイアログ全体の透明度を設定
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_confirmFadeAlpha);

        SetFontSize(80);
        DrawString(MESSAGE_DIALOG_BOX_POS_X, MESSAGE_DIALOG_BOX_POS_Y, L"配達先が決まりましたか？", Colors::WHITE, TRUE);
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

        // 描画モードを通常に戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}




// ------------------------------------------------------------------
// ステージ選択の確認ダイアログ "半透明の黒い四角"を描画
// ------------------------------------------------------------------
void SelectScene::DrawDialogBackground()
{
    // 半透明描画モードに設定 (アルファ値を128/255に設定: 約50%の透過度)
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);

    DrawBox(100, 40, 1200, 600, GetColor(0, 0, 0), TRUE);

    // 描画モードを通常に戻す 
    //SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}