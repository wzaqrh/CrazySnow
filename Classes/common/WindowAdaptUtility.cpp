//
//  WindowAdaptUtility.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-27.
//
//

#include "WindowAdaptUtility.h"
using namespace cocos2d;

inline Vec2 transferToAnchorPosition(const cocos2d::Size& contentSize, const cocos2d::Vec2& transferAnchor, bool ignoreAnchor, const cocos2d::Vec2& originPosition, cocos2d::Vec2 originAnchor = Vec2::ANCHOR_MIDDLE)
{
    cocos2d::Vec2 absPosition = originPosition - Vec2(contentSize.width * originAnchor.x, contentSize.height * originAnchor.y);
    if (! ignoreAnchor) {
        absPosition += Vec2(contentSize.width * transferAnchor.x, contentSize.height * transferAnchor.y);
    }
    return std::move(absPosition);
}

void WindowAdaptUtility::addRootNodeToScene(cocos2d::Scene* scene, cocos2d::Node* root) {
    addRootNodeToScene(scene, root, root->getLocalZOrder());
}

void WindowAdaptUtility::addRootNodeToScene(cocos2d::Scene* scene, cocos2d::Node* root, int zorder)
{
    Size windowSize    = Director::getInstance()->getWinSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize   = Director::getInstance()->getVisibleSize();
	Size nodeSize = root->getContentSize();
	{
		//layer->setAnchorPoint(Point(0.0f,0.5f));
		//Point pos = Point( (szVis.width-szWin.width)/2, szVis.height - szWin.height );
		//layer->setPosition(pos);
        Vec2 position = transferToAnchorPosition(nodeSize, root->getAnchorPoint(), root->isIgnoreAnchorPointForPosition(), Vec2(visibleSize.width / 2, visibleSize.height / 2), Vec2::ANCHOR_MIDDLE);
        //Vec2 position = Vec2::ZERO;
        root->setPosition(position);
    }
	scene->addChild(root, zorder);
}
