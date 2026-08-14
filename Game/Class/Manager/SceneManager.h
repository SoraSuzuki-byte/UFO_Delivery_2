/**
 * @file   SceneManager.h
 *
 * @brief  シーンマネージャーの、ヘッダファイル
 *
 * @author 鈴木蒼良
 *
 * @date   2026年8月6日
 */
#pragma once

#include "Game/Class/Scene/TitleScene.h"
#include "Game/Class/Scene/SelectScene.h"
#include "Game/Class/Scene/PlayScene.h"
//#include "Game/Class/Scene/SceneChange.h"

 // 前方宣言 ===============================================================
struct GameContext;




class SceneManager
{
	// 公開する定数や列挙型の宣言 ---------------------------------------
public:
	enum class SceneID // シーンID
	{
		None,
		TitleScene,
		SelectScene,
		PlayScene,
	};


private:
	// ゲームコンテキストのインスタンス
	GameContext& m_gameContext;

	// それぞれのシーンのインスタンス
	TitleScene m_titleScene;
	SelectScene m_selectScene;
	PlayScene m_playScene;

	// シーンID
	SceneID m_currentSceneID;
	SceneID m_requestedSceneID;


public:
	SceneManager(GameContext& gameContext);
	~SceneManager() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();
	// 次のシーンをリクエストする
	void RequestNextSceneID(SceneID requestSceneID);

	// 内部実装
private:
	// シーンを変更する
	void ChangeScene();

	// 現在シーンの初期化、更新、描画、終了処理
	void InitializeCurrentScene();
	void UpdateCurrentScene();
	void RenderCurrentScene();
	void FinalizeCurrentScene();


};

