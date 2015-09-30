//
//  WindowStageEntity.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//

#include "entity/WindowStageEntity.h"
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

static Node* createPaddMain() {
    return ResourceUtility::createSprite(nullptr, DIR_ZZIMAGE"padd_main.png");
}
static Node* createPaddFront() {
    return ResourceUtility::createSprite(nullptr, DIR_ZZIMAGE"padd_front.png");
}
static Node* createPaddNode(std::function<Node*()> imgCreateFunc, float dW, float dH) {
    //Node* padd = ResourceUtility::createSprite(nullptr, DIR_ZZIMAGE"padd.png");
    Node* padd = imgCreateFunc();
    const Size& paddSize = padd->getContentSize();
    padd->setScaleX(dW /CC_CONTENT_SCALE_FACTOR() / paddSize.width);
    padd->setScaleY(dH /CC_CONTENT_SCALE_FACTOR() / paddSize.height);
    return padd;
}
#define FIX_WIDTH
Node* addPaddToMainNode(Node* scene, std::function<Node*()> imgCreateFunc) {
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    
    Size viewSize = glView->getFrameSize();
    Size designSize = Director::getInstance()->getVisibleSize();
#ifdef FIX_WIDTH
    float paddHeight = (designSize.height - 960) / 2;
    Node* botPadd = createPaddNode(imgCreateFunc, designSize.width + 10, paddHeight + 10);
    botPadd->setPosition(designSize.width / 2, - paddHeight / 2);
    scene->addChild(botPadd);
    
    Node* topPadd = createPaddNode(imgCreateFunc, designSize.width + 10, paddHeight + 10);
    topPadd->setPosition(designSize.width / 2, scene->getContentSize().height + paddHeight / 2);
    scene->addChild(topPadd);
#elif defined FIX_HEIGHT
    float paddWidth = (designSize.width - 640) / 2;
    Node* leftPadd = createPaddNode(imgCreateFunc, paddWidth + 10, designSize.height + 1);
    leftPadd->setPosition(-paddWidth / 2, designSize.height / 2);
    scene->addChild(leftPadd);
    
    Node* rightPadd = createPaddNode(imgCreateFunc, paddWidth + 10, designSize.height + 1);
    rightPadd->setPosition(scene->getContentSize().width + paddWidth / 2, designSize.height / 2);
    scene->addChild(rightPadd);
#else
    float scaleX = viewSize.width / designSize.width;
    float scaleY = viewSize.height / designSize.height;
    float scale  = std::min(scaleX, scaleY);
    if (scaleX > scaleY) {
        viewSize = viewSize / scale;
        float paddWidth = (viewSize.width - designSize.width)/ 2 + 1;
        Node* leftPadd = createPaddNode(imgCreateFunc, paddWidth + 10, viewSize.height + 1);
        leftPadd->setPosition(paddWidth / 2, viewSize.height / 2);
        scene->addChild(leftPadd);
        
        Node* rightPadd = createPaddNode(imgCreateFunc, paddWidth + 10, viewSize.height + 1);
        rightPadd->setPosition(viewSize.width - paddWidth / 2, viewSize.height / 2);
        scene->addChild(rightPadd);
    }
    else if (scaleX < scaleY) {
        viewSize = viewSize / scale;
        float paddHeight = (viewSize.height - designSize.height) / 2 + 1;
        Node* upPadd = createPaddNode(imgCreateFunc, viewSize.width + 1, paddHeight + 10);
        upPadd->setPosition(viewSize.width / 2, viewSize.height - paddHeight / 2);
        scene->addChild(upPadd);
        
        Node* botPadd = createPaddNode(imgCreateFunc, viewSize.width + 1, paddHeight + 10);
        botPadd->setPosition(viewSize.width / 2, paddHeight / 2);
        scene->addChild(botPadd);
    }
#endif
    return scene;
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
        Director::getInstance()->pushScene(scene);
        return true;
    }
    return false;
}

bool WindowStageEntity::nextStage(WindowStageID stageID, const WindowStageCreateCallback& callback)
{
    Scene* scene = createScene(stageID, callback);
    if (scene) {
        Director::getInstance()->replaceScene(scene);
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
template<class _Class>
static cocos2d::Node* windowStageEntryAdapterPadd(WindowStageID stage, std::function<Node*()> createImg) {
    auto node = _Class::create();
    addPaddToMainNode(node, createImg);
    return node;
}
template<class _Class, typename _Tp>
static cocos2d::Node* windowStageEntryAdapterOne(WindowStageID stage, _Tp param_one) {
    return _Class::create(param_one);
}

void WindowStageEntity::initlizeAll()
{
    registerStageEntry(CS_STAGE_BEGIN, std::bind(windowStageEntryAdapterPadd<BeginLayer>, std::placeholders::_1, createPaddFront));
    registerStageEntry(CS_STAGE_GAME,  std::bind(windowStageEntryAdapterPadd<GameMainNode>, std::placeholders::_1, createPaddMain));
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

