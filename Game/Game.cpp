/**
 * @file   Game.cpp
 *
 * @brief  ゲーム全体に関するソースファイル
 *
 * @author 制作者名
 *
 * @date   日付
 */

 // ヘッダファイルの読み込み ===================================================
#include "Game.h"

#include "Game/Screen.h"


// メンバ関数の定義 ===========================================================
/**
 * @brief デフォルトコンストラクタ
 *
 * @param なし
 */
Game::Game()
    : m_gameContext{ m_inputManager }
    , m_sceneManager{ m_gameContext }
{
    // 乱数の初期値を設定
    SRand(static_cast<int>(time(nullptr)));
}



/**
 * @brief デストラクタ
 */
Game::~Game()
{

}



/**
 * @brief 初期化処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Initialize()
{
    // 入力マネジャーを初期化する
    m_gameContext.inputManager.Initialize();

    // シーンマネジャーを初期化する
    m_sceneManager.Initialize();
}



/**
 * @brief 更新処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Update(float elapsedTime)
{
    // 入力マネジャーを更新する
    m_gameContext.inputManager.Update();

    // シーンマネジャーを更新する
    m_sceneManager.Update();
}



/**
 * @brief 描画処理
 *
 * @param[in] なし
 *
 * @return なし
 */
void Game::Render()
{
    // シーンを描画する
    m_sceneManager.Render();
}



/**
 * @brief 終了処理
 *
 * @param なし
 *
 * @return なし
 */
void Game::Finalize()
{
    // シーンマネジャーを終了する
    m_sceneManager.Finalize();
}
