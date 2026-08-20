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
        L"Resources/Textures/Grass.png",
        L"Resources/Textures/UFO_Bass.png",
        L"Resources/Textures/UFO_Damage_Overlay.png",
        L"Resources/Textures/UFO_Orange_All.png",
        L"Resources/Textures/UFO_Orange_Left.png",
        L"Resources/Textures/UFO_Orange_Middle.png",
        L"Resources/Textures/UFO_Orange_Right.png",
        L"Resources/Textures/Item_Food_1.png"

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
