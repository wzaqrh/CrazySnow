//
//  WindowStageEntity.h
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//
#pragma once
#include "entity/WindowStageEntityType.h"

class WindowStageEntity
: public single_base<WindowStageEntity>
{
public:
    WindowStageEntity();
public:
    bool fisrtStage(WindowStageID stageID, const WindowStageCreateCallback& callback = nullptr);
    bool loadStage(WindowStageID stageID, const WindowStageCreateCallback& callback = nullptr);
    bool nextStage(WindowStageID stageID, const WindowStageCreateCallback& callback = nullptr);
    bool backToPrevStage();
public:
    bool loadDialog(WindowStageID stageID, const WindowStageCreateCallback& callback = nullptr);
private:
    void initlizeAll();
    void registerStageEntry(WindowStageID stageID, const WindowStageEntry& entry);
    cocos2d::Scene* createScene(WindowStageID stageID, const WindowStageCreateCallback& callback);
    cocos2d::Node*  createNode(WindowStageID stageID, const WindowStageCreateCallback& callback);
private:
    WindowStageEntry m_entry[CS_STAGE_MAX];
};

