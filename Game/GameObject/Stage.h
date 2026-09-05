/*
    @file   Stage.h
    @brief  ステージクラス
    @author 制作者
    @date   2026/07/03
*/




#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"
#include "Game/GameObject/Item_Food.h"  
#include "Game/GameObject/House.h"
#include "Game/GameObject/Enemy.h"
#include "Game/GameObject/Enemy2.h"
#include "Game/GameObject/Enemy3.h"
#include "Game/GameObject/Item_Bomb.h"
#include <vector>        // Item_Foodの数を、柔軟に変えられるように    
#include "Game/Class/Effect/Explosion.h"

// 前方宣言 ===============================================================
struct GameContext;
class Player;



class Stage
{
public:
    // マップチップの種類m_boundingBoxArray
    enum class Type
    {
        None = -1,
        Floor, Wall, Player, ItemFood, ItemBomb
    };

private:
    // マップチップの大きさ
    static constexpr int CHIP_SIZE = 20;

    // CSVのパスを作るための文字列データ
    const std::wstring PATH = L"Resources/MapData/";
    const std::wstring CSV = L".csv";

private:
    // 爆発エフェクトの同時に出せる最大数
    static constexpr int MAX_EXPLOSION = 3;
    // 爆発エフェクトの配列
    Explosion m_explosions[MAX_EXPLOSION]; 


    // ゲームコンテキストのインスタンス
    GameContext& m_gameContext;

    Player* m_player;   //（ポインタで持つ。まだ存在しない可能性があるため）


    // マップの幅と高さ
    int m_mapWidth;
    int m_mapHeight;

    // マップの２次元配列を動的に確保するためのダブルポインタ型変数// 画面上の見た目 用の
    Type** m_mapArray;

    // 境界ボックス配列（ワールド座標で管理）//当たり判定 用の
    BoundingBox** m_boundingBoxArray;

    // プレイヤー初期位置（ワールド座標で管理）
    Vector2D m_playerStartPosition;

    // Item_Foodの配列
    std::vector<Item_Food> m_itemFood;   
    // Houseの配列
    std::vector<House> m_houses;
    // Enemyの配列
    std::vector<Enemy> m_enemies_1;
    // Enemy2の配列
    std::vector<Enemy2> m_enemies_2;
    // Enemy3の配列
    std::vector<Enemy3> m_enemies_3;
    // Item_Bombの配列
    std::vector<Item_Bomb> m_itemBomb;


public:
    Stage(GameContext& gameContext);
    ~Stage();

    void Initialize(const wchar_t* stageNumber);
    void Update(); 
    void Render() const;
    void Finalize();

    // ワールド座標からマップ座標のマップチップの型を返す
    Stage::Type GetChipType(const Vector2D& worldPosition) const;

    // マップ座標からワールド座標の境界ボックスを計算する
    const BoundingBox& GetBoundingBox(const POINT& mapPosition) const;

    // ワールド座標をマップ座標に変換する
    POINT ConvertWorldPositionToMapPosition(const Vector2D& worldPosition);

    // Playerの参照を後から設定する
    void SetPlayer(Player& player) { m_player = &player; }

    // プレイヤーの初期位置のゲッター 
    const Vector2D& GetPlayerStartPosition() const { return m_playerStartPosition; }


    // 食べ物配列を取得する（外部から位置変更などを行うため）
    std::vector<Item_Food>& GetItemFood() { return m_itemFood; }

    // m_itemFoodの配列を取得するゲッター
    std::vector<Item_Food>& GetItems() { return m_itemFood; }

    // 家の配列を取得する（アイテムとの当たり判定用）
    std::vector<House>& GetHouses() { return m_houses; }

    // Enemy配列を取得する
    std::vector<Enemy>& GetEnemies() { return m_enemies_1; }

    // 家を配置する（ステージごとにコードで指定するため）
    void AddHouse(const Vector2D& position, float width, float height, Item_Food::FoodType wantedFoodType)
    {
        BoundingBox bb{ position, Vector2D{ position.x + width, position.y + height } };
        m_houses.emplace_back(m_gameContext, bb, wantedFoodType);
    }

    // プレイヤーの境界ボックスと壁チップとの当たり判定を行い、押し戻し量を計算する
    Vector2D ResolveWallCollision(const BoundingBox& playerBox) const;

    // すべての家に届け終わったかどうかを判定する
    bool IsAllHousesFulfilled() const
    {
        for (const auto& house : m_houses)
        {
            if (!house.GetIsFulfilled())
            {
                return false;   // 1つでも未達成の家があれば false
            }
        }
        return true;   // 全部届いていれば true
    }

    // 食べ物と、家との当たり判定
    void CheckFoodHouseCollision();
    // 爆弾と、敵1の当たり判定
    void CheckBombEnemyCollision(); 
    // 爆弾と、敵2の当たり判定
    void CheckBombEnemy2Collision(); 


    int GetMapWidth()  const { return m_mapWidth; }
    int GetMapHeight() const { return m_mapHeight; }
    int GetChipSize()  const { return CHIP_SIZE; }





    // 内部処理-----------------------------------------------------------------------------------------------
private:
    // ステージデータをロードする
    void LoadStageData(const wchar_t* stageName);

    // マップの境界ボックスを作成する
    void CreateBoundingBoxArray();

    // 描画のサブ関数
    void BackgroundRender() const;
    void StageRender() const;
    void ItemFoodRender() const;
    void HouseRender() const;
    void EnemyRender() const;
    void Enemy2Render() const;
    void Enemy3Render() const;
    void ItemBombRender() const;
};