/*
    @file   InputManager.cpp
    @brief  キーボードやジョイパッドからの入力を受け付けるクラス
    @author 制作者
    @date   2026/04/30
*/
#include "InputManager.h"

//  -----------------------------------------------------------------
/// <summary>
/// 初期化処理
/// </summary>
/// -----------------------------------------------------------------
void InputManager::Initialize()
{
    m_key = 0;
    m_oldKey = 0;
    m_trigger = 0;
}

//  -----------------------------------------------------------------
/// <summary>
/// 更新処理
/// </summary>
/// -----------------------------------------------------------------
void InputManager::Update()
{
    m_oldKey = m_key;
    m_key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
    m_trigger = (~m_oldKey & m_key);
}

//  -----------------------------------------------------------------
/// <summary>
/// 現フレームでのキー入力情報を取得する
/// </summary>
/// <returns>キー入力情報</returns>
/// -----------------------------------------------------------------
int InputManager::GetKeyCondition() const
{
    return m_key;
}

//  -----------------------------------------------------------------
/// <summary>
/// トリガー情報を取得する
/// </summary>
/// <returns>トリガー情報</returns>
/// -----------------------------------------------------------------
int InputManager::GetKeyTrigger() const
{
    return m_trigger;
}
