/*
    @file   TitleScene.h
    @brief  タイトルシーンクラス
    @author 鈴木蒼良
    @date   2026年8月6日
*/
#pragma once
#include "Game/GameObject/Player.h"
#include "Game/GameObject/Item_Food.h"
#include "Game/GameObject/House.h"
#include "Game/Class/Effect/ShootingStar.h"
#include <vector>
#include <memory>


// 前方宣言
class SceneManager;
struct GameContext;


class TitleScene
{
private:
    // 家の大きさ
    static constexpr float WIDTH = 61.0f;
    static constexpr float HEIGHT = 38.0f;

    // 家の配置位置
    static constexpr float HOUSE_POS_X = 1100.0f;
    static constexpr float HOUSE_POS_Y = 650.0f;


    // シーンマネジャー
    SceneManager& m_sceneManager;

    GameContext& m_gameContext; // ゲームコンテキストへの 参照を覚える変数

    // プレイヤークラスのインスタンス
    Player m_player;

    // TitleSceneはStageを持たないので、unique_ptrで所有する
    std::vector<std::unique_ptr<Item_Food>> m_demoItems;

    // タイトルシーンの家
    House m_demoHouse;

    // 流れ星のインスタンス
    ShootingStar m_shootingStar;


public:
    TitleScene(SceneManager& sceneManager, GameContext& gameContext);
    ~TitleScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();


private:
    void CheckDemoFoodHouseCollision();

};

