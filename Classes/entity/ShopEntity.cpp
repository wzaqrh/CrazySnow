//
//  ShopEntity.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//

#include "ShopEntity.h"
#include "scene/shop/ShopLayer.h"
#include "entity/WindowStageEntity.h"
using namespace cocos2d;

ShopEntity::ShopEntity()
{}

void ShopEntity::loadShop()
{
    WindowStageEntity::Inst()->loadDialog(CS_STAGE_SHOP);
}

void ShopEntity::onShopLoaded(WindowStageID stageID, cocos2d::Node* window)
{
    Size sz = Director::getInstance()->getWinSize();
	ShopLayer* m_pLayerShop = static_cast<ShopLayer*>(window);
	m_pLayerShop->setScale(1.33f);
    m_pLayerShop->setShopEventCallback(CC_CALLBACK_1(ShopEntity::onShopEvent, this));
	m_pLayerShop->setPosition(Point(sz.width/2, sz.height/2));
}

void ShopEntity::onShopEvent(int shopEventType)
{
    switch (shopEventType) {
        case CS_SHOP_EVENT_CLOSE:
            
            break;
        default:
            break;
    }
}
