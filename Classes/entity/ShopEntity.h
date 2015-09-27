//
//  ShopEntity.h
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//
#pragma once
#include "common/CommonDef.h"
#include "WindowStageEntityType.h"


class ShopEntity
:public single_base<ShopEntity>
{
public:
    ShopEntity();
    void loadShop();
private:
    void onShopLoaded(WindowStageID stageID, cocos2d::Node* window);
    void onShopEvent(int shopEventType);
};
