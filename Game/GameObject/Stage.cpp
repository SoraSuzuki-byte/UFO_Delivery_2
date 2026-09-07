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
    , m_player{ nullptr }
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
   
    CreateBoundingBoxArray();
    for (int i = 0; i < MAX_EXPLOSION; i++) { m_explosions[i].Initialize(); }
}

// ------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------
void Stage::Update()
{
   // 各オブジェクトの更新
    for (auto& item : m_itemFood) // 食べ物
    {
        item.Update();
    }    
    for (auto& enemy : m_enemies_1) // Enemy
    {
        enemy.Update();
    }   
    for (auto& enemy : m_enemies_2) // Enemy2
    {
        enemy.Update();
    }   
    for (auto& enemy : m_enemies_3) // Enemy3
    {
        enemy.Update();
    }   
    for (auto& bomb : m_itemBomb) // 爆弾
    {
        bomb.Update();
    }
    for (int i = 0; i < MAX_EXPLOSION; i++) // 爆発エフェクト
    {
        m_explosions[i].Update();
    }


    // 当たり判定
    CheckFoodHouseCollision(); // 食べ物と,家
    CheckBombEnemyCollision();  // 爆弾と, 敵
    CheckBombEnemy2Collision();  // 爆弾と, 敵2

}

// ------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------
void Stage::Render() const
{
    ItemFoodRender();
    StageRender();
    HouseRender();
    EnemyRender();
    Enemy2Render();
    Enemy3Render();
    ItemBombRender();

    // -- 爆発エフェクトの描画 -- //
    // 1. 画像ハンドルを取得する
    const int explosionHandle = m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Explosion);

    // 2. アクティブな爆発だけ描画関数を呼ぶ（ハンドルを渡す）
    for (int i = 0; i < MAX_EXPLOSION; i++) {
        if (m_explosions[i].IsActive()) 
        { 
            m_explosions[i].Render(explosionHandle); 
        } 
    }
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
// 食べ物と、家との当たり判定
// ------------------------------------------------------------------
void Stage::CheckFoodHouseCollision()
{
    for (auto& house : m_houses)
    {
        if (house.GetIsFulfilled()) continue;

        for (auto& item : m_itemFood)
        {
            if (!item.GetActiveFlag()) continue;

            // 当たり判定をチェック
            if (CheckHitAABB(item.GetBoundingBox(), house.GetBoundingBox())) {

                // 食べ物の種類と 家が求める種類を照合
                if (item.GetFoodType() == house.GetWantedFoodType())
                {
                    // 納品成功
                    item.SetActiveFlag(false);
                    house.SetIsFulfilled(true);
                    m_gameContext.soundManager.StartSe(SoundManager::Se::Se_Delivery);
                }
                break;
            }
        }
    }

}


// ------------------------------------------------------------------
// 爆弾と、敵1の当たり判定
// ------------------------------------------------------------------
void Stage::CheckBombEnemyCollision()
{
    for (auto& bomb : m_itemBomb)
    {
        if (!bomb.GetActiveFlag()) continue;

        for (auto& enemy : m_enemies_1)
        {
            if (!enemy.GetActiveFlag()) continue;

            // 当たり判定チェック
            if (CheckHitAABB(bomb.GetBoundingBox(), enemy.GetBoundingBox()))
            {
                // 空いている爆発枠を探して再生を開始する
                for (int i = 0; i < MAX_EXPLOSION; i++)
                {
                    // 使われていない（アニメーションが終わっている）爆発枠を見つける
                    if (!m_explosions[i].IsActive())
                    {
                        m_explosions[i].SetEnemyPosition(enemy.GetCenterPosition());
                        m_explosions[i].StartExplosion();
                        break; // 1つ設定したらループを抜ける
                    }
                }

                    bomb.SetActiveFlag(false);   // 爆弾消去
                    enemy.SetActiveFlag(false);  // 敵撃破
                    m_gameContext.soundManager.StartSe(SoundManager::Se::Se_Explosion);
                    break; // 敵探索のループを抜ける
                }
            }
        }
    }






// ------------------------------------------------------------------
// 爆弾と、敵2の当たり判定
// ------------------------------------------------------------------
void Stage::CheckBombEnemy2Collision()
{
    for (auto& bomb : m_itemBomb)
    {
        if (!bomb.GetActiveFlag()) continue;

        for (auto& enemy : m_enemies_2)
        {
            if (!enemy.GetActiveFlag()) continue;

            // 当たり判定チェック
            if (CheckHitAABB(bomb.GetBoundingBox(), enemy.GetBoundingBox()))
            {
                // 空いている爆発枠を探して再生を開始する
                for (int i = 0; i < MAX_EXPLOSION; i++)
                {
                    // 使われていない（アニメーションが終わっている）爆発枠を見つける
                    if (!m_explosions[i].IsActive())
                    {
                        m_explosions[i].SetEnemyPosition(enemy.GetCenterPosition());
                        m_explosions[i].StartExplosion();
                        break; // 1つ設定したらループを抜ける
                    }
                }

                    bomb.SetActiveFlag(false);   // 爆弾消去
                    enemy.SetActiveFlag(false);  // 敵撃破
                    m_gameContext.soundManager.StartSe(SoundManager::Se::Se_Explosion);
                    break; // 敵探索のループを抜ける
                }
            }
        }
    }













// 内部処理-----------------------------------------------------------------------------------------------


// ------------------------------------------------------------------
// ステージデータをロードする
// ------------------------------------------------------------------
void Stage::LoadStageData(const wchar_t* stageName)
{
    // CSVを読み込む前に、以前のステージの「House」の情報を空にする
    m_houses.clear();
    m_enemies_1.clear();
    m_enemies_2.clear();
    m_enemies_3.clear();
    m_itemBomb.clear();

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
    m_itemFood.clear();
    
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
                // Player
                case 2: 
                {
                    m_mapArray[y][x] = Type::Player;
                    m_playerStartPosition = Vector2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    break;
                }
                // 食べ物_1
                case 3:  
                {
                    m_mapArray[y][x] = Type::ItemFood;

                    const Vector2D minVec2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    const Vector2D maxVec2D{ minVec2D.x + CHIP_SIZE, minVec2D.y + CHIP_SIZE };
                    BoundingBox bb{ minVec2D, maxVec2D };

                    m_itemFood.emplace_back(Item_Food(m_gameContext, *this, m_player, bb, Item_Food::FoodType::Food1)); // bbは BoundingBox
                    m_itemFood.back().Initialize();
                    break;
                }
                // 食べ物_2
                case 4:   
                {
                    m_mapArray[y][x] = Type::ItemFood;   // Type自体は共通のままでOK（表示上の種類は別管理のため）

                    const Vector2D minVec2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    const Vector2D maxVec2D{ minVec2D.x + CHIP_SIZE, minVec2D.y + CHIP_SIZE };
                    BoundingBox bb{ minVec2D, maxVec2D };

                    m_itemFood.emplace_back(m_gameContext, *this, m_player, bb, Item_Food::FoodType::Food2);
                    m_itemFood.back().Initialize();
                    break;
                }
                // 食べ物_3
                case 5:
                {
                    m_mapArray[y][x] = Type::ItemFood;   // Type自体は共通のままでOK（表示上の種類は別管理のため）

                    const Vector2D minVec2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    const Vector2D maxVec2D{ minVec2D.x + CHIP_SIZE, minVec2D.y + CHIP_SIZE };
                    BoundingBox bb{ minVec2D, maxVec2D };

                    m_itemFood.emplace_back(m_gameContext, *this, m_player, bb, Item_Food::FoodType::Food3);
                    m_itemFood.back().Initialize();
                    break;
                }
                // 食べ物_4
                case 6:
                {
                    m_mapArray[y][x] = Type::ItemFood;   // Type自体は共通のままでOK（表示上の種類は別管理のため）

                    const Vector2D minVec2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    const Vector2D maxVec2D{ minVec2D.x + CHIP_SIZE, minVec2D.y + CHIP_SIZE };
                    BoundingBox bb{ minVec2D, maxVec2D };

                    m_itemFood.emplace_back(m_gameContext, *this, m_player, bb, Item_Food::FoodType::Food4);
                    m_itemFood.back().Initialize();
                    break;
                }
                // 食べ物_5
                case 7:
                {
                    m_mapArray[y][x] = Type::ItemFood;   // Type自体は共通のままでOK（表示上の種類は別管理のため）

                    const Vector2D minVec2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    const Vector2D maxVec2D{ minVec2D.x + CHIP_SIZE, minVec2D.y + CHIP_SIZE };
                    BoundingBox bb{ minVec2D, maxVec2D };

                    m_itemFood.emplace_back(m_gameContext, *this, m_player, bb, Item_Food::FoodType::Food5);
                    m_itemFood.back().Initialize();
                    break;
                }
                // Bomb
                case 8:
                {
                    m_mapArray[y][x] = Type::ItemBomb;   // Type自体は共通のままでOK（表示上の種類は別管理のため）

                    const Vector2D minVec2D{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };
                    const Vector2D maxVec2D{ minVec2D.x + CHIP_SIZE, minVec2D.y + CHIP_SIZE };
                    BoundingBox bb{ minVec2D, maxVec2D };

                    m_itemBomb.emplace_back(m_gameContext, *this, m_player, bb);
                    m_itemBomb.back().Initialize();
                    break;
                }
                // Enemy_1
                case 9:
                {
                    m_mapArray[y][x] = Type::Floor; // 出現する位置は、移動できる「床」であるため

                    const Vector2D startPos{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };

                    m_enemies_1.emplace_back(m_gameContext, *this, *m_player, startPos); // 敵を生成
                    break;
                }
                // Enemy_2
                case 10:
                {
                    m_mapArray[y][x] = Type::Floor; // 出現する位置は、移動できる「床」であるため

                    const Vector2D startPos{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };

                    m_enemies_2.emplace_back(m_gameContext, *this, *m_player, startPos); // 敵を生成
                    break;
                }
                // Enemy_3
                case 11:
                {
                    m_mapArray[y][x] = Type::Floor; // 出現する位置は、移動できる「床」であるため

                    const Vector2D startPos{ static_cast<float>(x) * CHIP_SIZE, static_cast<float>(y) * CHIP_SIZE };

                    m_enemies_3.emplace_back(m_gameContext, *this, *m_player, startPos); // 敵を生成
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
// ステージを描画
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
                DrawGraph(
                    x * CHIP_SIZE,
                    y * CHIP_SIZE,
                    m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::Wall),
                    TRUE);
            }
        }
    }
}



// ------------------------------------------------------------------
// ItemFoodを描画
// ------------------------------------------------------------------
void Stage::ItemFoodRender() const
{
  for (const auto& item : m_itemFood)   // ★ Item_Food → item に変更
    {
      // 取得済みでもない場合のみ、マップ上に描画する
         if (item.GetActiveFlag())
        {
            item.Render();
        }
    }}



// ------------------------------------------------------------------
// Houseを描画
// ------------------------------------------------------------------
void Stage::HouseRender() const
{
    for (const auto& house : m_houses)
    {
            house.Render();
    }
}



// ------------------------------------------------------------------
// Enemyを描画
// ------------------------------------------------------------------
void Stage::EnemyRender() const
{
    for (const auto& enemy : m_enemies_1)
    {
        if (enemy.GetActiveFlag())
        {
            enemy.Render();
        }
    }
}

// ------------------------------------------------------------------
// Enemy2を描画
// ------------------------------------------------------------------
void Stage::Enemy2Render() const
{
    for (const auto& enemy : m_enemies_2)
    {
        if (enemy.GetActiveFlag())
        {
            enemy.Render();
        }
    }
}

// ------------------------------------------------------------------
// Enemy3を描画
// ------------------------------------------------------------------
void Stage::Enemy3Render() const
{
    for (const auto& enemy : m_enemies_3)
    {
        if (enemy.GetActiveFlag())
        {
            enemy.Render();
        }
    }
}



// ------------------------------------------------------------------
// ItemBombを描画
// ------------------------------------------------------------------
void Stage::ItemBombRender() const
{
    for (const auto& bomb : m_itemBomb)   // ★ Item_Bomb → bomb に変更
    {
        // 使用済みでもない場合のみ、マップ上に描画する
        if (bomb.GetActiveFlag())
        {
            bomb.Render();
        }
    }
}
