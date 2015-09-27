//
//  HelpLayer.cpp
//  crazy_snow
//
//  Created by qrh on 15-2-14.
//
//

#include "HelpLayer.h"
#include "ResourceDef.h"
#include "ResourceUtility.h"
#include "common/LabelNode.h"

using namespace cocos2d;

enum enZorder {
    ZORDER_BG = -1,
    ZORDER_TITLE = 1,
    ZORDER_MENU = 2
};

enum enTag {
    TAG_BTN_CLOSE = 187
};

HelpLayer::HelpLayer()
:m_background(nullptr)
,m_title(nullptr)
,m_pMenu(nullptr)
,m_close(nullptr)
{
}

bool HelpLayer::init() {
    if (! Node::init()) {
        return false;
    }
    initUI();
    return true;
}

cocos2d::Node* HelpLayer::createBackground()
{
    Node* background = Sprite::create(DIR_HELP"help_bg.png");
    const Size& backdroundSize = background->getContentSize();

    {
        cocos2d::Label* label = ResourceUtility::createLabel("玩法: 点击颜色连接在一起的雪花消除分数达到目标即可通关", 24, Size(450, 96), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        label->setColor(Color3B(0, 255, 255));
        label->enableOutline(Color4B(0, 107, 255, 255), 3);
        
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->ignoreAnchorPointForPosition(false);
        
        label->setPosition(Vec2(backdroundSize.width / 2, 680 + 48));
        background->addChild(label);
    }
    {
        cocos2d::Label* label = ResourceUtility::createLabel("消灭的雪花越多得到的分数越高", 24, Size(450, 96), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        label->setColor(Color3B(0, 255, 255));
        label->enableOutline(Color4B(0, 107, 255, 255), 3);
        
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->ignoreAnchorPointForPosition(false);
        
        label->setPosition(Vec2(backdroundSize.width / 2, 513));
        background->addChild(label);
    }
    {
        cocos2d::Label* label = ResourceUtility::createLabel("道具:", 24, Size(80, 64), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        label->setColor(Color3B(0, 255, 255));
        label->enableOutline(Color4B(0, 107, 255, 255), 3);
        
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        label->ignoreAnchorPointForPosition(false);
        
        label->setPosition(Vec2(141, 303));
        background->addChild(label);
    }
    
    const char* s_propDesc[] = {
        "跳过: 跳过步骤(每次消耗%d个钻石)",
        "刷子: 可以随意改变雪花的颜色(每次消耗%d个钻石)",
        "刷新: 使用后所有雪花颜色随机改变(每次消耗%d个钻石)"
    };
    Vec2 propDescPos[] = {
        Vec2(180, 257),
        Vec2(180, 198),
        Vec2(180, 138)
    };
    int propPrices[] = {
        5, 5, 5
    };
    for (int i = 0; i < 3; ++i) {
        char buf[200]; sprintf(buf, s_propDesc[i], propPrices[i]);
        cocos2d::Label* label = ResourceUtility::createLabel(buf, 20, Size(400, 32), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        label->setColor(Color3B(0, 208, 255));
        //label->enableOutline(Color4B(0, 107, 255, 255), 3);
        
        label->setAnchorPoint(Vec2(0.0f, 0.5f));
        label->ignoreAnchorPointForPosition(false);
        
        label->setPosition(propDescPos[i]);
        background->addChild(label);
    }
    
    return background;
}

void HelpLayer::initUI()
{
    m_background = createBackground();
    this->addChild(m_background, ZORDER_BG);
    
    m_title = Sprite::create(DIR_HELP"help_head.png");
    this->addChild(m_title, ZORDER_TITLE);
    
    m_pMenu = Menu::create(NULL);
    m_pMenu->setAnchorPoint(Point::ZERO);
    m_pMenu->setPosition(Point::ZERO);
    this->addChild(m_pMenu, ZORDER_MENU);
    
    {
        auto _menuItem = ResourceUtility::createMenuItem(DIR_HELP"close.png", DIR_HELP"close.png", CC_CALLBACK_1(HelpLayer::onMenuCallback, this));
        _menuItem->setPosition(Point(90, 84));
        _menuItem->setTag(TAG_BTN_CLOSE);
        m_pMenu->addChild(_menuItem);
        m_close = _menuItem;
    }
    
    layout();
}

void HelpLayer::layout()
{
    const Size& backgroundSize = m_background->getContentSize();
    m_background->setPosition(Vec2(backgroundSize.width / 2, backgroundSize.height / 2));
    
    m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    m_title->setPosition(Vec2(backgroundSize.width / 2, backgroundSize.height - 42 - 62));
    
    m_close->setPosition(Vec2(backgroundSize.width / 2 + 192, m_title->getPosition().y + 41));
    
    Size conetntSize(backgroundSize.width, backgroundSize.height + 80);
    this->setContentSize(conetntSize);
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
}

void HelpLayer::onMenuCallback(cocos2d::Ref* sender)
{
    MenuItem* menuItem = static_cast<MenuItem*>(sender);
    if (menuItem->getTag() == TAG_BTN_CLOSE) {
        this->removeFromParent();
    }
}






