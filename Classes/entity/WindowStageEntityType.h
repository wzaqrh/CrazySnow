//
//  WindowStageEntityType.h
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//
#pragma once
#include "common/CommonDef.h"

typedef enum tagWindowStageID
{
    CS_STAGE_BEGIN,
    CS_STAGE_GAME,
    CS_STAGE_SHOP,
    CS_STAGE_HELP,
    CS_POPUP_WIN,
    CS_POPUP_LOSE,
    CS_STAGE_MAX
}WindowStageID;
typedef std::function<cocos2d::Node*(WindowStageID)> WindowStageEntry;
typedef std::function<void(WindowStageID, cocos2d::Node*)> WindowStageCreateCallback;