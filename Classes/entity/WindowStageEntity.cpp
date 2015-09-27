//
//  WindowStageEntity.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//

#include "WindowStageEntity.h"
#include "common/TouchSwallowLayer.h"
#include "common/DialogComponent.h"
#include "scene/BeginLayer.h"
#include "scene/shop/ShopLayer.h"
#include "scene/game/GameMainNode.h"
#include "scene/game/WinBoard.h"
#include "scene/game/LoseBoard.h"
#include "scene/help/HelpLayer.h"

using namespace cocos2d;

WindowStageEntity::WindowStageEntity()
{
    memset(m_entry, 0, sizeof(m_entry));
    initlizeAll();
}

cocos2d::Node*  WindowStageEntity::createNode(WindowStageID stageID, const WindowStageCreateCallback& callback) {
    if (m_entry[stageID]) {
        Node* node = m_entry[stageID](stageID);
        if (callback) {
            callback(stageID, node);
        }
        return node;
    }
    return nullptr;
}

cocos2d::Scene* WindowStageEntity::createScene(WindowStageID stageID, const WindowStageCreateCallback& callback)
{
    Scene* scene = nullptr;
    Node* node = createNode(stageID, callback);
    if (node) {
        scene = Scene::create();
        WindowAdaptUtility::addRootNodeToScene(scene, node);
    }
    return scene;
}

bool WindowStageEntity::fisrtStage(WindowStageID stageID, const WindowStageCreateCallback& callback)
{
    Scene* scene = createScene(stageID, callback);
    if (scene) {
        Director::getInstance()->runWithScene(scene);
        return true;
    }
    return false;
}

bool WindowStageEntity::loadStage(WindowStageID stageID, const WindowStageCreateCallback& callback)
{
    Scene* scene = createScene(stageID, callback);
    if (scene) {
        Director::getInstance()->replaceScene(scene);
        return true;
    }
    return false;
}

bool WindowStageEntity::nextStage(WindowStageID stageID, const WindowStageCreateCallback& callback)
{
    Scene* scene = createScene(stageID, callback);
    if (scene) {
        Director::getInstance()->pushScene(scene);
        return true;
    }
    return false;
}

bool WindowStageEntity::backToPrevStage()
{
    Director::getInstance()->popScene();
    return true;
}

bool WindowStageEntity::loadDialog(WindowStageID stageID, const WindowStageCreateCallback& callback)
{
    auto dialog = createNode(stageID, callback);
    if (dialog) {
        auto swallow = TouchSwallowLayer::create();
        dialog->addChild(swallow, SCENE_GLOBAL_ZORDER_LOWEST);
        
        WindowAdaptUtility::addRootNodeToScene(Director::getInstance()->getRunningScene(), dialog, SCENE_GLOBAL_ZORDER_DIALOG);
        return true;
    }
    return false;
}


template<class _Class>
static cocos2d::Node* windowStageEntryAdapter(WindowStageID stage) {
    return _Class::create();
}

template<class _Class, typename _Tp>
static cocos2d::Node* windowStageEntryAdapterOne(WindowStageID stage, _Tp param_one) {
    return _Class::create(param_one);
}

void WindowStageEntity::initlizeAll()
{
    registerStageEntry(CS_STAGE_BEGIN, windowStageEntryAdapter<BeginLayer>);
    registerStageEntry(CS_STAGE_GAME,  windowStageEntryAdapter<GameMainNode>);
    registerStageEntry(CS_STAGE_SHOP,  windowStageEntryAdapter<ShopLayer>);
    registerStageEntry(CS_STAGE_HELP,  windowStageEntryAdapter<HelpLayer>);
    
    registerStageEntry(CS_POPUP_WIN,  windowStageEntryAdapter<WinBoard>);
    registerStageEntry(CS_POPUP_LOSE,  windowStageEntryAdapter<LoseBoard>);
    //registerStageEntry(CS_POPUP_WIN,  std::bind(windowStageEntryAdapterOne<PopupDialog, DialogType>, std::placeholders::_1, CS_POPUP_DIALOG_WIN));
    //registerStageEntry(CS_POPUP_LOSE,  std::bind(windowStageEntryAdapterOne<PopupDialog, DialogType>, std::placeholders::_1, CS_POPUP_DIALOG_LOSE));
}

void WindowStageEntity::registerStageEntry(WindowStageID stage, const WindowStageEntry& entry)
{
    assert(m_entry[stage] == nullptr);
    m_entry[stage] = entry;
}

