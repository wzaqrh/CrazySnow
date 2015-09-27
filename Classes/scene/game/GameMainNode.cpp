//
//  main.cpp
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014 OneClick Co.,Ltd. All rights reserved.
//
#include "GameMainNode.h"
#include "GameTitleNode.h"
#include "graph/GraphMatrix.h"
#include "graph/IElemPaddingStragy.h"
#include "scene/board/TileMap.h"
#include "scene/board/PlygonDrawer.h"
#include "data/Factory.h"
#include "event/TilemapEvtHandler.h"
#include "event/TilmapEvtDispatcher.h"
#include "entity/StageEntity.h"
#include "common/TouchSwallowLayer.h"
#include "scene/game/WinBoard.h"
#include "scene/game/LoseBoard.h"
#include "entity/WindowStageEntity.h"


using namespace cocos2d;
using namespace std;

enum enZorder {
    ZORDER_BG = -1
};

GameMainNode::GameMainNode()
:m_pGraph(NULL) {
	m_pTileEvtDispatcher = new TilmapEvtDispatcher;
    m_pTileEvtHandlerManager = new TilmapEvtHandlerManager;
}
GameMainNode::~GameMainNode() {
    CC_SAFE_DELETE(m_pTileEvtDispatcher);
    CC_SAFE_DELETE(m_pTileEvtHandlerManager);
    CC_SAFE_DELETE(m_pGraph);
}

bool GameMainNode::init(){
	Layer::init();
	this->setContentSize(Size(640,960));
	return true;
};
bool GameMainNode::enter() {
	{
		m_pGraph = new GraphMat;
		m_pGraph->buildGraph(StageEntity::graphData());
	}

	{
		m_pTileMap = TileMapLayer::create();
		m_pTileMap->initWith(m_pGraph, StageEntity::graphData());
		this->addChild(m_pTileMap);

		Size szMap = m_pTileMap->getContentSize();
		m_pTileMap->setPosition(Vec2((DESIGN_RESOLUTION_WIDTH - szMap.width) / 2, 0));
	}

	{
		m_gameTitle = GameTitleNode::create();
        this->addChild(m_gameTitle, 5);
        
		m_gameTitle->setPosition(Vec2::ZERO);
        
		m_gameTitle->enter(nullptr);
		m_gameTitle->setEventHandler(CC_CALLBACK_2(GameMainNode::onTitleEvent, this));
	}

	{
		m_pPlyDrawer    = new PlygonDrawer;
		m_pPlyDrawer->attach(m_pGraph, m_pTileMap);
		this->addChild(m_pPlyDrawer);
		m_pPlyDrawer->release();
	}

	{
		//m_pGlbEvtProc = new GameController;
		//m_pGlbEvtProc->attach(m_gameTitle, m_pPlyDrawer);
		//gameCfgDat->addGameDatEvtListner(m_pGlbEvtProc);

		m_pTileEvtHandlerManager->addHandlerToDispatcher(m_pTileEvtDispatcher, m_pPlyDrawer, this);
	}

	{
		GrapElemsFlyInProxy flyProxy;
		GraphFlyInOnEnterSolver_popstar::Inst()->get_flyin_seq(m_pGraph, flyProxy);
		m_pPlyDrawer->do_moveCellAnim(flyProxy, CC_CALLBACK_0(GameMainNode::setTouchHandler,this));
	}

    {
        auto gameBg = Sprite::create(DIR_MAIN"gamebg.png");
        gameBg->setPosition(Vec2(DESIGN_RESOLUTION_WIDTH_HALF, DESIGN_RESOLUTION_HEIGHT_HALF));
        this->addChild(gameBg, ZORDER_BG);
    }
    
	return true;
}
void GameMainNode::onTitleEvent(int event, void* pdata) {
    if (event == GAME_TITLE_EVT_REFRESH_MAP) {
        m_pPlyDrawer->resetGraph();
    }
    else if (event == GAME_TITLE_EVT_RESET_BONUS) {
        StageEntity::Inst()->resetBonus();
    }
    else if (event == GAME_TITLE_EVT_STAGE_CLEAR) {
        m_pPlyDrawer->do_stateClrAnim(nullptr);
    }
}

void GameMainNode::setTouchHandler() {
    auto myListener = EventListenerTouchOneByOne::create();
    
    myListener->setSwallowTouches(true);
    myListener->onTouchBegan = CC_CALLBACK_2(GameMainNode::onTouchBegan, this);
    myListener->onTouchMoved = CC_CALLBACK_2(GameMainNode::onTouchMoved, this);
    myListener->onTouchEnded = CC_CALLBACK_2(GameMainNode::onTouchEnded, this);
    myListener->onTouchCancelled = CC_CALLBACK_2(GameMainNode::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, m_pTileMap);
}
bool GameMainNode::onTouchBegan(Touch* touch,Event* event) {
	Point curPoint = m_pTileMap->convertToNodeSpace(touch->getLocation());
	Point2i pos;
	if (m_pTileMap->containCoord(curPoint, &pos)) {
		m_ptCur = pos;
		m_pTileEvtDispatcher->dispatchEvent(TilmapEvent(TilmapEvent::TouchBegin, pos));
		return true;
	}
	return false;
}
void GameMainNode::onTouchMoved(Touch* touch,Event* event) {
}
void GameMainNode::onTouchEnded(Touch* touch, Event* event) {
	m_pTileEvtDispatcher->dispatchEvent(TilmapEvent(TilmapEvent::TouchEnd, m_ptCur));
}
void GameMainNode::onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* event) {
	m_pTileEvtDispatcher->dispatchEvent(TilmapEvent(TilmapEvent::TouchEnd, m_ptCur));
}

void GameMainNode::onEnter() {
	scheduleUpdate();
    CCLayer::onEnter();
}
void GameMainNode::onExit() {
	_eventDispatcher->removeEventListenersForTarget(this);
    
    UserInfo::Inst()->setHistMaxScore(StageEntity::getGameUser()->getCurScore());
    m_pGraph->serilize(*StageEntity::graphData());
    StageEntity::Inst()->saveRecord();
    
	unscheduleUpdate();
    CCLayer::onExit();
}
void GameMainNode::update(float) {
    
}


void GameMainNode::onTilmapLocked() {
	std::vector<Point2i> ptEnlSeq;
	m_pGraph->getEnblSeq(ptEnlSeq);
	StageEntity::Inst()->addRemainStarForScore(ptEnlSeq.size());
	
	m_pPlyDrawer->setBoardLocked(false);
    
	DialogType dialogType = StageEntity::Inst()->checkStateSuccess() ? CS_POPUP_DIALOG_WIN : CS_POPUP_DIALOG_LOSE;
    if (dialogType == CS_POPUP_DIALOG_WIN) {
        showResult(true);
    }
    else {
        showResult(false);
    }
}
void GameMainNode::showResult(bool win) {
    if (win) {
        WindowStageEntity::Inst()->loadDialog(CS_POPUP_WIN, CC_CALLBACK_2(GameMainNode::onDialogCreateCallback, this));
    }
    else {
        WindowStageEntity::Inst()->loadDialog(CS_POPUP_LOSE, CC_CALLBACK_2(GameMainNode::onDialogCreateCallback, this));
    }
}
void GameMainNode::onDialogCreateCallback(WindowStageID windowId, cocos2d::Node* node) {
    if (windowId == CS_POPUP_WIN) {
        WinBoard* board = static_cast<WinBoard*>(node);
        board->setBoardEventCallback(CC_CALLBACK_1(GameMainNode::onDialogEvent, this));
    }
    else if (windowId == CS_POPUP_LOSE) {
        LoseBoard* board = static_cast<LoseBoard*>(node);
        board->setBoardEventCallback(CC_CALLBACK_1(GameMainNode::onDialogEvent, this));
    }
}
void GameMainNode::onDialogEvent(int event) {
	DialogButtonType type = DialogButtonType(event);
	switch (type)
	{
        case CS_DIALOG_BTN_CLOSE:
        case CS_DIALOG_BTN_BACK:
            StageEntity::Inst()->endRound();
            break;
        case CS_DIALOG_BTN_NEXT:
            StageEntity::Inst()->nextRound();
            break;
        case CS_DIALOG_BTN_CONTINUE:
            //purchase
            break;
        default:
            break;
	}
	
}

std::pair<int, int> GameMainNode::nextBonus() {
	return StageEntity::Inst()->nextBonus();
}

void GameMainNode::popsNewStars(const std::vector<NodeCategory>& starCategSeq, bool preview) {
	int count = 0;
	int starNum = 0;
	for (int i = 0; i < starCategSeq.size(); ++i) {
		auto categ = starCategSeq[i];
		if (categ.spec == CHESS_CATEGORY_DOUBLE) {
			count += 2;
			starNum++;
		}
		else if (categ.spec != CHESS_CATEGORY_ARMOR) {
			count++;
			starNum++;
		}
	}
    if (! preview) StageEntity::Inst()->addStarForScore(starNum, count);
}

#if 0
//GameController
GameController::GameController() {
	m_pScoreBdLayer = nullptr;
	m_pDrawLayer   = nullptr;
}
GameController::~GameController() {
	CC_SAFE_RELEASE(m_pDrawLayer);
	CC_SAFE_RELEASE(m_pScoreBdLayer);
}
void GameController::attach(GameTitleNode* pScoreBdLayer, PlygonDrawer* pDrawLayer)
{
	m_pScoreBdLayer = pScoreBdLayer; m_pScoreBdLayer->retain();
	m_pDrawLayer = pDrawLayer; m_pDrawLayer->retain();
}
inline PlygonDrawer::en_combo_typ get_combo_typ(int nStars) {
	if (nStars>=11) {
		return PlygonDrawer::e_combo_fantastic;
	}
	else if(nStars>=9) {
		return PlygonDrawer::e_combo_awesome;
	}
	else if(nStars>=7) {
		return PlygonDrawer::e_combo_cool;
	}
	return PlygonDrawer::e_combo_null;
}
void GameController::GameMarkWillChange(GameData* pGameDat, int starNum, int addMark) {
	m_pScoreBdLayer->updateTips(starNum, addMark);
}
void GameController::GameMarkChanged(GameData* pGameDat, const StageBaseData& gameCfg, 
								   const UserInfo& gameUsrDat, GameTmpValue& tmpVal, int starNum) 
{
	m_pScoreBdLayer->updateCurScore(gameUsrDat.m_lbCurScore);
	
	m_pScoreBdLayer->updateBonusLabel(gameCfg.m_bonusNeedStar - gameUsrDat.m_bonusCurStar);
	
	if (tmpVal.isNewRussiaBox) {
		tmpVal.isNewRussiaBox = false;
		m_pScoreBdLayer->updateBonusImg(gameUsrDat.m_nxtBonusType);
	}
	
	if (tmpVal.m_stageClearFlag == 1) {
		tmpVal.m_stageClearFlag++;
		m_pDrawLayer->do_stateClrAnim(nullptr);
	}

	PlygonDrawer::en_combo_typ combo_typ = get_combo_typ(starNum);
	if( combo_typ != PlygonDrawer::e_combo_null ) {
		m_pDrawLayer->do_comboAnim(combo_typ, nullptr);
	}
}
void GameController::GameCfgChanged(GameData* pGameDat, const StageBaseData& gameCfg) {
	m_pScoreBdLayer->updateNeedScoreLabel(gameCfg.m_lbNeedScore);
	m_pScoreBdLayer->updateStageIdxLabel(gameCfg.m_roundIndex);

#endif
