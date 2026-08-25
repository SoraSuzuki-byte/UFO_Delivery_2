/*
    @file   GameContext.h
    @brief  ゲームコンテキストクラス（構造体）
    @author 制作者
    @date   2026/02/02
*/
#pragma once
#include "Game/Class/Manager/InputManager.h"
#include "Game/Class/Manager/GhManager.h"


// ステージの番号
enum class StageId {
    Stage1, // 0
    Stage2, // 1
    Stage3, // 2
    Max     // 3 (ステージの合計数)
};


struct GameContext
{
    // インプットマネージャー
    InputManager& inputManager;
    // グラフィックマネージャー
    GhManager& ghManager;
    // サウンドマネージャー








    //---------------------------------------------------------------------------------------------
    // セレクトシーンで現在、選択中になっている、ステージIDを入れる変数
    int selectedStageIndex = 0;

    // 現在の StageId(ステージID) を取得する
    StageId GetSelectedStageId() const
    {
        return static_cast<StageId>(selectedStageIndex);
    }
    //---------------------------------------------------------------------------------------------

};
