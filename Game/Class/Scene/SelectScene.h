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


 // 前方宣言
struct GameContext;
class SceneManager;



class SelectScene
{
private:
    // シーンマネジャー
    SceneManager& m_sceneManager;

    // ゲームコンテキスト
    GameContext& m_gameContext;



public:
    SelectScene(SceneManager& sceneManager, GameContext& gameContext);
    ~SelectScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();
};

