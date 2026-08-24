/*
    @file   Stage.h
    @brief  ステージクラス
    @author 制作者
    @date   2026/07/03
*/




#pragma once
#include "Library/GameMath.h"
#include "Game/CollisionAABB.h"
#include "Game/GameObject/Item_Food_1.h"  
#include "Game/GameObject/House.h"
#include <vector>        // Item_Food_1の数を、柔軟に変えられるように                 
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
        Floor, Wall, Player, ItemFood_1
    };

private:
    // マップチップの大きさ
    static constexpr int CHIP_SIZE = 20;

    // CSVのパスを作るための文字列データ
    const std::wstring PATH = L"Resources/MapData/";
    const std::wstring CSV = L".csv";

private:
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

    // Item_Food_1の配列
    std::vector<Item_Food_1> m_itemFood_1;   
    // Houseの配列
    std::vector<House> m_houses;


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

    // アイテム配列を取得する（外部から位置変更などを行うため）
    std::vector<Item_Food_1>& GetItemFood_1() { return m_itemFood_1; }
    // m_itemFood_1の配列を取得するゲッター
    std::vector<Item_Food_1>& GetItems() { return m_itemFood_1; }

    // 家の配列を取得する（アイテムとの当たり判定用）
    std::vector<House>& GetHouses() { return m_houses; }
    // 家を配置する（ステージごとにコードで指定するため）
    void AddHouse(const Vector2D& position, float width, float height, Item_Food_1::FoodType wantedFoodType)
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
    void ItemFood_1Render() const;
    void HouseRender() const;
};