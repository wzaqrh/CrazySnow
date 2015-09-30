//
//  WindowAdaptUtility.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-27.
//
//

#include "WindowAdaptUtility.h"
#include "common/CommonDef.h"
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

void WindowAdaptUtility::addRootNodeToScene(cocos2d::Scene* scene, cocos2d::Node* root, int zorder) {
	root->setPosition((scene->getContentSize().width - root->getContentSize().width) / 2, (scene->getContentSize().height - root->getContentSize().height) / 2);
    if (! root->isIgnoreAnchorPointForPosition() && root->getAnchorPoint() != Vec2::ZERO) {
        root->setPosition(root->getPosition() + root->getAnchorPointInPoints());
    }
	scene->addChild(root, zorder);
}
