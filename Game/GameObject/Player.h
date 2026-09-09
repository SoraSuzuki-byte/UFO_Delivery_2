/*
    @file   Stage.h
    @brief  プレイヤークラス
    @author 鈴木蒼良
    @date   2026年8月16日
*/
#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"
#include <vector>



// 前方宣言 ===============================================================
struct GameContext;
class Item_Food;
class Stage;


class Player
{
private:
    static constexpr const float SPEED = 0.05f;// 速度
    static constexpr const float WIDTH = 51;  // 横幅
    static constexpr const float HEIGHT = 26; // 縦幅

    static constexpr float ACCEL = 0.27f;  // 加速度（大きくするとクイックに動き出す）
    static constexpr float FRICTION = 0.94f; // 摩擦係数（1に近いほどよく滑る。0.85〜0.95あたりがおすすめ）
    static constexpr float MAX_SPEED = 10.0f;  // 最高速度

    static constexpr float BOUNCE_FACTOR = 1.2f;  // 跳ね返りの強さ（1.0で等倍、大きくすると強く跳ね返る）

    static constexpr int INVINCIBLE_TIME = 120;  // 無敵時間
    static constexpr int BLINK_INTERVAL = 2; // 点滅の速さ(間隔）
    static constexpr int MAX_HP = 5;  // 最大HP


    // ゲームコンテキストのリファレンス　（インスタンス・実体ではない
    GameContext& m_gameContext;

    Stage* m_stage; // PlayScene&ではなくStage*にする（nullptr許容）  


    // 現在の位置
    Vector2D m_position;
    // 現在の移動量
    Vector2D m_velocity;
    // 現在の加速度
    Vector2D m_acceleration;

    // 移動のアニメーションに使うTimer
    int m_animationTimer;
    // 現在のHP
    int m_hp;
     // 無敵タイマー（0より大きい間は無敵）
    int m_invincibleTimer;    

    // スペースキーを押しているか
    bool m_isPullingInput;






public:
    Player(GameContext& gameContext, Stage* stage = nullptr);// stage は nullptr でも良い（例：タイトル画面など、壁判定が不要な場面）
    ~Player();

    // startPosition は stage が nullptr のときのみ使われる
    void Initialize(const Vector2D& startPosition = Vector2D{ 0.0f, 0.0f });
    void Update();
    void Render();
    void Finalize();

    void SetStage(Stage* stage) { m_stage = stage; }

    // 「通常入力」と「タイトルシーンでの自動で移動」の２つのモード
    enum class ControlMode
    {
        Manual, // キー入力で操作するモード
        AutoDemo, //タイトルシーンで自動で動くモード
    };
    // コントロールモードのセッター
    void SetControlMode(ControlMode mode) { m_controlMode = mode; }

    // 現在、吸引ボタンに相当する入力が入っているか（手動操作でもデモでも共通で使う）
    bool IsPullingInput() const { return m_isPullingInput; }





    // HPのゲッター　プレイシーンで「HPが0になったらリザルト表示」をするため
    int GetHp() const { return m_hp; }
    // m_positionのゲッター
    Vector2D GetPosition() const { return m_position; }
    // m_positionのセッター
    Vector2D SetPosition(Vector2D add) { m_position += add; }

    // UFOの見た目の中心座標を取得する
    Vector2D GetCenterPosition() const
    {
        return Vector2D{ m_position.x + WIDTH / 2.0f, m_position.y + HEIGHT / 2.0f };
    }


    // 当たり判定用の境界ボックスを取得する
    BoundingBox GetHurtBox() const
    {
        return BoundingBox{
            m_position,
            Vector2D{ m_position.x + WIDTH, m_position.y + HEIGHT }
        };
    }

    // UFOを移動させる
    void Move(int keyCondition);
    // 画面外に出ないようにする
    void ClampPositionToScreen();
    // 当たった時の処理
    void OnCollision();
    // HPを減らす
    void TakeDamage();


    // 移動時のアニメーション
    void MoveAnimation();
    // ダメージ表現のオーバーレイ
    void DrawDamageOverlay();





    private: // 内部処理--------------------------------------------

        ControlMode m_controlMode; // 変数宣言

        // ---- タイトルデモ用のステート管理 ----
        enum class DemoState
        {
            MoveLeft,   // 指定位置まで左に移動
            Pulling,    // その場で吸引
            MoveRight,  // 指定位置まで右に移動
            Idle,       // 吸引をやめて停止
        };
        DemoState m_demoState = DemoState::MoveLeft;
        int       m_demoTimer;

        // タイトルシーンでのデモ用の目標座標
        static constexpr float DEMO_LEFT_TARGET_X = 210.0f; // ここまで左に移動する
        static constexpr float DEMO_RIGHT_TARGET_X = 1100.0f; // ここまで右に移動する
        static constexpr int   DEMO_PULL_DURATION = 240;     // 吸引し続けるフレーム数
        static constexpr float DEMO_POSITION_TOLERANCE = 5.0f; // 目標位置とみなす誤差

        // タイトルデモ時の速度倍率（本編の難易度には影響しない）
        static constexpr float DEMO_SPEED_SCALE = 0.2f;

        int GenerateAutoInput();

};

