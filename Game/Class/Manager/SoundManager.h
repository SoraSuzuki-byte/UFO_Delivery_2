/*
    @file   SoundManager.h
    @brief  サウンドを管理するクラス
    @author 鈴木蒼良
    @date   2026年9月6日
*/

#pragma once
class SoundManager
{
public:
    enum class Se {
        Se_Explosion,  // 爆発(0)
        Se_TakeDamage, // 被ダメージ音（1）
        Se_Falling,    // 落下音（2）
        Se_Delivery,   // 宅配音（3）
        Se_ResultBar,
        
        Max    // SEの合計数
    };

    enum class Bgm {
        Bgm_TitleScene,  // タイトルシーンのBGM (0)
        Bgm_SelectScene, // セレクトシーンのBGM（1）
        
        Max    // BGMの合計数
    };

private:

    // Max(SEの合計数)を int型へと変換
    static constexpr int SE_SOUND_COUNT = static_cast<int>(Se::Max);
    // 配列を宣言（SEハンドルの配列）
    int m_seSoundHandle[SE_SOUND_COUNT];

    // Max(BGMの合計数)を int型へと変換
    static constexpr int BGM_SOUND_COUNT = static_cast<int>(Bgm::Max);
    // 配列を宣言（BGMハンドルの配列）
    int m_bgmSoundHandle[BGM_SOUND_COUNT];







// メンバ関数の宣言 -------------------------------------------------
public:
    SoundManager();
    ~SoundManager();

    void Initialize();
    void Finalize();


    // 操作
public:
    void StartSe(Se se)const;
    void LoopSe(Se se)const;   // ループ再生（すでに鳴っていれば何もしない）
    void StopSe(Se se)const;   
    void StartBgm(Bgm bgm)const;
    void StopBgm(Bgm bgm)const;

    void BgmInitialize();


    // 取得/設定
public:

    // 内部実装
private:
    void DeleteSeHandle(int& handle);
    void DeleteBgmHandle(int& handle);
};

