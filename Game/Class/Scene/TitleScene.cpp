/*
    @file   TitleScene.cpp
    @brief  タイトルシーンクラス
    @author 鈴木蒼良
    @date   2026年8月6日
*/

#include "pch.h"
#include "TitleScene.h"
#include "Game/Class/Manager/SceneManager.h"
#include "Game/GameContext.h"
#include "Game/Screen.h"
#include <cassert>

//  -----------------------------------------------------------------
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">シーンマネジャーの参照</param>
/// <param name="gameContext">ゲームコンテキストの参照</param>
/// -----------------------------------------------------------------

TitleScene::TitleScene(SceneManager& sceneManager, GameContext& gameContext)
    : m_sceneManager{ sceneManager }
    , m_gameContext{ gameContext }
    , m_player{gameContext}// Stageなし
    , m_demoHouse{
        gameContext,
        BoundingBox{ Vector2D{ HOUSE_POS_X, HOUSE_POS_Y }, Vector2D{ HOUSE_POS_X + 60.0f, HOUSE_POS_Y + 80.0f } },
        Item_Food::FoodType::Food1
    }
{
}









//  -----------------------------------------------------------------
/// <summary>
/// デストラクタ
/// </summary>
/// -----------------------------------------------------------------
TitleScene::~TitleScene()
{
}

//  -----------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// -----------------------------------------------------------------
void TitleScene::Initialize()
{   
    m_gameContext.soundManager.StartBgm(SoundManager::Bgm::Bgm_TitleScene);

    // Stageがないので、開始位置を直接指定する
    m_player.Initialize(Vector2D{ 800.0f, 300.0f });
    // デモ動作に切り替える
    m_player.SetControlMode(Player::ControlMode::AutoDemo);

    // デモ用のアイテムを1つ、UFOの左移動先あたりに配置する
    m_demoItems.clear();

    float FoodPosX = 170.0f;
    float FoodPosY = 100.0f;
    float FoodSize = 50;
    const BoundingBox itemBox{
        Vector2D{ FoodPosX, FoodPosY },
        Vector2D{ FoodPosX + FoodSize, FoodPosY + FoodSize }
    };

    auto item = std::make_unique<Item_Food>(
        m_gameContext,
        nullptr,           // Stageなし
        &m_player,
        itemBox,
        Item_Food::FoodType::Food1
    );
    item->Initialize();

    // 食べ物が止まる地面の位置を設定
    item->SetDemoGroundY(700.0f);

    m_demoItems.push_back(std::move(item));
}
//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void TitleScene::Update()
{
    // キー入力情報を取得する
    const int keyCondition = m_gameContext.inputManager.GetKeyCondition();
    const int keyTrigger = m_gameContext.inputManager.GetKeyTrigger();

    // スペースキーが押されたら
    if (keyTrigger & PAD_INPUT_10)
    {
        // シーンを変更する
        m_sceneManager.RequestNextSceneID(SceneManager::SceneID::SelectScene);
    }

    for (auto& item : m_demoItems)
    {
        item->Update();
    }

    m_player.Update();
    CheckDemoFoodHouseCollision();
}

//  -----------------------------------------------------------------
/// <summary>
/// 描画処理
/// </summary>
/// -----------------------------------------------------------------
void TitleScene::Render()
{
    DrawGraph(0, 0, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Background_1), TRUE);


    int defaultFontSize = GetFontSize();	// デフォルトのフォントサイズを記憶しておく
    SetFontSize(70);
    DrawString(300, 500, L"Spaceキーで始める", Colors::WHITE);
    SetFontSize(defaultFontSize);// フォントサイズを元に戻す


    for (auto& item : m_demoItems)
    {
        item->Render();
    }

    m_player.Render();
    m_demoHouse.Render();
    DrawGraph(200, 100, m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Logo), TRUE);

}

//  -----------------------------------------------------------------
/// <summary>
/// 終了処理
/// </summary>
/// -----------------------------------------------------------------
void TitleScene::Finalize()
{
    // タイトルシーン終了時にBGMを停止する
    m_gameContext.soundManager.EndBgm(SoundManager::Bgm::Bgm_TitleScene);
}

// 家と食べ物の当たり判定
void TitleScene::CheckDemoFoodHouseCollision()
{
    if (m_demoHouse.GetIsFulfilled()) { return; }

    for (auto& item : m_demoItems)
    {
        if (!item->GetActiveFlag()) { continue; }

        if (CheckHitAABB(item->GetBoundingBox(), m_demoHouse.GetBoundingBox()))
        {
            if (item->GetFoodType() == m_demoHouse.GetWantedFoodType())
            {
                item->SetActiveFlag(false);
                m_demoHouse.SetIsFulfilled(true);
                m_gameContext.soundManager.StartSe(SoundManager::Se::Se_Delivery);
            }
        }
    }
}


