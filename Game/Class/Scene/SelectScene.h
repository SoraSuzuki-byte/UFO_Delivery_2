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

    static constexpr const float STAGE6_POS_X = 560.0f;
    static constexpr const float STAGE6_POS_Y = 500.0f;
    
    static constexpr const float STAGE7_POS_X = 520.0f;
    static constexpr const float STAGE7_POS_Y = 200.0f;

    static constexpr const float STAGE8_POS_X = 1080.0f;
    static constexpr const float STAGE8_POS_Y = 150.0f;


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

    // 「↓」キーのY座標
    static constexpr const int DOWN_ARROW_POSITION_Y = 600;
    // プレイヤーの上下移動の 高さ
    static constexpr const int Player_FLOAT_HEIGHT = 20;



    // StageId::Max を利用して配列サイズを固定
    // （描画・移動用の目標座標テーブル）   
    const std::array<Vector2D, static_cast<size_t>(StageId::Max)> STAGE_POSITIONS = // std::array は「サイズが固定された配列」を扱うための標準コンテナ
    { {
        { STAGE1_POS_X, STAGE1_POS_Y }, // Stage1 (0)
        { STAGE2_POS_X, STAGE2_POS_Y }, // Stage2 (1)
        { STAGE3_POS_X, STAGE3_POS_Y }, // Stage3 (2)
        { STAGE4_POS_X, STAGE4_POS_Y }, // Stage4 (3)
        { STAGE5_POS_X, STAGE5_POS_Y }, // Stage5 (4)
        { STAGE6_POS_X, STAGE6_POS_Y }, // Stage6 (5)
        { STAGE7_POS_X, STAGE7_POS_Y }, // Stage7 (6)
        { STAGE8_POS_X, STAGE8_POS_Y }, // Stage8 (7)
    } };

    // 「星系」を変える時に使う キャプチャ用画像とレンダーターゲットの準備
    int capturedGraph = MakeGraph(Screen::WIDTH, Screen::HEIGHT);

    // 「星系」を変える時の、カウンター
    int m_changeStarSystemCounter;

    // 「星系」を変える時の、最大カウンター
    static constexpr const int STAR_SYSTEM_CHANGE_THRESHOLD = 100;
    static constexpr const int MAX_CHANGE_STAR_SYSTEM_FRAME = 120;

    // 画面エフェクトの進行具合
    float m_progress;

    // 「星系」移動キーを押している時間のカウンター
    int m_effectTimer = 0;

    // 「星系」をもう一つの方に変更しているのか
    bool m_isOtherStarSystem;

    // ステージ1～5をクリアしたか
    bool m_canChangeStarSystem;

     int m_starSystemChangeCooldown = 0; // 「星系」切り替えのクールダウン残りフレーム数
     static const int STAR_SYSTEM_CHANGE_COOLDOWN_FRAME = 120; // 切り替えのクールタイム



    

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
    // 「↓」キーの、ふわふわアニメーション用のカウンタ
    float m_floatCount = 0.0f; 
    // プレイヤーの、ふわふわアニメーション用のカウンタ
    float m_playerFloatCount;


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
    void RenderFirstStarSystem();
    void RenderSecondStarSystem();
    void RenderArrowUi();
    void RenderConfirmingUi();
    void DrawDialogBackground();
};

