/*
    @file   GameContext.h
    @brief  ゲームコンテキストクラス（構造体）
    @author 制作者
    @date   2026/02/02
*/
#pragma once
#include "InputManager.h"
//#include "Game/GameObject/KillCount.h"
//#include "Game/GameObject/FlagManager.h"


// ステージの番号
enum class StageId {
    Stage1, // 0
    Stage2, // 1
    Stage3, // 2
    Max     // 3 (ステージの合計数)
};


struct GameContext
{
    InputManager& inputManager;

    // セレクトシーンで現在、選択中になっている、ステージIDを入れる変数
    int selectedStageIndex = 0;

    // 現在の StageId(ステージID) を取得する
    StageId GetSelectedStageId() const 
    {
        return static_cast<StageId>(selectedStageIndex);
    }




    // 画像とSoundをここで管理してみたい

    //FlagManager& flagManager;

    //KillCount& killCount;



};
