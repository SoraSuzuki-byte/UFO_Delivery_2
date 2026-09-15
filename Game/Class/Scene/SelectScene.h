/**
 * @file   SelectManager.h
 *
 * @brief  セレクトシーンの、ヘッダファイル
 *
 * @author 鈴木蒼良
 *
 * @date   2026年8月6日
 */
#pragma once
#include "Library/GameMath.h"
#include "Game/GameContext.h"
#include "Game/Screen.h"


#include <array>

 // 前方宣言
struct GameContext;
class SceneManager;



class SelectScene
{
private:
    static constexpr const float MOVE_SPEED = 0.05f;

    static constexpr const float STAGE1_POS_X = 50.0f;
    static constexpr const float STAGE1_POS_Y = 50.0f;

    static constexpr const float STAGE2_POS_X = 140.0f;
    static constexpr const float STAGE2_POS_Y = 390.0f;

    static constexpr const float STAGE3_POS_X = 450.0f;
    static constexpr const float STAGE3_POS_Y = 520.0f;

    static constexpr const float STAGE4_POS_X = 820.0f;
    static constexpr const float STAGE4_POS_Y = 370.0f;

    static constexpr const float STAGE5_POS_X = 1120.0f;
    static constexpr const float STAGE5_POS_Y = 150.0f;


    static constexpr const float MESSAGE_YES_POS_X = 350.0f;
    static constexpr const float MESSAGE_YES_POS_Y = 450.0f;
    static constexpr const float MESSAGE_NO_POS_X = 750.0f;
    static constexpr const float MESSAGE_NO_POS_Y = 450.0f;

    static constexpr const float MESSAGE_UI_POS_X = 30.0f;
    static constexpr const float MESSAGE_UI_POS_Y = 600.0f;
    static constexpr const float MESSAGE_DIALOG_BOX_POS_X = 180.0f;
    static constexpr const float MESSAGE_DIALOG_BOX_POS_Y = 100.0f;

    // 「確認ダイアログ」のフェードの最大アルファ値
    static constexpr const int CONFIRM_FADE_MAX_ALPHA = 235;
    // 「確認ダイアログ」のフェードのアルファ値の増加量
    static constexpr const int CONFIRM_FADE_ALPHA_STEP = 5;
    // 「確認ダイアログ背景」の最大アルファ値
    static constexpr const int CONFIRM_DIALOG_BACKGROUND_MAX_ALPHA = 60;


    // StageId::Max を利用して配列サイズを固定
    // （描画・移動用の目標座標テーブル）   
    const std::array<Vector2D, static_cast<size_t>(StageId::Max)> STAGE_POSITIONS = // std::array は「サイズが固定された配列」を扱うための標準コンテナ
    { {
        { STAGE1_POS_X, STAGE1_POS_Y }, // Stage1 (0)
        { STAGE2_POS_X, STAGE2_POS_Y }, // Stage2 (1)
        { STAGE3_POS_X, STAGE3_POS_Y }, // Stage3 (2)
        { STAGE4_POS_X, STAGE4_POS_Y }, // Stage4 (3)
        { STAGE5_POS_X, STAGE5_POS_Y }, // Stage5 (4)
    } };

    // 「星系」を変える時に使う キャプチャ用画像とレンダーターゲットの準備
    int capturedGraph = MakeGraph(Screen::WIDTH, Screen::HEIGHT);

    // 「星系」を変える時の、カウンター
    int m_changeStarSystemCounter;

    // 「星系」を変える時の、最大カウンター
    static constexpr const int MAX_CHANGE_STAR_SYSTEM_FRAME = 120;

    // 画面エフェクトの進行具合
    float m_progress;

    // 「星系」移動キーを押している時間のカウンター
    int m_effectTimer = 0;

    

    // シーンマネジャー
    SceneManager& m_sceneManager;

    // ゲームコンテキスト
    GameContext& m_gameContext;

    // セレクト画面でのプレイヤーの位置
    Vector2D m_playerPosition;

    // ステージを選ぶフラグ
    bool m_isStageSelected;
    // 選んだステージでいいかを確認フラグ
    bool m_isConfirming;
    // 選択項目を点滅させる時のカウンター
    int m_selectionBlinkCounter;
    // ステージ選択の確認ダイアログ の"フェード用"
    int m_confirmFadeAlpha;


public:
    SelectScene(SceneManager& sceneManager, GameContext& gameContext);
    ~SelectScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();


private:
    void ChangeScene(int keyTrigger);
    void MovePlayer();
    void RenderPlayer();
    void RenderStageImage();
    void RenderArrowUi();
    void RenderConfirmingUi();
    void DrawDialogBackground();
};

