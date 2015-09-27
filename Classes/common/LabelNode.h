#pragma once
#include "PreCompile.h"
#include "LabelNodeImpl.h"

typedef struct tagLabelNodeData {
    std::string m_str;
    int m_fontSize;
    cocos2d::Size m_rectSize;
    cocos2d::TextHAlignment m_hAlignment;
    cocos2d::TextVAlignment m_vAlignment;
    cocos2d::Color4B m_textColor;
    cocos2d::Color4B m_outlineColor;
    int m_outlineSize;
    bool m_enableOutline;
}LabelNodeData;

class LabelNode : public cocos2d::Node
{
private:
    LabelNode(cocos2d::Label* _label, const LabelNodeData& data);
    LabelNode(SpriteLabel* label, const LabelNodeData& data);
    virtual ~LabelNode();
public:
    void setString(const std::string& _str);
    const std::string& getString();
    
    virtual void setColor(const cocos2d::Color3B& color)override;
    virtual void setOpacity(GLubyte opacity) override;
    virtual void setScale(float scale) override;
    
    void runActionInside(cocos2d::Action* action);
    void stopAllActionsInside();
private:
    //cocos2d::Label* m_label;
    LabelNodeProtocol*      m_lbProtocol;
    cocos2d::Node*          m_lbNode;
    LabelNodeData           m_data;
    friend class LabelNodeUtility;
};

class LabelNodeUtility{
public:
    LabelNodeUtility(const std::string& _str);
    ~LabelNodeUtility();
public:
    LabelNode* build();
public:
    LabelNodeUtility& setPlistName(const char* plistName);
    LabelNodeUtility& setContentSize(const cocos2d::Size& _size);
    LabelNodeUtility& setHorizonAlign(cocos2d::TextHAlignment _align);
    LabelNodeUtility& setVerticalAlign(cocos2d::TextVAlignment _align);
    LabelNodeUtility& setColor(const cocos2d::Color4B& _color);
    LabelNodeUtility& setColor(const cocos2d::Color3B& _color);
    LabelNodeUtility& setFontSize(int _fontSize);
    LabelNodeUtility& enableOutline(const cocos2d::Color4B& _color, int outlineSize);
public:
    static SpriteLabel* createSpriteLabel(LabelNodeData& data, const std::string& strContent, const std::string& plistName);
    static cocos2d::Label* createLabel(LabelNodeData& data, const std::string& strContent);
    LabelNodeUtility& setLabelData(const LabelNodeData& data);
private:
    LabelNodeData m_data;
    std::string   m_plistName;
};