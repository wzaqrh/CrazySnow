//
//  DialogComponent.h
//  crazy_snow
//
//  Created by macos1 on 14-12-26.
//
//
#pragma once
#include "PreCompile.h"

typedef enum tagDialogTitleType {
    CS_DIALOG_TITLE_SHOP,
	CS_DIALOG_TITLE_WIN,
	CS_DIALOG_TITLE_LOSE,
	CS_DIALOG_TITLE_MAX
}DialogTitleType;
class DialogTitle : public cocos2d::Node
{
    DialogTitle();
public:
    static DialogTitle* create(DialogTitleType titleType, const char* message);
private:
    static cocos2d::Node*  makeBackGround(DialogTitleType type);
    static cocos2d::Label* makeLabel(DialogTitleType type, const char* message);
    void initLayout(DialogTitleType titleType, const char* message);
    void initShopLayout(const char* message);
private:
    cocos2d::Node*  m_backGround;
    cocos2d::Label* m_label;
};

typedef enum tagDialogType {
	CS_POPUP_DIALOG_WIN,
	CS_POPUP_DIALOG_LOSE,
	CS_POPUP_DIALOG_MAX
}DialogType;
typedef enum tagDialogButtonType {
	CS_DIALOG_BTN_CLOSE,
	CS_DIALOG_BTN_BACK,
	CS_DIALOG_BTN_CONTINUE,
	CS_DIALOG_BTN_NEXT,
	CS_DIALOG_BTN_MAX
}DialogButtonType;
typedef enum tagDialogLabelType {
	CS_DIALOG_LABEL_CON,
	CS_DIALOG_LABEL_MAX
}DialogLabelType;
typedef int PopupDialogEventType;
typedef std::function<void(PopupDialogEventType)> PopupDialogEventCallback;
class PopupDialog : public cocos2d::Node
{
	enum {
		INDEX_ONE,
		INDEX_TWO,
		INDEX_MAX
	};
	PopupDialog();
public:
	static PopupDialog* create(DialogType dialogType);
	void setTimer(int duration);
	void setDialogEventCallback(const PopupDialogEventCallback& callback);
private:
	void initLayout(DialogType dialogType);
	void initWinLayout();
	void initLoseLayout();
	void onMenuCallback(cocos2d::Ref* sender);
private:
	static DialogTitle*   makeTitle(DialogTitleType titleType);
	static cocos2d::Node* makeBackGround(int bkgType);
	static cocos2d::MenuItem* makeButton(DialogButtonType buttonType, const cocos2d::ccMenuCallback& callback);
	static cocos2d::Node* makeLabel(DialogLabelType labelType);
private:
	DialogTitle*	m_title;
	cocos2d::Node*  m_backGround;
	cocos2d::Node*	m_buttonClose;
	cocos2d::Node*	m_button[INDEX_MAX];
	cocos2d::Node*  m_label[INDEX_MAX];
	cocos2d::Label* m_timer;
private:
	cocos2d::Menu*	m_pMenu;
	PopupDialogEventCallback m_eventCallback;
};