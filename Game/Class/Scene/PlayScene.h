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


 // クラスの前方宣言 ===============================================================
// 前方宣言
struct GameContext;
class SceneManager;



class PlayScene
{
private:
    // ゲームコンテキストの、リファレンス
    GameContext& m_gameContext;
    // シーンマネジャーの、リファレンス
    SceneManager& m_sceneManager;
    
    // ステージクラスのインスタンス
    Stage m_stage;

    // プレイヤークラスのインスタンス
    Player m_player;



public:
    PlayScene(SceneManager& sceneManager, GameContext& gameContext);
    ~PlayScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();


    Stage& GetStage() { return m_stage; }
};
