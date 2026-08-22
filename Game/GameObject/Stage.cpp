/*
    @file   Stage.cpp
    @brief  ステージクラス
    @author 制作者
    @date   2026/07/03
*/
#include "pch.h"
#include "Stage.h"
#include "Game/GameContext.h"

#include "Game/Screen.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>   // ★ std::max, std::min のために追加

// ------------------------------------------------------------------
// コンストラクター
// ------------------------------------------------------------------
Stage::Stage(GameContext& gameContext)
    : m_gameContext{ gameContext }
    , m_mapWidth{}
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
/// <param name="stageNumber">セレクトシーンで選んだステージID</param>
// ------------------------------------------------------------------
void Stage::Initialize(const wchar_t* stageNumber)
{
    LoadStageData(stageNumber);
    m_gameContext.ghManager.Initialize();

    CreateBoundingBoxArray();
}

// ------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------
void Stage::Update()
{
    for (auto& item : m_itemFood_1)
    {
        item.Update();
    }
}

// ------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------
void Stage::Render() const
{
    ItemFood_1Render();
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

    // マップ範囲外なら安全に None を返す
    if (x < 0 || x >= m_mapWidth || y < 0 || y >= m_mapHeight)
    {
        return Type::None;
    }

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

    

    // CSVを読み込む前に、以前のステージのアイテム情報を空（0個）にする
    m_itemFood_1.clear();
    
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
                case 3:  
                {
                    m_mapArray[y][x] = Type::ItemFood_1;

                    const Vector2D minVec2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    const Vector2D maxVec2D{ minVec2D.x + CHIP_SIZE, minVec2D.y + CHIP_SIZE };
                    BoundingBox bb{ minVec2D, maxVec2D };

                    m_itemFood_1.emplace_back(Item_Food_1(m_gameContext, *this, m_player, bb)); // bbは BoundingBox
                    m_itemFood_1.back().Initialize();
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
    // 見た目（三角形）より当たり判定を 一回り小さくするための定数を
    const float margin = CHIP_SIZE * 0.25f;


    // すでに境界ボックス配列があれば削除
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
    for (int y = 0; y < m_mapHeight; y++) 
    {
        for (int x = 0; x < m_mapWidth; x++) {
            // 壁のとき 境界ボックスを作成する
            if (m_mapArray[y][x] != Type::Wall) { continue; }

            // 余白ぶん内側にずらして、小さめの当たり判定にする
            const Vector2D minVec2D
            {
                static_cast<float>(x) * CHIP_SIZE + margin,
                static_cast<float>(y) * CHIP_SIZE + margin
            };
            const Vector2D maxVec2D
            {
                minVec2D.x + CHIP_SIZE - margin * 2,
                minVec2D.y + CHIP_SIZE - margin * 2
            };

            m_boundingBoxArray[y][x] = BoundingBox{ minVec2D,maxVec2D };
        }
    }
}


// ------------------------------------------------------------------
// プレイヤーと壁チップとの当たり判定を行い、押し戻し量を返す
// メモ→　std::max と std::min は、C++の標準ライブラリ（<algorithm> ヘッダー）で用意されている2つの値を比較して「大きい方」または「小さい方」を返す関数
// ------------------------------------------------------------------
Vector2D Stage::ResolveWallCollision(const BoundingBox& playerBox) const
{
    // 調べるべきマップ座標の範囲を割り出す（前後1マス多めに調べて漏れを防ぐ）
    const int minX = std::max(0, static_cast<int>(playerBox.minPosition.x) / CHIP_SIZE - 1);
    const int maxX = std::min(m_mapWidth - 1, static_cast<int>(playerBox.maxPosition.x) / CHIP_SIZE + 1);
    const int minY = std::max(0, static_cast<int>(playerBox.minPosition.y) / CHIP_SIZE - 1);
    const int maxY = std::min(m_mapHeight - 1, static_cast<int>(playerBox.maxPosition.y) / CHIP_SIZE + 1);

    // 押し戻し量の合計
    Vector2D offset{ 0.0f, 0.0f };

    // 押し戻しながら更新していく、現在のプレイヤー境界ボックス
    BoundingBox currentBox = playerBox;

    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {
            // 壁チップ以外は無視する
            if (m_mapArray[y][x] != Type::Wall) { continue; }

            const BoundingBox& wallBox = m_boundingBoxArray[y][x];

            // 衝突していなければスキップ
            if (!CheckHitAABB(currentBox, wallBox)) { continue; }

            // 当たった方向を調べる
            const HitDirection direction = CheckHitDirection(wallBox, currentBox);

            if ((direction & HitDirection::Top) != HitDirection::None)
            {
                const float pushY = wallBox.minPosition.y - currentBox.maxPosition.y;
                offset.y += pushY;
                currentBox.minPosition.y += pushY;
                currentBox.maxPosition.y += pushY;
            }
            if ((direction & HitDirection::Bottom) != HitDirection::None)
            {
                const float pushY = wallBox.maxPosition.y - currentBox.minPosition.y;
                offset.y += pushY;
                currentBox.minPosition.y += pushY;
                currentBox.maxPosition.y += pushY;
            }
            if ((direction & HitDirection::Left) != HitDirection::None)
            {
                const float pushX = wallBox.minPosition.x - currentBox.maxPosition.x;
                offset.x += pushX;
                currentBox.minPosition.x += pushX;
                currentBox.maxPosition.x += pushX;
            }
            if ((direction & HitDirection::Right) != HitDirection::None)
            {
                const float pushX = wallBox.maxPosition.x - currentBox.minPosition.x;
                offset.x += pushX;
                currentBox.minPosition.x += pushX;
                currentBox.maxPosition.x += pushX;
            }
        }
    }

    return offset;
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
                //DrawBox(
                //    x * CHIP_SIZE,
                //    y * CHIP_SIZE,
                //    x * CHIP_SIZE + CHIP_SIZE,
                //    y * CHIP_SIZE + CHIP_SIZE,
                //    GetColor(255, 255, 255), TRUE); // ★白の塗りつぶしで描画

                DrawGraph(
                    x * CHIP_SIZE,
                    y * CHIP_SIZE,
                    m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Grass),
                    TRUE);
            }
        }
    }
}



// ------------------------------------------------------------------
// ItemFood_1
// ------------------------------------------------------------------
void Stage::ItemFood_1Render() const
{
  for (const auto& item : m_itemFood_1)   // ★ Item_Food_1 → item に変更
    {
      // ★保有中でなく、かつ取得済みでもない場合のみ、マップ上に描画する
         if (item.GetActiveFlag() && !item.GetIsHeld())
        {
            item.Render();
        }
    }}