//
//  LabelNodeImpl.cpp
//  MushroomDefense
//
//  Created by macos1 on 14-12-27.
//
//

#include "LabelNodeImpl.h"
#include "CommonDef.h"
using namespace cocos2d;

//#define SIMULATE_PLIST_MODE
/************************************
 * LabelNodeProtoLabelAdapter
 ***********************************/
void LabelNodeProtoLabelAdapter::setString(const std::string &label) {
    m_label->setString(label);
    
    TTFConfig ttfconfig = m_label->getTTFConfig();
    Size labelSize;
    for (labelSize = m_label->getContentSize(); ttfconfig.fontSize > 0 && (labelSize.width > m_rectSize.width || labelSize.height > m_rectSize.height); ) {
        //CCLOG("labelSize=(%f,%f) contentSize=(%f,%f)", labelSize.width, labelSize.height, m_data.m_rectSize.width, m_data.m_rectSize.height);
        --ttfconfig.fontSize;
        
        m_label->setTTFConfig(ttfconfig);
        labelSize = m_label->getContentSize();
    }
    //CCLOG("labelSize=(%f,%f) contentSize=(%f,%f)", labelSize.width, labelSize.height, m_data.m_rectSize.width, m_data.m_rectSize.height);
}
/************************************
 * LabelNodeProtoLabelAdapter
 ***********************************/
void LabelNodeProtoSpriteLbAdapter::setTextColor(const cocos2d::Color4B &color) {
    CCLOG("unsupport setTextColor for SpriteLabel");
}
void LabelNodeProtoSpriteLbAdapter::setString(const std::string &label) {
    m_label->setString(label);
}
const std::string& LabelNodeProtoSpriteLbAdapter::getString() const {
    return m_label->getString();
}

/************************************
 * IStringMapper
 ***********************************/
class StringMapperPlist :public IStringMapper {
public:
    StringMapperPlist(const char* plistName);
    virtual std::string getValue(int key) override;
private:
    std::string m_plistPrefix;
};
StringMapperPlist::StringMapperPlist(const char* plistName) {
    m_plistPrefix = ResourceUtility::makePlistPrefix(plistName);
}
std::string StringMapperPlist::getValue(int key) {
    char buf[100];
#ifndef SIMULATE_PLIST_MODE
    sprintf(buf, "%c.png", key);
#else
    sprintf(buf, "%s%c.png", m_plistPrefix.c_str(), key);
#endif
    return buf;
}
/************************************
 * SpriteLabel
 ***********************************/
SpriteLabel::SpriteLabel():m_dirty(false),m_mapper(nullptr),m_rectSize(Size::ZERO),m_fontDistance(0) {}

SpriteLabel::~SpriteLabel() {
    CC_SAFE_DELETE(m_mapper);
    for (auto iter = m_unusedTextNodes.begin(); iter != m_unusedTextNodes.end(); ++iter) {
        iter->second->release();
    }
#ifndef SIMULATE_PLIST_MODE
    this->removeAllChildren();
    //SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
#endif
}

SpriteLabel* SpriteLabel::create(const char* plistName, const cocos2d::Size& rectSize, int fontDistance) {
    SpriteLabel* label = new SpriteLabel;
    if (! label->init()) {
        delete label;
        return nullptr;
    }
    label->autorelease();
    label->initWithPlist(plistName);
    label->setRectSize(rectSize, false);
    label->setFontDistance(fontDistance, false);
    return label;
}

void  SpriteLabel::initWithPlist(const char* plistName) {
#ifndef SIMULATE_PLIST_MODE
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistName);
#endif
    m_mapper = new StringMapperPlist(plistName);
}

void  SpriteLabel::setFontDistance(int fontDistance, bool flushDirty) {
    if (m_fontDistance != fontDistance) {
        setDirty(true);
        m_fontDistance = fontDistance;
    }
    if (flushDirty) {
        updateContent();
    }
}

void  SpriteLabel::setRectSize(const cocos2d::Size& rectSize, bool flushDirty) {
    if (! m_rectSize.equals(rectSize)) {
        setDirty(true);
        m_rectSize = rectSize;
    }
    if (flushDirty) {
        updateContent();
    }
}

void  SpriteLabel::setString(const std::string &label, bool flushDirty) {
    if (m_message != label) {
        setDirty(true);
        m_message = label;
    }
    if (flushDirty) {
        updateContent();
    }
}

const std::string& SpriteLabel::getString() {
    return m_message;
}

void  SpriteLabel::setDirty(bool dirty) {
    m_dirty = dirty;
}

cocos2d::Node* SpriteLabel::getTextNode(int key) {
    auto equal_range = m_unusedTextNodes.equal_range(key);
    for (auto iter = equal_range.first; iter != equal_range.second; ++iter) {
        Node* node = iter->second;
        if (node->getParent() == nullptr) {
            m_unusedTextNodes.erase(iter);
            node->setScale(1.0f);
            return node;
        }
    }
    std::string filename = m_mapper->getValue(key);
#ifndef SIMULATE_PLIST_MODE
    Node* node = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename.c_str()));
#else
    Node* node = Sprite::create(filename.c_str());
#endif
    if (node == NULL) node = Node::create();
    
    node->retain();
    m_unusedTextNodes.insert(std::make_pair(key, node));
    
    return node;
}

void  SpriteLabel::updateContent()
{
    if (! isDirty()) return ;
    setDirty(false);
    
    std::for_each(m_arrTextNode.begin(), m_arrTextNode.end(), std::mem_fn(&Node::removeFromParent));
    m_arrTextNode.clear();
    
    Size contentSize = Size::ZERO;
    Vec2 curNodeOrigin = Vec2::ZERO;
    for (int i = 0; i < m_message.length(); ++i) {
        int key = m_message[i];
        
        Node* textNode = getTextNode(key);
        const Size& textNodeSize = textNode->getContentSize();
        textNode->setPosition(curNodeOrigin + Vec2(textNodeSize.width / 2, m_rectSize.height / 2));
        this->addChild(textNode);
        m_arrTextNode.pushBack(textNode);
        
        curNodeOrigin.x += textNodeSize.width;
        contentSize.width = curNodeOrigin.x;
        contentSize.height = std::max(contentSize.height, textNodeSize.height);
    }
    setContentSize(contentSize);
    
    autoScale();
}

void  SpriteLabel::autoScale()
{
    const Size& contentSize = this->getContentSize();
    if (contentSize.width > m_rectSize.width) {
        float scale = m_rectSize.width/contentSize.width;
        for (int i = 0; i < m_arrTextNode.size(); ++i) {
            Node* textNode = m_arrTextNode.at(i);
            const Vec2& textNodePos  = textNode->getPosition();
            //const Size& textNodeSize = textNode->getContentSize();
            textNode->setScale(scale);
            textNode->setPosition(Vec2(textNodePos.x * scale, textNodePos.y));
        }
    }
}






