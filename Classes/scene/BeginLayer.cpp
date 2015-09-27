#include "BeginLayer.h"
#include "data/Factory.h"
#include "entity/StageEntity.h"
#include "entity/ShopEntity.h"
#include "WindowStageEntity.h"
#include "data/UserInfo.h"
using namespace cocos2d;

enum enTag {
    TAG_BTN_NEWGAME = 1001,
    TAG_BTN_CONTINUE,
    TAG_BTN_SHOP,
    TAG_BTN_HELP,
    TAG_BTN_VOL_ON,
    TAG_BTN_VOL_OFF
};

enum enZorder {
    ZORDER_BG = -1
};

BeginLayer::BeginLayer()
:m_pMItemResume(nullptr)
{
}

bool BeginLayer::init(){
	Layer::init();
	setContentSize(Size(640, 960));
	initUI();
	updateUI();
	return true;
}

void BeginLayer::onEnter() {
	Layer::onEnter();
	updateUI();
}

void BeginLayer::onExit() {
    Layer::onExit();
    DataStorage::Inst()->save(*UserInfo::Inst());
}

void BeginLayer::updateUI() {
	if (DataStorage::Inst()->test()) {
		m_pMItemResume->setEnabled(true);
	}
	else {
		m_pMItemResume->setEnabled(false);
	}
}

void BeginLayer::initUI()
{
    DataStorage::Inst()->read(*UserInfo::Inst());
    
	Size contentSize = getContentSize();
	{
		Sprite* background = Sprite::create(DIR_FRONT_PAGE"home_bg.png");
		background->setPosition(contentSize.width / 2, contentSize.height);
		background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		this->addChild(background, ZORDER_BG);
	}

	{
		Sprite* logo = Sprite::create(DIR_FRONT_PAGE"logo.png");
		logo->setPosition(contentSize.width / 2, 700);
		this->addChild(logo);
	}

	{
		Menu* pMenu = Menu::create(NULL);
		pMenu->setAnchorPoint(Point::ZERO);
		pMenu->setPosition(Point::ZERO);
		this->addChild(pMenu);

		{
            auto _menuItem = ResourceUtility::createMenuItem(DIR_FRONT_PAGE"menu_start.png", DIR_FRONT_PAGE"menu_start.png", CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
			_menuItem->setPosition(Point(contentSize.width / 2, 390));
			pMenu->addChild(_menuItem);
			_menuItem->setTag(TAG_BTN_NEWGAME);
		}
		
		{
            m_pMItemResume = ResourceUtility::createMenuItem(DIR_FRONT_PAGE"menu_resume.png", DIR_FRONT_PAGE"menu_resume.png", CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
			m_pMItemResume->setPosition(Point(contentSize.width / 2, 270));
			pMenu->addChild(m_pMItemResume);
			m_pMItemResume->setTag(TAG_BTN_CONTINUE);
            
            if (DataStorage::Inst()->readFlag() == -1) {
                m_pMItemResume->setEnabled(false);
            }
		}

		{
            auto _menuItem = ResourceUtility::createMenuItem(DIR_FRONT_PAGE"menu_shop.png", DIR_FRONT_PAGE"menu_shop.png", CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
			_menuItem->setPosition(Point(contentSize.width / 2, 150));
			pMenu->addChild(_menuItem);
			_menuItem->setTag(TAG_BTN_SHOP);
		}
        
        {
            auto _menuItem = ResourceUtility::createMenuItem(DIR_FRONT_PAGE"menu_help.png", DIR_FRONT_PAGE"menu_help.png", CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
			_menuItem->setPosition(Point(90, 84));
			pMenu->addChild(_menuItem);
			_menuItem->setTag(TAG_BTN_HELP);
        }
	}
}

void BeginLayer::onMenuCallback(cocos2d::Ref* ref1)
{
	MenuItemSprite* pItem = dynamic_cast<MenuItemSprite*>(ref1);
	assert(pItem);

    switch (pItem->getTag()) {
        case TAG_BTN_NEWGAME:
            StageEntity::Inst()->beginRound(0);
            break;
        case TAG_BTN_CONTINUE:
            StageEntity::Inst()->loadRound();
            break;
        case TAG_BTN_SHOP:
            ShopEntity::Inst()->loadShop();
            break;
        case TAG_BTN_HELP:
            WindowStageEntity::Inst()->loadDialog(CS_STAGE_HELP);
            break;
        default:
            break;
    }
}












