/*
    @file   Stage.h
    @brief  プレイヤークラス
    @author 鈴木蒼良
    @date   2026年8月16日
*/
#pragma once
#include "Library/GameMath.h"



// 前方宣言 ===============================================================
struct GameContext;
class PlayScene;



class Player
{
private:
    static constexpr const float SPEED = 0.05f;// 速度
    static constexpr const float WIDTH = 51;  // 横幅
    static constexpr const float HEIGHT = 26; // 縦幅

    static constexpr float ACCEL = 0.27f;  // 加速度（大きくするとクイックに動き出す）
    static constexpr float FRICTION = 0.94f; // 摩擦係数（1に近いほどよく滑る。0.85〜0.95あたりがおすすめ）
    static constexpr float MAX_SPEED = 10.0f;  // 最高速度




    // ゲームコンテキストのリファレンス　（インスタンス・実体ではない
    GameContext& m_gameContext;

    // ステージクラスのリファレンス
    PlayScene& m_scene;          


    // 現在の位置
    Vector2D m_position;
    // 現在の移動量
    Vector2D m_velocity;
    // 現在の加速度
    Vector2D m_acceleration;



public:
    Player(GameContext& gameContext, PlayScene& scene);
    ~Player();

    void Initialize();
    void Update();
    void Render();
    void Finalize();

    // UFOを動かす
    void Move();

};

