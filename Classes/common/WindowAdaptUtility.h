//
//  WindowAdaptUtility.h
//  crazy_snow
//
//  Created by macos1 on 14-12-27.
//
//
#pragma once
#include "PreCompile.h"

class WindowAdaptUtility
{
public:
    static void addRootNodeToScene(cocos2d::Scene* scene, cocos2d::Node* root);
    static void addRootNodeToScene(cocos2d::Scene* scene, cocos2d::Node* root, int zorder);
};

