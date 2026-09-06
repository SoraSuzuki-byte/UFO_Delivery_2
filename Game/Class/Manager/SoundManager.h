/*
    @file   SoundManager.h
    @brief  グラフィックを管理するクラス
    @author 鈴木蒼良
    @date   2026年8月15日
*/

#pragma once
class SoundManager
{
public:
    enum class Se {
        Wall, // 草(0)
        UFO_Bass, // UFOの素体（1）
        UFO_Damage_Overlay,   // UFOのHPに合わせて透明度が変わるやつ（2）
        UFO_Orange_All,    // オレンジの全部（3）
        
        Max    // SEの合計数
    };

    enum class Bgm {
        Wall, // 草(0)
        UFO_Bass, // UFOの素体（1）
        UFO_Damage_Overlay,   // UFOのHPに合わせて透明度が変わるやつ（2）
        UFO_Orange_All,    // オレンジの全部（3）
        
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
    void StartBgm(Bgm bgm)const;
    void EndBgm(Bgm bgm)const;
    void SoundSe(Se se)const;


    // 取得/設定
public:

    // 内部実装
private:
    void DeleteSeHandle(int& handle);
    void DeleteBgmHandle(int& handle);
};

