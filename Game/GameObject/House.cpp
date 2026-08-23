/*
    @file   House.h
    @brief  ハウスクラス（このオブジェクトへ食べ物を届ける）
    @author 鈴木蒼良
    @date   2026年8月22日
*/

#include "pch.h"
#include "House.h"
#include "Game/GameContext.h"


House::House(GameContext& gameContext, const BoundingBox& boundingBox, Item_Food_1::FoodType wantedFoodType)
    :m_gameContext{ gameContext }
    , m_boundingBox{ boundingBox }
    , m_wantedFoodType{ wantedFoodType }
    , m_isFulfilled{ false }
{
}

House::~House()
{
}

void House::Render() const
{

    // 家の画像
    DrawGraph(
        static_cast<int>(m_boundingBox.minPosition.x),
        static_cast<int>(m_boundingBox.minPosition.y),
        m_gameContext.ghManager.GetGraphicHandle(GhManager::Textures::House),
        TRUE);

    // 届け終わっていない時だけ、欲しがっている食べ物のアイコンを表示する
    if (!m_isFulfilled) { RenderWantedItemIcon(); }

}



// 欲しがっている食べ物を、家の横に表示
void House::RenderWantedItemIcon() const
{
    // if文の手前で変数を宣言する
    GhManager::Textures texture;
    // if文で画像の種類を分岐して、代入する
    if (m_wantedFoodType == Item_Food_1::FoodType::Food1)
    {
        texture = GhManager::Textures::Item_Food_1;
    }
    else if (m_wantedFoodType == Item_Food_1::FoodType::Food2)
    {
        texture = GhManager::Textures::Item_Food_2;
    }

    // 表示位置を計算してアイコンを描画する
    const int iconX = static_cast<int>(m_boundingBox.maxPosition.x) + 4; // 家の右横に少し余白
    const int iconY = static_cast<int>(m_boundingBox.minPosition.y);
    const int iconSize = 24;

   
    // 看板風の背景（半透明の黒い四角）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(iconX - 4, iconY - 4, iconX + iconSize + 4, iconY + iconSize + 4, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

    // 看板の枠線
    DrawBox(iconX - 4, iconY - 4, iconX + iconSize + 4, iconY + iconSize + 4, GetColor(255, 255, 255), FALSE);

    // アイコン本体（サイズを縮小して描画）
    DrawExtendGraph(iconX, iconY, iconX + iconSize, iconY + iconSize,
        m_gameContext.ghManager.GetGraphicHandle(texture), TRUE);
}
