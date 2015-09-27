//
//  TouchSwallowLayer.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-27.
//
//

#include "TouchSwallowLayer.h"
using namespace cocos2d;

bool TouchSwallowLayer::init() {
    if (! Node::init()) {
        return false;
    }
    setTouchHandler();
    return true;
}

void TouchSwallowLayer::setTouchHandler()
{
	{
		auto myListener = EventListenerTouchOneByOne::create();
        
		myListener->setSwallowTouches(true);
		myListener->onTouchBegan = CC_CALLBACK_2(TouchSwallowLayer::onTouchBegan, this);
		myListener->onTouchMoved = CC_CALLBACK_2(TouchSwallowLayer::onTouchMoved, this);
		myListener->onTouchEnded = CC_CALLBACK_2(TouchSwallowLayer::onTouchEnded, this);
		myListener->onTouchCancelled = CC_CALLBACK_2(TouchSwallowLayer::onTouchCancelled, this);
        
		_eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
	}
	CCLOG("World configured ok");
}

bool TouchSwallowLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event) {
    return true;
}

void TouchSwallowLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event)
{}

void TouchSwallowLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event)
{}

void TouchSwallowLayer::onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* event)
{}