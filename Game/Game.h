/**
 * @file   Game.h
 *
 * @brief  ゲーム全体に関するヘッダファイル
 *
 * @author 制作者名
 *
 * @date   日付
 */

 // 多重インクルードの防止 =====================================================
#pragma once

// ヘッダファイルの読み込み ===================================================
#include "GameContext.h"
#include "Class/Manager/SceneManager.h"


// クラスの宣言 ===============================================================


// クラスの定義 ===============================================================
/**
 * @brief ゲーム
 */
class Game
{
    // クラス定数の宣言 -------------------------------------------------
public:
    // システム関連
    static constexpr const wchar_t* TITLE = L"UFO_Delivery";   ///< ゲームタイトル


    // データメンバの宣言 -----------------------------------------------
private:
    // ゲームコンテキスト
    GameContext m_gameContext;
    // 入力関連
    InputManager m_inputManager;


    // シーンマネジャー
    SceneManager m_sceneManager;



    // メンバ関数の宣言 -------------------------------------------------
    // コンストラクタ/デストラクタ
public:
    // コンストラクタ
    Game();

    // デストラクタ
    ~Game();


    // 操作
public:
    // 初期化処理
    void Initialize();

    // 更新処理
    void Update(float elapsedTime);

    // 描画処理
    void Render();

    // 終了処理
    void Finalize();


    // 取得/設定
public:


    // 内部実装
private:


};
