//
//  PauseBoard.cpp
//  crazy_snow
//
//  Created by qrh on 15-2-14.
//
//

#include "PauseBoard.h"
#include "ResourceDef.h"
#include "ResourceUtility.h"

using namespace cocos2d;

enum enTag {
    TAG_BTN_PAUSE = 189,
    TAG_BTN_HOMEPAGE,
    TAG_BTN_CONTINUE
};

void PauseBoard::addBoardClkListener(std::function<void(enBoardItem)> delegate) {
	m_pDelegate = delegate;
}
PauseBoard::PauseBoard()
:m_pDelegate(nullptr)
,m_background(nullptr)
,m_title(nullptr)
,m_pMenu(nullptr)
,m_resume(nullptr)
,m_homepage(nullptr)
,m_continue(nullptr)
{
}
bool PauseBoard::init()
{
	Layer::init();
    initUI();
    layout();
    return true;
}
void PauseBoard::initUI()
{
    m_background = Sprite::create(DIR_PAUSE"stop_bg.png");
    this->addChild(m_background);
    
    m_title = Sprite::create(DIR_PAUSE"stop_head.png");
    this->addChild(m_title);
    
    m_pMenu = Menu::create();
	m_pMenu->setAnchorPoint(Point::ZERO);
	m_pMenu->setPosition(Point::ZERO);
	this->addChild(m_pMenu);
    
    m_resume = ResourceUtility::createMenuItem(DIR_PAUSE"stop_btn.png", DIR_PAUSE"stop_btn.png", CC_CALLBACK_1(PauseBoard::onMenuCallback, this));
    m_resume->setTag(TAG_BTN_PAUSE);
    m_pMenu->addChild(m_resume);
    
    m_homepage = ResourceUtility::createMenuItem(DIR_PAUSE"back_btn.png", DIR_PAUSE"back_btn.png", CC_CALLBACK_1(PauseBoard::onMenuCallback, this));
    m_homepage->setTag(TAG_BTN_HOMEPAGE);
    m_pMenu->addChild(m_homepage);
    
    m_continue = ResourceUtility::createMenuItem(DIR_PAUSE"go_btn.png", DIR_PAUSE"go_btn.png", CC_CALLBACK_1(PauseBoard::onMenuCallback, this));
    m_continue->setTag(TAG_BTN_CONTINUE);
    m_pMenu->addChild(m_continue);
}
void PauseBoard::layout()
{
    Size contnetSize(m_background->getContentSize());
    this->setContentSize(contnetSize);
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    m_background->setPosition(Vec2(contnetSize.width / 2, contnetSize.height / 2));
    m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    m_title->setPosition(Vec2(contnetSize.width / 2, contnetSize.height + 5));
    m_resume->setPosition(Vec2(300, 212));
    m_homepage->setPosition(Vec2(175, 90));
    m_continue->setPosition(Vec2(414, 91));
}
void PauseBoard::onMenuCallback(cocos2d::Ref* sender)
{
    MenuItem* menuItem = static_cast<MenuItem*>(sender);
    switch (menuItem->getTag()) {
        case TAG_BTN_PAUSE:
            if (m_pDelegate) {
                m_pDelegate(enItemSure);
            }
            break;
        case TAG_BTN_HOMEPAGE:
            if (m_pDelegate) {
                m_pDelegate(enItemLeft);
            }
            break;
        case TAG_BTN_CONTINUE:
            if (m_pDelegate) {
                m_pDelegate(enItemRight);
            }
            break;
        default:
            break;
    }
}