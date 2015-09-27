//
//  LabelNodeImpl.h
//  MushroomDefense
//
//  Created by macos1 on 14-12-27.
//
//
#pragma once
#include "common/PreCompile.h"

class LabelNodeProtocol :public cocos2d::LabelProtocol {
public:
    virtual void setTextColor(const cocos2d::Color4B &color) = 0;
};
class LabelNodeProtoLabelAdapter :public LabelNodeProtocol {
public:
    LabelNodeProtoLabelAdapter(cocos2d::Label* label, cocos2d::Size& rectSize) :m_label(label) ,m_rectSize(rectSize) {}
    virtual void setTextColor(const cocos2d::Color4B &color) { m_label->setTextColor(color); }
    virtual void setString(const std::string &label);
    virtual const std::string& getString() const { return m_label->getString(); }
private:
    cocos2d::Label* m_label;
    cocos2d::Size   m_rectSize;
};
class SpriteLabel;
class LabelNodeProtoSpriteLbAdapter :public LabelNodeProtocol {
public:
    LabelNodeProtoSpriteLbAdapter(SpriteLabel* label) :m_label(label) {}
    virtual void setTextColor(const cocos2d::Color4B &color);
    virtual void setString(const std::string &label);
    virtual const std::string& getString() const;
private:
    SpriteLabel*    m_label;
};


class IStringMapper {
public:
    virtual ~IStringMapper() {}
    virtual std::string getValue(int key) = 0;
};

class SpriteLabel : public cocos2d::Node {
    SpriteLabel();
    virtual ~SpriteLabel();
public:
    static SpriteLabel* create(const char* plistName, const cocos2d::Size& rectSize, int fontDistance = 0);
    void  initWithPlist(const char* plistName);
public:
    void  setFontDistance(int fontDistance, bool flushDirty = true);
    void  setRectSize(const cocos2d::Size& rectSize, bool flushDirty = true);
    void  setString(const std::string &label, bool flushDirty = true);
    const std::string& getString();
private:
    void  autoScale();
    void  updateContent();
    void  setDirty(bool dirty);
    bool  isDirty() const { return m_dirty; }
    cocos2d::Node* getTextNode(int key);//
private:
    bool            m_dirty;
    cocos2d::Size   m_rectSize;
    int             m_fontDistance;
    std::string     m_message;
    IStringMapper*  m_mapper;
    cocos2d::Vector<cocos2d::Node*> m_arrTextNode;
    std::multimap<int, cocos2d::Node*> m_unusedTextNodes;
};

