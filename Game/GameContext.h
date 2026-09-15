/*
    @file   GameContext.h
    @brief  ゲームコンテキストクラス（構造体）
    @author 制作者
    @date   2026/02/02
*/
#pragma once
#include "Game/Class/Manager/InputManager.h"
#include "Game/Class/Manager/GhManager.h"
#include"Game/Class/Manager/SoundManager.h"


// ステージの番号
enum class StageId {
    Stage1, // 0
    Stage2, // 1
    Stage3, // 2
    Stage4, // 3
    Stage5, // 4
    Stage6, // 5
    Max     // 5 (ステージの合計数)
};




struct GameContext
{
    // インプットマネージャー
    InputManager& inputManager;
    // グラフィックマネージャー
    GhManager& ghManager;
    // サウンドマネージャー
    SoundManager& soundManager;








    //---------------------------------------------------------------------------------------------
    // セレクトシーンで現在、選択中になっている、ステージIDを入れる変数
    int selectedStageIndex = 0;

    // 現在の StageId(ステージID) を取得する
    StageId GetSelectedStageId() const
    {
        return static_cast<StageId>(selectedStageIndex);
    }
    //---------------------------------------------------------------------------------------------
     // ステージをクリアしたかどうか
    bool stageCleared[static_cast<int>(StageId::Max)]{false};

    // 指定したステージを クリア済みにする
    void SetStageCleared(StageId stageId)
    {
        stageCleared[static_cast<int>(stageId)] = true;
    }

    // 指定したステージを クリア済みかを取得する
    bool IsStageCleared(StageId stageId) const
    {
        return stageCleared[static_cast<int>(stageId)];
    }
};
