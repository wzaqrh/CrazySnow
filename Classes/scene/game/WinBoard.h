//
//  WinBoard.h
//  crazy_snow
//
//  Created by qrh on 15-2-14.
//
//
#pragma once
#include "cocos2d.h"

class WinBoard : public cocos2d::Layer
{
public:
	CREATE_FUNC(WinBoard);
	WinBoard();
	virtual bool init();
public:
	void setBoardEventCallback(std::function<void(int)> delegate);
private:
    void initUI();
    void layout();
    void onMenuCallback(cocos2d::Ref* sender);
private:
    std::function<void(int)> m_callback;
    cocos2d::Node* m_background;
    cocos2d::Node* m_title;
    cocos2d::Menu* m_pMenu;
    cocos2d::MenuItem* m_btnLeft;
    cocos2d::MenuItem* m_btnRight;
    cocos2d::MenuItem* m_btnClose;
};
