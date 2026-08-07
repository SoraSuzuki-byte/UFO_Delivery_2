/*
    @file   InputManager.h
    @brief  キーボードやジョイパッドからの入力を受け付けるクラス
    @author 制作者
    @date   2026/04/30
*/
#pragma once

class InputManager
{
private:
    int m_key;      // 現フレームの入力情報
    int m_oldKey;   // ひとつ前のフレーム入力情報
    int m_trigger;  // トリガー情報

public:
    InputManager() = default;
    ~InputManager() = default;

    void Initialize();
    void Update();

    int GetKeyCondition() const;
    int GetKeyTrigger() const;
};
