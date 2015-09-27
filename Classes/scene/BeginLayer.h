#pragma once
#include "common/CommonDef.h"


class GameMainNode;
class ShopLayer;
class BeginLayer
    : public cocos2d::Layer
{
public:
	CREATE_FUNC(BeginLayer);
	BeginLayer();
	virtual bool init();
	virtual void onEnter();
    virtual void onExit();
private:
	void initUI();
	void updateUI();
	void onMenuCallback(cocos2d::Ref* ref1);
private:
	cocos2d::MenuItem* m_pMItemResume;
};

