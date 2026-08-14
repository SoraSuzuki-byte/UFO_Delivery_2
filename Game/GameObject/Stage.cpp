/*
    @file   Stage.cpp
    @brief  ステージクラス
    @author 制作者
    @date   2026/07/03
*/
#include "pch.h"
#include "Stage.h"
#include "Game/Screen.h"
#include <fstream>
#include <sstream>
#include <cassert>

// ------------------------------------------------------------------
// コンストラクター
// ------------------------------------------------------------------
Stage::Stage()
    : m_mapWidth{}
    , m_mapHeight{}
    , m_mapArray{}
    , m_boundingBoxArray{}
    , m_playerStartPosition{}
{
}

// ------------------------------------------------------------------
// デストラクター
// ------------------------------------------------------------------
Stage::~Stage()
{
    // newしたらdeleteする
    if (m_mapArray)
    {
        for (size_t i = 0; i < m_mapHeight; i++)
        {
            delete[] m_mapArray[i];
        }
        delete[] m_mapArray;
        m_mapArray = nullptr;
    }
    if (m_boundingBoxArray)
    {
        for (size_t i = 0; i < m_mapHeight; i++)
        {
            delete[] m_boundingBoxArray[i];
        }
        delete[] m_boundingBoxArray;
        m_boundingBoxArray = nullptr;
    }
}

// ------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------
void Stage::Initialize()
{
    LoadStageData(L"Map_Test");
    CreateBoundingBoxArray();
}

// ------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------
void Stage::Render() const
{
    StageRender();
}

// ------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------
void Stage::Finalize()
{
}

// ------------------------------------------------------------------
// ワールド座標から（マップ座標の）マップチップの型を返す
// ------------------------------------------------------------------
Stage::Type Stage::GetChipType(const Vector2D& worldPosition) const
{
    const int x = static_cast<int>(worldPosition.x) / CHIP_SIZE;
    const int y = static_cast<int>(worldPosition.y) / CHIP_SIZE;
    return m_mapArray[y][x];
}

// ------------------------------------------------------------------
// マップ座標からワールド座標の境界ボックスを返す
// ------------------------------------------------------------------
const BoundingBox& Stage::GetBoundingBox(const POINT& mapPosition) const
{
    return m_boundingBoxArray[mapPosition.y][mapPosition.x];
}

// ------------------------------------------------------------------
// ワールド座標をマップ座標に変換する
// ------------------------------------------------------------------
POINT Stage::ConvertWorldPositionToMapPosition(const Vector2D& worldPosition)
{
    return POINT{
        static_cast<int>(worldPosition.x) / CHIP_SIZE,
        static_cast<int>(worldPosition.y) / CHIP_SIZE
    };
}

// ------------------------------------------------------------------
// ステージデータをロードする
// ------------------------------------------------------------------
void Stage::LoadStageData(const wchar_t* stageName)
{
    std::ifstream ifs;      // ファイルストリーム
    std::string line;       // １行分のデータ
    std::istringstream iss; // 文字列ストリーム

    // ロードするファイルパス用の文字列を作成する
    const std::wstring filePath = PATH + stageName + CSV;

    // ファイルをオープンする
    ifs.open(filePath.c_str());

    assert(ifs.is_open() && L"CSV Open Error!");

    // CSV１行目：マップの幅と高さを取得する
    std::getline(ifs, line);                            // １行取得：ifs->line
    std::replace(line.begin(), line.end(), ',', ' ');   // 「カンマ」を「半角スペース」に置換
    iss.clear();
    iss.str(line);                                      // line->iss
    iss >> m_mapWidth >> m_mapHeight;                   // issのデータを変数に代入する、列と行を取得


    // すでにマップデータがあれば削除する
    if (m_mapArray)
    {
        for (size_t i = 0; i < m_mapHeight; i++)
        {
            delete[] m_mapArray[i];
        }
        delete[] m_mapArray;
        m_mapArray = nullptr;
    }

    // マップ配列の大きさを決定する
    m_mapArray = new Type * [m_mapHeight];
    for (int i = 0; i < m_mapHeight; i++)
    {
        m_mapArray[i] = new Type[m_mapWidth];
    }

    // CSV２行目：プレイヤーの数を取得する
    std::getline(ifs, line);
    std::replace(line.begin(), line.end(), ',', ' ');
    iss.clear();
    iss.str(line);
    int playerCount;
    iss >> playerCount;


    // CSV３行目以降：マップデータの取得
    for (int y = 0; y < m_mapHeight; y++)
    {
        // １行分のデータ
        std::getline(ifs, line);    // ifs->line
        iss.clear();
        iss.str(line);              // line->iss

        for (int x = 0; x < m_mapWidth; x++)
        {
            // カンマ区切りでデータを分割する
            std::string item;
            std::getline(iss, item, ',');   // iss->item

            // CSVの数字をマップチップのTypeに変換する
            switch (std::stoi(item))
            {
                case 0: m_mapArray[y][x] = Type::Floor;  break;
                case 1: m_mapArray[y][x] = Type::Wall;   break;
                case 2:
                {
                    m_mapArray[y][x] = Type::Player;
                    m_playerStartPosition = Vector2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    break;
                }
                default:
                    assert(!"不正なタイル番号が検知されました");
            }
        }
    }

    // ファイルをクローズする
    ifs.close();
}

// ------------------------------------------------------------------
// 境界ボックスを作成する
// ------------------------------------------------------------------
void Stage::CreateBoundingBoxArray()
{
    /* 課題１ */

    // すでに境界ボックス配列があれば削除する
    if (m_boundingBoxArray) {
        for (int i = 0; i < m_mapHeight; i++) {
            delete[] m_boundingBoxArray[i];
        }
        delete[] m_boundingBoxArray;
        m_boundingBoxArray = nullptr;
    }

    // 境界ボックス配列の大きさを決定する
    m_boundingBoxArray = new BoundingBox * [m_mapHeight];
    for (int i = 0; i < m_mapHeight; i++) {
        m_boundingBoxArray[i] = new BoundingBox[m_mapWidth];
    }

    // 壁に該当する部分に境界ボックスを作成する
    for (int y = 0; y < m_mapHeight; y++) {
        for (int x = 0; x < m_mapWidth; x++) {
            // 壁のとき 境界ボックスを作成する
            if (m_mapArray[y][x] != Type::Wall) { continue; }

            const Vector2D minVec2D{ static_cast<float>(x) * CHIP_SIZE,static_cast<float>(y) * CHIP_SIZE };
            const Vector2D maxVec2D{ minVec2D.x + CHIP_SIZE, minVec2D.y + CHIP_SIZE };

            m_boundingBoxArray[y][x] = BoundingBox{ minVec2D,maxVec2D };
        }
    }
}

// ------------------------------------------------------------------
// ステージを描画する
// ------------------------------------------------------------------
void Stage::StageRender() const
{

    for (int y = 0; y < m_mapHeight; y++)
    {
        for (int x = 0; x < m_mapWidth; x++)
        {
            // 壁・足場（Type::Wall）を描画
            if (m_mapArray[y][x] == Type::Wall)
            {
                DrawBox(
                    x * CHIP_SIZE,
                    y * CHIP_SIZE,
                    x * CHIP_SIZE + CHIP_SIZE,
                    y * CHIP_SIZE + CHIP_SIZE,
                    GetColor(255, 255, 255), TRUE); // ★白の塗りつぶしで描画
            }
        }
    }
}

// ------------------------------------------------------------------
// 宝箱を描画する
// ------------------------------------------------------------------
//void Stage::ChestRender(int scroll) const
//{
//    for (const auto& chest : m_chests)
//    {
//        if (chest.GetActiveFlag())
//        {
//            chest.Render(scroll);
//        }
//    }
//}
