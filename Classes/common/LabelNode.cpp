#include "LabelNode.h"

USING_NS_CC;
#define LABELNODE_DEFAULT_FONT_SIZE  (24)
#define LABELNODE_DEFAULT_TTF_NAME ("custom_6000.ttf") ///("simhei_6000.ttf")
/************************************
 * LabelNode
 ***********************************/
LabelNode::LabelNode(cocos2d::Label* _label, const LabelNodeData& data)
:m_data(data)
{
    m_lbProtocol = new LabelNodeProtoLabelAdapter(_label, m_data.m_rectSize);
    m_lbNode = _label;
    addChild(m_lbNode);
    setContentSize(m_lbNode->getContentSize());
}

LabelNode::LabelNode(SpriteLabel* _label, const LabelNodeData& data)
:m_data(data)
{
    m_lbProtocol = new LabelNodeProtoSpriteLbAdapter(_label);
    m_lbNode = _label;
    addChild(m_lbNode);
    setContentSize(m_lbNode->getContentSize());
}

LabelNode::~LabelNode()
{
}

void LabelNode::runActionInside(cocos2d::Action* _action){
    m_lbNode->runAction(_action);
}

void LabelNode::stopAllActionsInside(){
    m_lbNode->stopAllActions();
}

void LabelNode::setColor(const Color3B& _color){
    m_lbProtocol->setTextColor(Color4B(_color.r, _color.g, _color.b, 255));
}

void LabelNode::setString(const std::string& _str)
{
    m_lbProtocol->setString(_str);
    setContentSize(m_lbNode->getContentSize());
}

const std::string& LabelNode::getString(){
    return m_lbProtocol->getString();
}

void LabelNode::setOpacity(GLubyte _opacity){
    m_lbNode->setOpacity(_opacity);
}

void LabelNode::setScale(float _scale){
    m_lbNode->setScale(_scale);
    Node::setScale(_scale);
}

/************************************
 * LabelNodeUtility
 ***********************************/
LabelNodeUtility::LabelNodeUtility(const std::string& _str)
{
	m_data.m_str = _str;
    m_data.m_enableOutline = false;
    m_data.m_fontSize = LABELNODE_DEFAULT_FONT_SIZE;
    m_data.m_hAlignment = TextHAlignment::LEFT;
    m_data.m_vAlignment = TextVAlignment::CENTER;
    m_data.m_textColor = Color4B::BLACK;
}
LabelNodeUtility::~LabelNodeUtility(){
}
LabelNode* LabelNodeUtility::build(){
    LabelNode* labelNode = nullptr;
    std::string strContent(m_data.m_str);
    if (m_plistName.empty()) {
        auto lbTTF = createLabel(m_data, strContent);
        if (lbTTF) {
            labelNode = new LabelNode(lbTTF, m_data);
            labelNode->autorelease();
        }
    }
    else {
        auto label = createSpriteLabel(m_data, strContent, m_plistName);
        if (label) {
            labelNode = new LabelNode(label, m_data);
            labelNode->autorelease();
        }
    }
    
    return labelNode;
}
Label* LabelNodeUtility::createLabel(LabelNodeData& data, const std::string& strContent)
{
    TTFConfig ttfConfig(LABELNODE_DEFAULT_TTF_NAME, data.m_fontSize, GlyphCollection::DYNAMIC);
    
    ttfConfig.outlineSize = data.m_outlineSize;
    
    Label* lbTTF = Label::createWithTTF(ttfConfig, strContent, data.m_hAlignment, data.m_rectSize.width);
    if (lbTTF) {
        /// get the label content size for adjust position.
        if (data.m_rectSize.height == 0 || data.m_rectSize.width == 0){
            data.m_rectSize = lbTTF->getContentSize();
        }
        lbTTF->setDimensions(data.m_rectSize.width, data.m_rectSize.height);
        //lbTTF->setDimensions(m_rectSize.width, 0);
        lbTTF->setVerticalAlignment(data.m_vAlignment);
        lbTTF->disableEffect();
        
        lbTTF->setPosition(data.m_rectSize.width*0.5, data.m_rectSize.height*0.5);
        lbTTF->setTextColor(data.m_textColor);
        if (data.m_outlineSize > 0) {
            lbTTF->enableOutline(data.m_outlineColor);
        }
    }
    return lbTTF;
}
SpriteLabel* LabelNodeUtility::createSpriteLabel(LabelNodeData& data, const std::string& strContent, const std::string& plistName)
{
    SpriteLabel* label = SpriteLabel::create(plistName.c_str(), data.m_rectSize);
    label->setString(strContent);
    return label;
}
LabelNodeUtility& LabelNodeUtility::setPlistName(const char* plistName) {
    m_plistName = plistName;
    return *this;
}
LabelNodeUtility& LabelNodeUtility::enableOutline(const cocos2d::Color4B& _color, int outlineSize) {
    m_data.m_enableOutline = true;
    m_data.m_outlineSize = outlineSize;
    m_data.m_outlineColor = _color;
    return *this;
}
LabelNodeUtility& LabelNodeUtility::setFontSize(int _fontSize){
    CC_ASSERT(_fontSize > 0);
    m_data.m_fontSize = _fontSize;
    return *this;
}
LabelNodeUtility& LabelNodeUtility::setContentSize(const cocos2d::Size& _size){
    m_data.m_rectSize = _size;
    return *this;
}
LabelNodeUtility& LabelNodeUtility::setHorizonAlign(cocos2d::TextHAlignment _align){
    m_data.m_hAlignment = _align;
    return *this;
}
LabelNodeUtility& LabelNodeUtility::setVerticalAlign(cocos2d::TextVAlignment _align){
    m_data.m_vAlignment = _align;
    return *this;
}
LabelNodeUtility& LabelNodeUtility::setColor(const cocos2d::Color4B& _color){
    m_data.m_textColor = _color;
    return *this;
}
LabelNodeUtility& LabelNodeUtility::setColor(const cocos2d::Color3B& _color){
    Color4B newColor(_color.r, _color.g, _color.b, 255);
    return setColor(newColor);
}
LabelNodeUtility& LabelNodeUtility::setLabelData(const LabelNodeData& data){
    m_data = data;
    return *this;
}