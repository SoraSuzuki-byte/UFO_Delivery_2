/*
    @file   TitleScene.h
    @brief  タイトルシーンクラス
    @author 鈴木蒼良
    @date   2026年8月6日
*/
#pragma once

// 前方宣言
class SceneManager;
struct GameContext;


class TitleScene
{
private:
    // シーンマネジャー
    SceneManager& m_sceneManager;

    GameContext& m_gameContext;    // ゲームコンテキストへの 参照を覚える変数



public:
    TitleScene(SceneManager& sceneManager, GameContext& gameContext);
    ~TitleScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();

};

