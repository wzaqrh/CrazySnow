//
//  HelpLayer.h
//  crazy_snow
//
//  Created by qrh on 15-2-14.
//
//
#pragma once
#include "cocos2d.h"

class HelpLayer : public cocos2d::Node
{
public:
    HelpLayer();
    CREATE_FUNC(HelpLayer);
private:
    virtual bool init();
    void initUI();
    static cocos2d::Node* createBackground();
    void layout();
    void onMenuCallback(cocos2d::Ref* sender);
private:
    cocos2d::Node* m_background;
    cocos2d::Node* m_title;
    cocos2d::MenuItem* m_close;
    cocos2d::Menu* m_pMenu;
};
