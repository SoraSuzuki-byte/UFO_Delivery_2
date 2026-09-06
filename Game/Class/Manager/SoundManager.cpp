/*
    @file   SoundManager.h
    @brief  サウンドを管理するクラス
    @author 鈴木蒼良
    @date   2026年9月6日
*/


#include "pch.h"
#include "SoundManager.h"
#include <cassert>


SoundManager::SoundManager()//------------------------------------------------------
{
    // 配列の数字を生成するときに、すべて指定
    for (auto& handle : m_seSoundHandle)
    {
        handle = -1;
    }
    for (auto& handle : m_bgmSoundHandle)
    {
        handle = -1;
    }
}

SoundManager::~SoundManager()//------------------------------------------------------
{
    for (auto& handle : m_seSoundHandle)
    {
        DeleteSeHandle(handle);
    }
    for (auto& handle : m_bgmSoundHandle)
    {
        DeleteBgmHandle(handle);
    }
}

void SoundManager::Initialize()//------------------------------------------------------
{
    for (auto& handle : m_seSoundHandle)
    {
        DeleteSeHandle(handle);
    }
    for (auto& handle : m_bgmSoundHandle)
    {
        DeleteBgmHandle(handle);
    }

    // 画像のパスを配列で管理（enum の順番と合わせることに注意）
    const wchar_t* sePaths[] = {
        L"Resources/Sounds/Se_Explosion.mp3", // (0)
        L"Resources/Sounds/Se_TakeDamage.mp3", // (1)
        L"Resources/Sounds/Se_Falling.mp3", // (2)
        L"Resources/Sounds/Se_Delivery.mp3", // (3)
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 素材を増やすたび,順番が対応するパスをここに追記!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    };
    const wchar_t* bgmPaths[] = {
        L"Resources/Sounds/Bgm_TitleScene.mp3", // (0)
        L"Resources/Sounds/Bgm_SelectScene.mp3", // (1)
        L"Resources/Sounds/Bgm_Stage1.mp3", // (2)
        L"Resources/Sounds/Bgm_Stage2.mp3", // (3)
        L"Resources/Sounds/Bgm_Stage3.mp3", // (4)
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 素材を増やすたび,順番が対応するパスをここに追記!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    };

    // 【安全チェック】enumの登録数 と パス文字列の数 が一致しているかビルド時に確認→パスの追加忘れや enum の書き換えミスによるメモリクラッシュを未然に防げる
   static_assert(sizeof(sePaths) / sizeof(sePaths[0]) == SE_SOUND_COUNT, "enum Se の要素数と sePaths の要素数が一致していません！");
   static_assert(sizeof(bgmPaths) / sizeof(bgmPaths[0]) == BGM_SOUND_COUNT, "enum Bgm の要素数と bgmPaths の要素数が一致していません！");


   // Seのロード
    for (size_t i = 0; i < static_cast<size_t>(Se::Max); ++i) 
    {
        m_seSoundHandle[i] = LoadSoundMem(sePaths[i]);
        assert(m_seSoundHandle[i] != -1 && "seのロードに失敗しました");
    }
   // Bgmのロード
    for (size_t i = 0; i < static_cast<size_t>(Bgm::Max); ++i) 
    {
        m_bgmSoundHandle[i] = LoadSoundMem(bgmPaths[i]);
        assert(m_bgmSoundHandle[i] != -1 && "bgmのロードに失敗しました");
    }
}

void SoundManager::Finalize()//------------------------------------------------------
{
    for (auto& handle : m_seSoundHandle)
    {
        DeleteSoundMem(handle);
    }
    for (auto& handle : m_bgmSoundHandle)
    {
        DeleteSoundMem(handle);
    }
}




// ------------------------------------------------------------------
// Seを鳴らす
/// <param name="se">鳴らしたいSeハンドルへの参照</param>// ------------------------------------------------------------------
void SoundManager::SoundSe(Se se)const
{
    PlaySoundMem(m_seSoundHandle[static_cast<int>(se)], DX_PLAYTYPE_BACK);
}


// ------------------------------------------------------------------
// Bgmをはじめる
/// <param name="bgm">はじめるBgmハンドルへの参照</param>// ------------------------------------------------------------------
void SoundManager::StartBgm(Bgm bgm)const
{
    int index = static_cast<int>(bgm);
    int handle = m_bgmSoundHandle[index];

    // ハンドルが無効なら処理しない
    if (handle == -1) return;

    // すでに再生中（CheckSoundMem == 1）なら何もしない（最初から鳴らし直さない）
    if (CheckSoundMem(handle) == 1)
    {
        return;
    }

    // 再生中でない場合のみ、再生する
    PlaySoundMem(handle, DX_PLAYTYPE_LOOP, TRUE);
}


// ------------------------------------------------------------------
// Bgmを止める
/// <param name="bgm">止めるBgmハンドルへの参照</param>// ------------------------------------------------------------------
void SoundManager::EndBgm(Bgm bgm)const
{
    StopSoundMem(m_bgmSoundHandle[static_cast<int>(bgm)]);
}




// ------------------------------------------------------------------
// Seハンドルを安全に解放する
/// <param name="handle">解放するSeハンドルへの参照</param>// ------------------------------------------------------------------
void SoundManager::DeleteSeHandle(int& handle)
{
    if (handle != -1)
    {
        DeleteSoundMem(handle);
        handle = -1;// 参照渡しなので呼び出し元の要素が -1 に書き換わる
    }
}



// ------------------------------------------------------------------
// Bgmハンドルを安全に解放する
/// <param name="handle">解放するBgmハンドルへの参照</param>// ------------------------------------------------------------------
void SoundManager::DeleteBgmHandle(int& handle)
{
    StopSoundMem(handle);
    if (handle != -1)
    {
        DeleteSoundMem(handle);
        handle = -1;// 参照渡しなので呼び出し元の要素が -1 に書き換わる
    }
}

