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

#include <array>

 // 前方宣言
struct GameContext;
class SceneManager;



class SelectScene
{
private:
    static constexpr const float STAGE1_POS_X = 100.0f;
    static constexpr const float STAGE1_POS_Y = 100.0f;

    static constexpr const float STAGE2_POS_X = 130.0f;
    static constexpr const float STAGE2_POS_Y = 300.0f;

    static constexpr const float STAGE3_POS_X = 240.0f;
    static constexpr const float STAGE3_POS_Y = 350.0f;

    static constexpr const float STAGE4_POS_X = 620.0f;
    static constexpr const float STAGE4_POS_Y = 600.0f;

    static constexpr const float STAGE5_POS_X = 1200.0f;
    static constexpr const float STAGE5_POS_Y = 100.0f;

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


    // シーンマネジャー
    SceneManager& m_sceneManager;

    // ゲームコンテキスト
    GameContext& m_gameContext;

    // セレクト画面でのプレイヤーの位置
    Vector2D m_playerPosition;



public:
    SelectScene(SceneManager& sceneManager, GameContext& gameContext);
    ~SelectScene();

    void Initialize();
    void Update();
    void Render();
    void Finalize();


private:
    void MovePlayer();
    void RenderPlayer();
};

