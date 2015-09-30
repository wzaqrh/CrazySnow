//
//  PauseBoard.h
//  crazy_snow
//
//  Created by qrh on 15-2-14.
//
//
#pragma once
#include "cocos2d.h"

class PauseBoard : public cocos2d::Layer
{
public:
	enum enBoardItem {
		enItemClose,
		enItemSure,
		enItemLeft,
		enItemRight
	};
	CREATE_FUNC(PauseBoard);
	PauseBoard();
    ~PauseBoard();
	virtual bool init();
public:
	void addBoardClkListener(std::function<void(enBoardItem)> delegate);
	std::function<void(enBoardItem)> m_pDelegate;
private:
    void initUI();
    void layout();
    void onMenuCallback(cocos2d::Ref* sender);
private:
    cocos2d::Node* m_background;
    cocos2d::Node* m_title;
    cocos2d::Menu* m_pMenu;
    cocos2d::MenuItem* m_resume;
    cocos2d::MenuItem* m_homepage;
    cocos2d::MenuItem* m_continue;
};
