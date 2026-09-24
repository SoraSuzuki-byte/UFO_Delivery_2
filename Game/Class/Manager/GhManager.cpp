/*
    @file   GhManager.h
    @brief  グラフィックを管理するクラス
    @author 鈴木蒼良
    @date   2026年8月15日
*/


#include "pch.h"
#include "GhManager.h"
#include <cassert>


GhManager::GhManager()//------------------------------------------------------
{
    // 配列の数字を生成するときに、すべて指定
    for (auto& handle : m_graphicHandle)
    {
        handle = -1;
    }
}

GhManager::~GhManager()//------------------------------------------------------
{
    for (auto& handle : m_graphicHandle)
    {
        DeleteGraphicHandle(handle);
    }
}

void GhManager::Initialize()//------------------------------------------------------
{
    // 画像のパスを配列で管理（enum の順番と合わせることに注意）
    const wchar_t* paths[] = {
        L"Resources/Textures/Wall.png", // (0)
        L"Resources/Textures/UFO_Bass.png", // (1) 
        L"Resources/Textures/UFO_Damage_Overlay.png", // (2)
        L"Resources/Textures/UFO_Orange_All.png", // (3)
        L"Resources/Textures/UFO_Orange_Left.png", // (4)
        L"Resources/Textures/UFO_Orange_Middle.png", // (5)
        L"Resources/Textures/UFO_Orange_Right.png", // (6)
        L"Resources/Textures/House.png", // (7)
        L"Resources/Textures/Background_1.png", // (8)
        L"Resources/Textures/Item_Food_1.png", // (9)
        L"Resources/Textures/Item_Food_2.png", // (10)
        L"Resources/Textures/Item_Food_3.png", // (11)
        L"Resources/Textures/Item_Food_4.png", // (12)
        L"Resources/Textures/Item_Food_5.png", // (13)
        L"Resources/Textures/Item_Bomb.png", // (14)
        L"Resources/Textures/Explosion.png", // (15)
        L"Resources/Textures/Enemy_1.png" , // (16)
        L"Resources/Textures/Enemy_1_1.png" , // (17)
        L"Resources/Textures/Enemy_2.png" , // (18)
        L"Resources/Textures/Enemy_2_1.png" , // (19)
        L"Resources/Textures/Enemy_3.png" , // (20)
        L"Resources/Textures/Enemy_3_1.png" , // (21)
        L"Resources/Textures/Background_2.png", // (22)
        L"Resources/Textures/Background_3.png", // (23)
        L"Resources/Textures/Background_SelectScene.png", // (24)
        L"Resources/Textures/Stage1_a.png", // (25)
        L"Resources/Textures/Stage1_b.png", // (26)
        L"Resources/Textures/Stage2_a.png", // (27)
        L"Resources/Textures/Stage2_b.png", // (28)
        L"Resources/Textures/Stage3_a.png", // (29)
        L"Resources/Textures/Stage3_b.png", // (30)
        L"Resources/Textures/Stage4_a.png", // (31)
        L"Resources/Textures/Stage4_b.png", // (32)
        L"Resources/Textures/Stage5_a.png", // (33)
        L"Resources/Textures/Stage5_b.png", // (34)
        L"Resources/Textures/Stage6_a.png", // (35)
        L"Resources/Textures/Stage6_b.png", // (36)
        L"Resources/Textures/Stage7_a.png", // (37)
        L"Resources/Textures/Stage7_b.png", // (38)
        L"Resources/Textures/Stage8_a.png", // (39)
        L"Resources/Textures/Stage8_b.png", // (40)
        L"Resources/Textures/LeftArrow_None.png", // (41)
        L"Resources/Textures/LeftArrow_Push.png", // (42)
        L"Resources/Textures/RightArrow_None.png", // (43)
        L"Resources/Textures/RightArrow_Push.png", // (44)
        L"Resources/Textures/DownArrow_None.png", // (45)
        L"Resources/Textures/DownArrow_Push.png", // (46)
        L"Resources/Textures/Background_4.png", // (47)
        L"Resources/Textures/Background_5.jpg", // (48)
        L"Resources/Textures/logo.png", // (49)
        L"Resources/Textures/Background_6.png", // (50)
        L"Resources/Textures/Background_7.png", // (51)
        L"Resources/Textures/Background_8.png", // (52)
        L"Resources/Textures/Background_SelectScene2_before.png", // (53)
        L"Resources/Textures/Background_SelectScene2_after.png", // (54)
        L"Resources/Textures/Item_Food_6.png", // (55)
        L"Resources/Textures/Item_Food_7.png", // (56)
        L"Resources/Textures/Item_Food_8.png", // (57)
        L"Resources/Textures/SpawnEffect.png", // (58)
        L"Resources/Textures/BrokenHouse.png" // (59)




    };

    // 【安全チェック】enumの登録数 と パス文字列の数 が一致しているかビルド時に確認→パスの追加忘れや enum の書き換えミスによるメモリクラッシュを未然に防げる
   static_assert(sizeof(paths) / sizeof(paths[0]) == TEXTURE_COUNT, "enum Textures の要素数と paths の要素数が一致していません！");


    for (size_t i = 0; i < static_cast<size_t>(Textures::Max); ++i) 
    {
        m_graphicHandle[i] = LoadGraph(paths[i]);
        assert(m_graphicHandle[i] != -1 && "画像のロードに失敗しました");
    }
}

void GhManager::Finalize()//------------------------------------------------------
{
    for (auto& handle : m_graphicHandle)
    {
        DeleteGraphicHandle(handle);
    }
}


// ------------------------------------------------------------------
// 描画したいテクスチャを取得する
/// <param name="texture">描画を依頼されるテクスチャ名</param>
// ------------------------------------------------------------------
int GhManager::GetGraphicHandle(Textures texture)
{
    // 指定されたテクスチャに対応するハンドルを、配列から取得
    return m_graphicHandle[static_cast<int>(texture)];// 依頼された「texture」をint型に変換して、returnする
}




// ------------------------------------------------------------------
// グラフィックハンドルを安全に解放する
/// <param name="gh">解放するグラフィックハンドルへの参照</param>// ------------------------------------------------------------------
void GhManager::DeleteGraphicHandle(int& gh)
{
    if (gh != -1)
    {
        DeleteGraph(gh);
        gh = -1;// 参照渡しなので呼び出し元の要素が -1 に書き換わる
    }
}
