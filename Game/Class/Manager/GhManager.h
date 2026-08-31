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
        Wall, // 草(0)
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
        Enemy_2, // (17)


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

