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


    static constexpr int TITLE_RETURN_HOLD_TIME = 60;  // タイトルに戻るまでの長押し時間（フレーム数）
    int m_titleReturnTimer;   // スペースキーを押し続けている時間



public:
    PlayScene(SceneManager& sceneManager, GameContext& gameContext);
    ~PlayScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();


    Stage& GetStage() { return m_stage; }



    // 内部処理--------------------------------------------------------------------
private:
    // タイトルへ戻る長押しゲージの描画
    void DrawTitleReturnGauge() const;

    // 保有しているアイテムを表示
    void DrawHeldItemsUI() const;

    // ステージごとに家を配置する
    void PlaceHouses(StageId stageId);
};
