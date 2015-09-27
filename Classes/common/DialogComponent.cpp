//
//  DialogComponent.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-26.
//
//

#include "DialogComponent.h"
#include "common/CommonDef.h"
#include "common/TouchSwallowLayer.h"

using namespace cocos2d;

template<typename _Tp1, typename _Tp2>
_Tp1* addChildSafety(_Tp1* child, _Tp2*& child_ptr, Node* parent) {
	child_ptr = child;
	if (child_ptr) {
		parent->addChild(child_ptr);
	}
	return child;
}

DialogTitle::DialogTitle()
:m_backGround(nullptr)
,m_label(nullptr)
{}

DialogTitle* DialogTitle::create(DialogTitleType titleType, const char* message) {
    DialogTitle* pNode = new DialogTitle;
    if (! pNode->init()) {
        delete pNode;
        return nullptr;
    }
    pNode->initLayout(titleType, message);
	pNode->addChild(TouchSwallowLayer::create(), -1);
    pNode->autorelease();
    return pNode;
}

cocos2d::Node*  DialogTitle::makeBackGround(DialogTitleType type)
{
	const char* backGoundName[CS_DIALOG_TITLE_MAX] = {
		DIR_SHOP"shop_headbg.png",
		DIR_DIALOG"win_bg.png",
		DIR_DIALOG"lostbg.png",
	};
	const char* filename = backGoundName[type];
    return Sprite::create(filename);
}

cocos2d::Label* DialogTitle::makeLabel(DialogTitleType type, const char* message)
{
    cocos2d::Label* label = nullptr;
    switch (type) {
        case CS_DIALOG_TITLE_SHOP:
        default:
            break;
    }
    return label;
}

void DialogTitle::initLayout(DialogTitleType titleType, const char* message)
{
    m_backGround = makeBackGround(titleType);
    this->addChild(m_backGround);
    
	addChildSafety(makeLabel(titleType, message), m_label, this);
    
    this->setContentSize(m_backGround->getContentSize());
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    switch (titleType) {
        case CS_DIALOG_TITLE_SHOP:
            initShopLayout(message);
            break;
        default:
            break;
    }
}

void DialogTitle::initShopLayout(const char* message)
{
    const Size& contentSize = getContentSize();
    m_backGround->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));
}


//PopupDialog
enum enZorder {
	ZORDER_MENU = 1
};

enum enTag {
	TAG_BTN_CLOSE,
	TAG_BTN_BACK,
	TAG_BTN_CONTINUE,
	TAG_BTN_NEXT,
};

PopupDialog::PopupDialog()
:m_title(nullptr)
,m_backGround(nullptr)
,m_buttonClose(nullptr)
,m_timer(nullptr)
{
	memset(m_button, 0, sizeof(m_button));
	memset(m_label, 0, sizeof(m_label));
}

PopupDialog* PopupDialog::create(DialogType dialogType)
{
	PopupDialog* pNode = new PopupDialog;
	if (! pNode->init()) {
		delete pNode;
		return nullptr;
	}
	pNode->initLayout(dialogType);
	pNode->autorelease();
	return pNode;
}

void PopupDialog::setDialogEventCallback(const PopupDialogEventCallback& callback) {
	m_eventCallback = callback;
}

void PopupDialog::initLayout(DialogType dialogType)
{
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	m_pMenu = Menu::create();
	m_pMenu->setAnchorPoint(Vec2::ZERO);
	m_pMenu->setPosition(Vec2::ZERO);
	this->addChild(m_pMenu, ZORDER_MENU);

	switch (dialogType) {
	case CS_POPUP_DIALOG_LOSE:
		initLoseLayout();
		break;
	case CS_POPUP_DIALOG_WIN:
		initWinLayout();
		break;
	default:
		break;
	}
}

void PopupDialog::initWinLayout()
{
	auto background = addChildSafety(makeBackGround(CS_POPUP_DIALOG_WIN), m_backGround, this);
	this->setContentSize(m_backGround->getContentSize());
	
	auto title = addChildSafety(makeTitle(CS_DIALOG_TITLE_WIN), m_title, this);
	auto close = addChildSafety(makeButton(CS_DIALOG_BTN_CLOSE, CC_CALLBACK_1(PopupDialog::onMenuCallback, this)), m_button[CS_DIALOG_BTN_CLOSE], this);
	auto back  = addChildSafety(makeButton(CS_DIALOG_BTN_BACK, CC_CALLBACK_1(PopupDialog::onMenuCallback, this)), m_button[CS_DIALOG_BTN_BACK], this);
	auto next  = addChildSafety(makeButton(CS_DIALOG_BTN_NEXT, CC_CALLBACK_1(PopupDialog::onMenuCallback, this)), m_button[CS_DIALOG_BTN_NEXT], this);

	const Size& contentSize = getContentSize();
	background->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));
	title->setPosition(Vec2(contentSize.width / 2, 188));
	close->setPosition(Vec2(490, 354));
	back->setPosition(Vec2(189, 130));
	next->setPosition(Vec2(414, 130));
}

void PopupDialog::initLoseLayout()
{
	auto background = addChildSafety(makeBackGround(CS_POPUP_DIALOG_LOSE), m_backGround, this);
	this->setContentSize(m_backGround->getContentSize());

	auto title = addChildSafety(makeTitle(CS_DIALOG_TITLE_LOSE), m_title, this);
	auto close = addChildSafety(makeButton(CS_DIALOG_BTN_CLOSE, CC_CALLBACK_1(PopupDialog::onMenuCallback, this)), m_button[CS_DIALOG_BTN_CLOSE], this);
	auto contin  = addChildSafety(makeButton(CS_DIALOG_BTN_CONTINUE, CC_CALLBACK_1(PopupDialog::onMenuCallback, this)), m_button[CS_DIALOG_BTN_CONTINUE], this);

	const Size& contentSize = getContentSize();
	background->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));
	title->setPosition(Vec2(contentSize.width / 2, 188));
	close->setPosition(Vec2(490, 354));
	contin->setPosition(Vec2(290, 130));
}

void PopupDialog::onMenuCallback(cocos2d::Ref* sender)
{
	MenuItem* menuItem = static_cast<MenuItem*>(sender);
	int index = menuItem->getTag() - TAG_BTN_CLOSE;

	if (m_eventCallback) {
		m_eventCallback(index);
	}
	
}

void PopupDialog::setTimer(int duration)
{

}

DialogTitle*   PopupDialog::makeTitle(DialogTitleType titleType)
{
	return DialogTitle::create(titleType, "");
}

cocos2d::Node* PopupDialog::makeBackGround(int bkgType)
{
	const char* filenames[CS_POPUP_DIALOG_MAX] = {
		DIR_DIALOG"win_bg.png",
		DIR_DIALOG"lostbg.png"
	};
	return Sprite::create(filenames[bkgType]);
}

cocos2d::MenuItem* PopupDialog::makeButton(DialogButtonType buttonType, const cocos2d::ccMenuCallback& callback)
{
	const char* filenames[CS_DIALOG_BTN_MAX] = {
		DIR_DIALOG"close_btn.png",
		DIR_DIALOG"back_btn.png",
		DIR_DIALOG"jxtg.png",
		DIR_DIALOG"next_b.png"
	};
	auto menuItem = ResourceUtility::createMenuItem(filenames[buttonType], filenames[buttonType], callback);
	menuItem->setTag(buttonType - CS_DIALOG_BTN_CLOSE + TAG_BTN_CLOSE);
	return menuItem;
}

cocos2d::Node* PopupDialog::makeLabel(DialogLabelType labelType)
{
	const char* filenames[CS_DIALOG_BTN_MAX] = {
		DIR_DIALOG"jxx.png",
	};
	return Sprite::create(filenames[labelType]);
}









