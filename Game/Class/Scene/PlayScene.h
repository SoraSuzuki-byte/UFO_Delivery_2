/**
 * @file   PlayManager.h
 *
 * @brief  プレイシーンの、ヘッダファイル
 *
 * @author 鈴木蒼良
 *
 * @date   2026年8月6日
 */
#pragma once
#include "Game/GameObject/Stage.h"
#include "Game/GameObject/Player.h"
#include "Game/Class/Effect/SpawnEffect.h"
#include "Game/GameContext.h"

 // クラスの前方宣言 ===============================================================
// 前方宣言
struct GameContext;
class SceneManager;



class PlayScene
{
private:

    // ゲームの状態
    enum class GameState
    {
        Play,    // プレイ中
        GameOver,// ゲームオーバー
        Clear,   // クリア
    };
    GameState m_gameState;   // 現在のゲーム状態


    // ゲームコンテキストの、リファレンス
    GameContext& m_gameContext;
    // シーンマネジャーの、リファレンス
    SceneManager& m_sceneManager;
    
    // ステージクラスのインスタンス
    Stage m_stage;

    // プレイヤークラスのインスタンス
    Player m_player;

    // SpawnEffectのインスタンス
    SpawnEffect m_spawnEffect;


    static constexpr int TITLE_RETURN_HOLD_TIME = 60;  // タイトルに戻るまでの長押し時間（フレーム数）
    int m_titleReturnTimer;   // スペースキーを押し続けている時間

    static constexpr int RESULT_MESSAGE_TIME = 150;  // 「Spaceキーを長押し」を表示させる時間
    static constexpr int RESULT_MESSAGE_TO_HIDE_TIME = 20;  // 「Spaceキーを長押し」を隠す時間
    
    int m_messageBlinkCounter;// 「Spaceキーを長押し」の、メッセージを点滅させるカウンター


    // チュートリアル用のタイマー
    int m_stepTimer;
    // 移動方法を教える
    bool m_step1;
    // 吸引方法を教える
    bool m_step2;
    // 配達方法を教える
    bool m_step3;
    // チュートリアルステージのスキップ用のカウンタ
    int m_skipStage1Counter;
    static constexpr int SKIP_STAGE_TIME = 90;  // チュートリアルステージをスキップするときにかかる時間

public:
    PlayScene(SceneManager& sceneManager, GameContext& gameContext);
    ~PlayScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();

    // ゲームオーバー状態にする
    void SetGameOver() { m_gameState = GameState::GameOver; }

    Stage& GetStage() { return m_stage; }



    // 内部処理--------------------------------------------------------------------
private:
    // 背景の描画
    void BackgroundRender() const;

    // リザルト表示（ゲージと「長押し」の文字の描画）
    void DrawTitleReturnGauge() const;

    // リザルト表示時の、半透明の黒い四角を描画
    void DrawClearResultBackground();


    // ステージ1(チュートリアル)の更新
    void UpdateStage1(int keyCondition);
    // ステージ1(チュートリアル)の描画
    void RenderStage1();


    // ステージごとに家を配置する
    void PlaceHouses(StageId stageId);
};
