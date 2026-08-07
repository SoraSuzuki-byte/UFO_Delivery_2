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


// いろいろなところで使いたいリソースをまとめたクラス（構造体）
struct GameContext
{
    // TODO: サウンドとかリソースとか、これから追加する

    InputManager& inputManager;

    // 画像とSoundをここで管理してみたい

    //FlagManager& flagManager;

    //KillCount& killCount;



};
