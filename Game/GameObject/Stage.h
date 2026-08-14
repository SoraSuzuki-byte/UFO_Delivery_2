/*
    @file   Stage.h
    @brief  ステージクラス
    @author 制作者
    @date   2026/07/03
*/
#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"

class Stage
{
public:
    // マップチップの種類m_boundingBoxArray
    enum class Type
    {
        None = -1,
        Floor, Wall, Player
    };

private:
    // マップチップの大きさ
    static constexpr int CHIP_SIZE = 40;

    // CSVのパスを作るための文字列データ
    const std::wstring PATH = L"Resources/MapData/";
    const std::wstring CSV = L".csv";

private:
    // マップの幅と高さ
    int m_mapWidth;
    int m_mapHeight;

    // マップの２次元配列を動的に確保するためのダブルポインタ型変数// 画面上の見た目 用の
    Type** m_mapArray;

    // 境界ボックス配列（ワールド座標で管理）//当たり判定 用の
    BoundingBox** m_boundingBoxArray;

    // プレイヤー初期位置（ワールド座標で管理）
    Vector2D m_playerStartPosition;


public:
    Stage();
    ~Stage();

    void Initialize();
    void Render() const;
    void Finalize();

    // ワールド座標からマップ座標のマップチップの型を返す
    Stage::Type GetChipType(const Vector2D& worldPosition) const;

    // マップ座標からワールド座標の境界ボックスを計算する
    const BoundingBox& GetBoundingBox(const POINT& mapPosition) const;

    // ワールド座標をマップ座標に変換する
    POINT ConvertWorldPositionToMapPosition(const Vector2D& worldPosition);

    // getter
    const Vector2D& GetPlayerStartPosition() const { return m_playerStartPosition; }


    int GetMapWidth()  const { return m_mapWidth; }
    int GetMapHeight() const { return m_mapHeight; }
    int GetChipSize()  const { return CHIP_SIZE; }


private:
    // ステージデータをロードする
    void LoadStageData(const wchar_t* stageName);

    // マップの境界ボックスを作成する
    void CreateBoundingBoxArray();

    // 描画のサブ関数
    void StageRender() const;
    void ChestRender() const;
};