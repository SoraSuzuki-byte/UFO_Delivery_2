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
        L"Resources/Textures/Enemy_2.png" // (17)

        //  L"Resources/Textures/
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 素材を増やすたび,順番が対応するパスをここに追記!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
