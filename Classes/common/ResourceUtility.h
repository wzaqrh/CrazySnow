//
//  ResourceUtility.h
//  crazy_snow
//
//  Created by macos1 on 14-12-26.
//
//
#pragma once
#include "PreCompile.h"

class ResourceUtility
{
public:
    static cocos2d::MenuItem* createMenuItem(const char* _normalName, const char* _pressName, const cocos2d::ccMenuCallback& callback);
    static cocos2d::Label* createLabel(const std::string& text, float fontSize, const cocos2d::Size& dimensions, cocos2d::TextHAlignment hAlignment  = cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment vAlignment = cocos2d::TextVAlignment::CENTER);
    
    static std::string makePlistPrefix(const char* plistname);
};
