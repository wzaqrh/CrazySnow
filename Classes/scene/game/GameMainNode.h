//
//  main.h
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014Äê OneClick Co.,Ltd. All rights reserved.
//
#pragma once
#include "common/CommonDef.h"
#include "graph/GraphCommDef.h"
#include "data/GameEntity.h"
#include "event/EventPopstar.h"
#include "entity/WindowStageEntityType.h"

class TileMapLayer;
class GraphMat;
class PlygonDrawer;
class GameTitleNode;
class GameController;
class GameData;
class TilmapEvtDispatcher;
class TilmapEvtHandlerManager;
class StageUserData;

class GameMainNode
: public cocos2d::Layer
, public TilmapEvtDelegate
{
public:
	CREATE_FUNC(GameMainNode);
    GameMainNode();
    virtual ~GameMainNode();
	virtual bool init();
	bool enter();
public:
    virtual void onEnter();
    virtual void onExit();
	virtual void update(float);
public:
	virtual std::pair<int, int> nextBonus() override;
	virtual void popsNewStars(const std::vector<NodeCategory>& starCategSeq, bool preview) override;
	virtual void onTilmapLocked() override;
private:
	void setTouchHandler();
	bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
	void onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* event);
    
    void onTitleEvent(int event, void* pdata);
private:
    void showResult(bool win);
    void onDialogCreateCallback(WindowStageID windowId, cocos2d::Node* node);
    void onDialogEvent(int event);
    void onStageDataChanged(StageUserData* stageUserData);
private:
    void preloadWinBoard();
    void preloadParticle();
    void onLoadTextureOk(cocos2d::Texture2D* tex);
private:
	Point2i              m_ptCur;
    TileMapLayer*        m_pTileMap;
    GraphMat*            m_pGraph;
	PlygonDrawer*		 m_pPlyDrawer;
    
    TilmapEvtHandlerManager* m_pTileEvtHandlerManager;
	TilmapEvtDispatcher* m_pTileEvtDispatcher;

	GameTitleNode*       m_gameTitle;
	GameController*		 m_pGlbEvtProc;
    cocos2d::Vector<cocos2d::Texture2D*> m_textures;
};

#if 0
class GameController
	:public GameDatEvtListner
	,public TilEvtHandlerPopStar::TilmapEvtDelegate
{
public:
	GameController();
	virtual ~GameController();
	void attach(GameTitleNode* pScoreBdLayer, PlygonDrawer* pDrawLayer);
public:
	virtual void GameMarkWillChange(GameData* pGameDat, int starNum, int addMark);
	virtual void GameMarkChanged(GameData* pGameDat, const StageBaseData& gameCfg, const UserInfo& gameUsrDat, GameTmpValue& tmpVal, int starNum);
	virtual void GameCfgChanged(GameData* pGameDat, const StageBaseData& gameCfg);
    void showResult(bool win);
public:
	virtual std::pair<int, int> nextBonus() override;
	virtual void popsNewStars(const std::vector<NodeCategory>& starCategSeq, bool preview) override;
	virtual void onTilmapLocked() override;
private:
    void onDialogCreateCallback(WindowStageID windowId, cocos2d::Node* node);
	void onDialogEvent(int event);
private:
	GameTitleNode* m_pScoreBdLayer;
	PlygonDrawer*    m_pDrawLayer;
};
#endif


