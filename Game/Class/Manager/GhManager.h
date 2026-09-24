/*
    @file   GhManager.h
    @brief  グラフィックを管理するクラス
    @author 鈴木蒼良
    @date   2026年8月15日
*/

#pragma once
class GhManager
{
public:
    enum class Textures {
        Wall, // 壁(0)
        UFO_Bass, // UFOの素体（1）
        UFO_Damage_Overlay,   // UFOのHPに合わせて透明度が変わるやつ（2）
        UFO_Orange_All,    // オレンジの全部（3）
        UFO_Orange_Left,   // オレンジのひだり（4）
        UFO_Orange_Middle, // オレンジの中央（5）
        UFO_Orange_Right,  // オレンジのみぎ（6）
        House, // 家 (7)
        Background_1, // 背景画像 (8)
        Item_Food_1, // (9)
        Item_Food_2, // (10)
        Item_Food_3, // (12)
        Item_Food_4, // (12)
        Item_Food_5, // (13)
        Item_Bomb, // 爆弾アイテム (14)
        Explosion, // 爆発エフェクト (15)
        Enemy_1, // (16)
        Enemy_1_1, // (17)
        Enemy_2, // (19)
        Enemy_2_1, // (18)
        Enemy_3, // (20)
        Enemy_3_1, // (21)
        Background_2, // 背景画像 (22)
        Background_3, // 背景画像 (23)
        Background_SelectScene, // セレクトシーンの背景画像 (24)
        Stage1_a, // ステージ1のアフター画像(25)
        Stage1_b, // ステージ1のビフォー画像(26)
        Stage2_a, // ステージ2のアフター画像(27)
        Stage2_b, // ステージ2のビフォー画像(28)
        Stage3_a, // ステージ3のアフター画像(29)
        Stage3_b, // ステージ3のビフォー画像(30)
        Stage4_a, // ステージ4のアフター画像(31)
        Stage4_b, // ステージ4のビフォー画像(32)
        Stage5_a, // ステージ5のアフター画像(33)
        Stage5_b, // ステージ5のビフォー画像(34)
        Stage6_a, // ステージ6のアフター画像(35)
        Stage6_b, // ステージ6のビフォー画像(36)
        Stage7_a, // ステージ7のアフター画像(37)
        Stage7_b, // ステージ7のビフォー画像(38)
        Stage8_a, // ステージ8のアフター画像(39)
        Stage8_b, // ステージ8のビフォー画像(40)
        LeftArrow_None, // デフォルトの左キー(41)
        LeftArrow_Push, // 押されている左キー(42)
        RightArrow_None, // デフォルトの右キー(43)
        RightArrow_Push, // 押されている右キー(44)
        DownArrow_None, // デフォルトの下キー(45)
        DownArrow_Push, // 押されている下キー(46)
        Background_4, // 背景画像 (47)
        Background_5, // 背景画像 (48)
        Logo, // タイトル画面のロゴ(49)
        Background_6, // 背景画像 (50)
        Background_7, // 背景画像 (51)
        Background_8, // 背景画像 (52)
        Background_SelectScene2_before,// セレクトシーンの背景画像 (53)
        Background_SelectScene2_after,// セレクトシーンの背景画像 (54)
        Item_Food_6, // (55)
        Item_Food_7, // (56)
        Item_Food_8, // (57)
        SpawnEffect, // (58)


        Max    // テクスチャの合計数
    };

private:

    // Max(テクスチャの合計数)を int型へと変換
    static constexpr int TEXTURE_COUNT = static_cast<int>(Textures::Max);

    // 配列を宣言（グラフィックハンドルの配列）
    int m_graphicHandle[TEXTURE_COUNT];







// メンバ関数の宣言 -------------------------------------------------
public:
    GhManager();
    ~GhManager();

    void Initialize();
    void Finalize();

// 取得/設定
public:
    int GetGraphicHandle(Textures texture);
    /*
    void DrawGraphic(Vector2D position, POINT size, int srcX, int srcY, int srcWidth, int srcHeight, Textures texture);
    void DrawGraphic(POINT position, POINT size, int srcX, int srcY, int srcWidth, int srcHeight, Textures texture)
    {
        DrawGraphic(Vector2D{ static_cast<float>(position.x),static_cast<float>(position.y) },
            size, srcX, srcY, srcWidth, srcHeight, texture);
    }
    void DrawRotateGraphic(Vector2D position, POINT size, int srcX, int srcY, int srcWidth, int srcHeight,
        POINT rotateCenter, float angle, Textures texture);
    void DrawRotateGraphic(POINT position, POINT size, int srcX, int srcY, int srcWidth, int srcHeight,
        POINT rotateCenter, float angle, Textures texture) {
        DrawRotateGraphic(Vector2D{ static_cast<float>(position.x),static_cast<float>(position.y) },
            size, srcX, srcY, srcWidth, srcHeight, rotateCenter, angle, texture);
    }
    */


// 内部実装
private:
    void DeleteGraphicHandle(int& gh);
};

