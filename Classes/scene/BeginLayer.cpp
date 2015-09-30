#include "BeginLayer.h"
#include "entity/StageEntity.h"
#include "entity/ShopEntity.h"
#include "entity/WindowStageEntity.h"
#include "data/UserInfo.h"
#include "data/DataStorage.h"
#include "common/ResourceUtility.h"
#include "common/SoundPool.h"
using namespace cocos2d;

enum enTag {
    TAG_BTN_NEWGAME = 1001,
    TAG_BTN_CONTINUE,
    TAG_BTN_SHOP,
    TAG_BTN_HELP,
    TAG_BTN_VOL,
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
    initGlobal();
    initSound();
	initUI();
	updateUI();
    addParticleEffect();
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

void BeginLayer::initGlobal() {
    DataStorage::Inst()->read(*UserInfo::Inst());
}

void BeginLayer::initSound() {
    SoundPool::Inst()->playBackGroundMusic();
}

void BeginLayer::updateUI() {
	if (DataStorage::Inst()->test()) {
		m_pMItemResume->setEnabled(true);
	}
	else {
		m_pMItemResume->setEnabled(false);
	}
}

void BeginLayer::addParticleEffect() {
#ifdef CS_ENBALE_PARTICLE
    auto particle = ParticleSystemQuad::create(DIR_PARTICLE"snow.plist");
    particle->setAutoRemoveOnFinish(false);
    particle->setPosition(Vec2(getContentSize().width / 2, Director::getInstance()->getVisibleSize().height));
    this->addChild(particle, 5);
#endif
}

void BeginLayer::initUI()
{
	Size contentSize = getContentSize();
	{
		Sprite* background = ResourceUtility::createSprite(DIR_FRONT_PAGE, "home_bg.png");
        background->setScaleY(CC_CONTENT_SCALE_FACTOR());
		background->setPosition(contentSize.width / 2, 0);
		background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		this->addChild(background, ZORDER_BG);
	}

	{
		Sprite* logo = ResourceUtility::createSprite(DIR_FRONT_PAGE, "logo.png");
		logo->setPosition(contentSize.width / 2, 700);
		this->addChild(logo);
	}

	{
		Menu* pMenu = Menu::create(NULL);
		pMenu->setAnchorPoint(Point::ZERO);
		pMenu->setPosition(Point::ZERO);
		this->addChild(pMenu, 10);

		{
            auto _menuItem = ResourceUtility::createMenuItem("menu_start.png", "menu_start.png", DIR_FRONT_PAGE, CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
			_menuItem->setPosition(Point(contentSize.width / 2, 390));
			pMenu->addChild(_menuItem);
			_menuItem->setTag(TAG_BTN_NEWGAME);
		}
		
		{
            m_pMItemResume = ResourceUtility::createMenuItem("menu_resume.png", "menu_resume.png", DIR_FRONT_PAGE, CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
			m_pMItemResume->setPosition(Point(contentSize.width / 2, 270));
            if (DataStorage::Inst()->test()) {
                m_pMItemResume->setEnabled(false);
            }
            pMenu->addChild(m_pMItemResume);
			m_pMItemResume->setTag(TAG_BTN_CONTINUE);
		}
#ifdef CS_ENBALE_SHOP
		{
            auto _menuItem = ResourceUtility::createMenuItem("menu_shop.png", "menu_shop.png", DIR_FRONT_PAGE, CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
			_menuItem->setPosition(Point(contentSize.width / 2, 150));
			pMenu->addChild(_menuItem);
			_menuItem->setTag(TAG_BTN_SHOP);
		}
#endif
        {
            auto _menuItem = ResourceUtility::createMenuItem("menu_help.png", "menu_help.png", DIR_FRONT_PAGE, CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
			_menuItem->setPosition(Point(90, 84));
			pMenu->addChild(_menuItem);
			_menuItem->setTag(TAG_BTN_HELP);
        }
        
        {
            cocos2d::Vector<MenuItem*> arrItem;
            {
                auto itemOn = ResourceUtility::createMenuItem("menu_volume_on.png", "menu_volume_on.png", DIR_FRONT_PAGE, CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
                arrItem.pushBack(itemOn);
                
                auto itemOff = ResourceUtility::createMenuItem("menu_volume_off.png", "menu_volume_off.png", DIR_FRONT_PAGE, CC_CALLBACK_1(BeginLayer::onMenuCallback,this));
                arrItem.pushBack(itemOff);
            }
            
            auto _menuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(BeginLayer::onMenuCallback,this), arrItem);
            _menuItem->setPosition(Point(640 - 90, 84));
            pMenu->addChild(_menuItem);
			_menuItem->setTag(TAG_BTN_VOL);
            
            _menuItem->setSelectedIndex(UserInfo::Inst()->isSoundEnable() ? 0 : 1);
        }
	}
}

void BeginLayer::onMenuCallback(cocos2d::Ref* ref1)
{
	MenuItem* pItem = dynamic_cast<MenuItem*>(ref1);
	assert(pItem);

    switch (pItem->getTag()) {
        case TAG_BTN_NEWGAME:
            StageEntity::Inst()->beginRound(1);
            break;
        case TAG_BTN_CONTINUE:
            StageEntity::Inst()->loadRound();
            break;
#ifdef CS_ENBALE_SHOP
        case TAG_BTN_SHOP:
            ShopEntity::Inst()->loadShop();
            break;
#endif
        case TAG_BTN_HELP:
            WindowStageEntity::Inst()->loadDialog(CS_STAGE_HELP);
            break;
        case TAG_BTN_VOL:
        {
            bool enable = UserInfo::Inst()->isSoundEnable();
            UserInfo::Inst()->setSoundEnbale(! enable);
            static_cast<MenuItemToggle*>(pItem)->setSelectedIndex(UserInfo::Inst()->isSoundEnable() ? 0 : 1);
            
            SoundPool::Inst()->setEnable(! enable);
        }
            break;
        default:
            break;
    }
}












