//
//  WinBoard.cpp
//  crazy_snow
//
//  Created by qrh on 15-2-14.
//
//

#include "WinBoard.h"
#include "ResourceDef.h"
#include "ResourceUtility.h"
#include "DialogComponent.h"

using namespace cocos2d;

enum enTag {
    TAG_BTN_LEFT = 189,
    TAG_BTN_RIGHT,
    TAG_BTN_CLOSE
};

WinBoard::WinBoard()
:m_callback(nullptr)
,m_background(nullptr)
,m_title(nullptr)
,m_pMenu(nullptr)
,m_btnLeft(nullptr)
,m_btnRight(nullptr)
,m_btnClose(nullptr)
{
}
bool WinBoard::init()
{
	Layer::init();
    initUI();
    layout();
    return true;
}
void WinBoard::setBoardEventCallback(std::function<void(int)> delegate) {
	m_callback = delegate;
}
void WinBoard::initUI()
{
    m_background = Sprite::create(DIR_DIALOG"win_bg.png");
    this->addChild(m_background);
    
    m_title = Sprite::create(DIR_DIALOG"win.png");
    this->addChild(m_title);
    
    m_pMenu = Menu::create();
	m_pMenu->setAnchorPoint(Point::ZERO);
	m_pMenu->setPosition(Point::ZERO);
	this->addChild(m_pMenu);
    
    m_btnLeft = ResourceUtility::createMenuItem(DIR_DIALOG"back_btn.png", DIR_DIALOG"back_btn.png", CC_CALLBACK_1(WinBoard::onMenuCallback, this));
    m_btnLeft->setTag(TAG_BTN_LEFT);
    m_pMenu->addChild(m_btnLeft);
    
    m_btnRight = ResourceUtility::createMenuItem(DIR_DIALOG"next_b.png", DIR_DIALOG"next_b.png", CC_CALLBACK_1(WinBoard::onMenuCallback, this));
    m_btnRight->setTag(TAG_BTN_RIGHT);
    m_pMenu->addChild(m_btnRight);
    
    m_btnClose = ResourceUtility::createMenuItem(DIR_DIALOG"close_btn.png", DIR_DIALOG"close_btn.png", CC_CALLBACK_1(WinBoard::onMenuCallback, this));
    m_btnClose->setTag(TAG_BTN_CLOSE);
    m_pMenu->addChild(m_btnClose);
}
void WinBoard::layout()
{
    Size contnetSize(m_background->getContentSize());
    this->setContentSize(contnetSize);
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    m_background->setPosition(Vec2(contnetSize.width / 2, contnetSize.height / 2));
    m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    m_title->setPosition(Vec2(contnetSize.width / 2, contnetSize.height + 63));
    m_btnLeft->setPosition(Vec2(204, 140));
    m_btnRight->setPosition(Vec2(431, 140));
    m_btnClose->setPosition(Vec2(510, 361));
}
void WinBoard::onMenuCallback(cocos2d::Ref* sender)
{
    MenuItem* menuItem = static_cast<MenuItem*>(sender);
    switch (menuItem->getTag()) {
        case TAG_BTN_LEFT:
            if (m_callback) {
                m_callback(CS_DIALOG_BTN_BACK);
            }
            break;
        case TAG_BTN_RIGHT:
            if (m_callback) {
                m_callback(CS_DIALOG_BTN_NEXT);
            }
            break;
        case TAG_BTN_CLOSE:
            if (m_callback) {
                m_callback(CS_DIALOG_BTN_CLOSE);
            }
        default:
            break;
    }
}