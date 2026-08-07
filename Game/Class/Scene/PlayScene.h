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


 // クラスの前方宣言 ===============================================================
// 前方宣言
struct GameContext;
class SceneManager;



class PlayScene
{
private:
    // ゲームコンテキストの、インスタンス
    GameContext& m_gameContext;
    // シーンマネジャーの、インスタンス
    SceneManager& m_sceneManager;



public:
    PlayScene(SceneManager& sceneManager, GameContext& gameContext);
    ~PlayScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();

};
