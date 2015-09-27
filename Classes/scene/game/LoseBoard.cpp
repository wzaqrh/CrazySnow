//
//  LoseBoard.cpp
//  crazy_snow
//
//  Created by qrh on 15-2-14.
//
//

#include "LoseBoard.h"
#include "ResourceDef.h"
#include "ResourceUtility.h"
#include "DialogComponent.h"

using namespace cocos2d;

enum enTag {
    TAG_BTN_LEFT = 189,
    TAG_BTN_RIGHT,
    TAG_BTN_CLOSE
};

LoseBoard::LoseBoard()
:m_callback(nullptr)
,m_background(nullptr)
,m_title(nullptr)
,m_pMenu(nullptr)
,m_btnLeft(nullptr)
//,m_btnRight(nullptr)
,m_btnClose(nullptr)
{
}
bool LoseBoard::init()
{
	Layer::init();
    initUI();
    layout();
    return true;
}
void LoseBoard::setBoardEventCallback(std::function<void(int)> delegate) {
	m_callback = delegate;
}
void LoseBoard::initUI()
{
    m_background = Sprite::create(DIR_DIALOG"lose_bg.png");
    this->addChild(m_background);
    
    m_title = Sprite::create(DIR_DIALOG"lose.png");
    this->addChild(m_title);
    
    m_pMenu = Menu::create();
	m_pMenu->setAnchorPoint(Point::ZERO);
	m_pMenu->setPosition(Point::ZERO);
	this->addChild(m_pMenu);
    
    m_btnLeft = ResourceUtility::createMenuItem(DIR_DIALOG"jxtg.png", DIR_DIALOG"jxtg.png", CC_CALLBACK_1(LoseBoard::onMenuCallback, this));
    m_btnLeft->setTag(TAG_BTN_LEFT);
    m_pMenu->addChild(m_btnLeft);
    
    //m_btnRight = ResourceUtility::createMenuItem(DIR_DIALOG"go_btn.png", DIR_DIALOG"go_btn.png", CC_CALLBACK_1(LoseBoard::onMenuCallback, this));
    //m_btnRight->setTag(TAG_BTN_RIGHT);
    //m_pMenu->addChild(m_btnRight);
    
    m_btnClose = ResourceUtility::createMenuItem(DIR_DIALOG"close_btn.png", DIR_DIALOG"close_btn.png", CC_CALLBACK_1(LoseBoard::onMenuCallback, this));
    m_btnClose->setTag(TAG_BTN_CLOSE);
    m_pMenu->addChild(m_btnClose);
    
    {
        Node* countdownBg = Sprite::create(DIR_DIALOG"coumbg.png");
        countdownBg->setPosition(Vec2(407, 207));
        this->addChild(countdownBg, 10);
        
        const Size& countdownBgSize = countdownBg->getContentSize();
        
        m_countDone = 25;
        char buf[200]; sprintf(buf, "%d", m_countDone);
        m_lbCountDown = ResourceUtility::createLabel(buf, 18, countdownBgSize, cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        m_lbCountDown->setPosition(Vec2(countdownBgSize.width / 2, countdownBgSize.height / 2));
        countdownBg->addChild(m_lbCountDown);
        
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(LoseBoard::onCountDown, this, m_countDone - 1))));
    }
    
    {
        Node* labelImg = Sprite::create(DIR_DIALOG"jxx.png");
        labelImg->setPosition(Vec2(m_background->getContentSize().width / 2, 261));
        this->addChild(labelImg);
    }
}
void LoseBoard::onCountDown(int timeLeft) {
    if (timeLeft > 0) {
        char buf[200]; sprintf(buf, "%d", timeLeft);
        m_lbCountDown->setString(buf);
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(LoseBoard::onCountDown, this, timeLeft - 1))));
    }
    else {
        this->retain();
        if (m_callback) {
            m_callback(CS_DIALOG_BTN_CLOSE);
        }
        this->removeFromParent();
        this->release();
    }
}
void LoseBoard::layout()
{
    Size contnetSize(m_background->getContentSize());
    this->setContentSize(contnetSize);
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    m_background->setPosition(Vec2(contnetSize.width / 2, contnetSize.height / 2));
    m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    m_title->setPosition(Vec2(contnetSize.width / 2, contnetSize.height + 63));
    m_btnLeft->setPosition(Vec2(302, 174));
    //m_btnRight->setPosition(Vec2(431, 140));
    m_btnClose->setPosition(Vec2(505, 446));
}
void LoseBoard::onMenuCallback(cocos2d::Ref* sender)
{
    MenuItem* menuItem = static_cast<MenuItem*>(sender);
    switch (menuItem->getTag()) {
        case TAG_BTN_LEFT:
            if (m_callback) {
                m_callback(CS_DIALOG_BTN_BACK);
            }
            break;
//        case TAG_BTN_RIGHT:
//            if (m_callback) {
//                m_callback(CS_DIALOG_BTN_NEXT);
//            }
//            break;
        case TAG_BTN_CLOSE:
            if (m_callback) {
                m_callback(CS_DIALOG_BTN_CLOSE);
            }
        default:
            break;
    }
}