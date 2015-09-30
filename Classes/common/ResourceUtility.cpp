//
//  ResourceUtility.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-26.
//
//

#include "ResourceUtility.h"
using namespace cocos2d;

cocos2d::MenuItem* ResourceUtility::createMenuItem(const char* _normalName, const char* _pressName, const char* plistname, const cocos2d::ccMenuCallback& callback)
{
    Sprite* _normal = createSprite(plistname, _normalName);
    Sprite* _press  = createSprite(plistname, _pressName);
    if (strcmp(_normalName, _pressName) == 0) {
        _press->setScale(0.9f);
    }
    MenuItemSprite* _menuItem = MenuItemSprite::create(_normal, _press, callback);
    return _menuItem;
}

cocos2d::Label* ResourceUtility::createLabel(const std::string& text, float fontSize, const cocos2d::Size& dimensions, cocos2d::TextHAlignment hAlignment, cocos2d::TextVAlignment vAlignment)
{
    auto label = Label::createWithSystemFont(text, "fonts/hzgb.ttf", fontSize, dimensions, hAlignment, vAlignment);
    return label;
}

std::string ResourceUtility::makePlistPrefix(const char* plistname)
{
    std::string plistPrefix = plistname;
    size_t lastSepr = plistPrefix.find_last_of(".");
    if (lastSepr != std::string::npos) {
        plistPrefix = plistPrefix.substr(0, lastSepr) + "/";
    }
    return std::move(plistPrefix);
}

cocos2d::Sprite* ResourceUtility::createSprite(const char* plistname, const char* name) {
    if (plistname) {
        auto cache = SpriteFrameCache::getInstance();
        cache->addSpriteFramesWithFile(plistname);
        return Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(name));
    }
    else
        return Sprite::create(name);
}