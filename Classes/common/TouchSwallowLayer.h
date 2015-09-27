//
//  TouchSwallowLayer.h
//  crazy_snow
//
//  Created by macos1 on 14-12-27.
//
//
#pragma once
#include "PreCompile.h"

class TouchSwallowLayer : public cocos2d::Node
{
public:
    CREATE_FUNC(TouchSwallowLayer);
private:
    virtual bool init();
private:
    void setTouchHandler();
    bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* event);
};
