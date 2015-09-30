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
#include "event/TilemapEvtHandler.h"
#include "event/TilmapEvtDispatcher.h"
#include "entity/StageEntity.h"
#include "common/TouchSwallowLayer.h"
#include "scene/game/WinBoard.h"
#include "scene/game/LoseBoard.h"
#include "entity/WindowStageEntity.h"
#include "common/SoundPool.h"

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
    
    StageEntity::getGameUser()->removeObserver(this);
}

bool GameMainNode::init(){
	Layer::init();
    setContentSize(Size(640, 960));
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
		m_pTileMap->setPosition(Vec2((getContentSize().width - szMap.width) / 2, 0));
        //cocos2d::log("(%f,%f)", m_pTileMap->getPosition().x, m_pTileMap->getPosition().y);
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
        auto background = ResourceUtility::createSprite(DIR_MAIN, "gamebg.png");
        background->setScaleY(CC_CONTENT_SCALE_FACTOR());
        background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        background->setPosition(Vec2(getContentSize().width / 2, 0));
        this->addChild(background, ZORDER_BG);
    }
    
    StageEntity::getGameUser()->addObserver(this, CC_CALLBACK_1(GameMainNode::onStageDataChanged, this));
    
    //preloadParticle();
    
	return true;
}


static enComboType get_combo_typ(int nStars) {
	if (nStars >= 11) {
		return COMBO_FANTASTIC;
	}
	else if(nStars >= 9) {
		return COMBO_AWESOME;
	}
	else if(nStars >= 7) {
		return COMBO_COOL;
	}
	return COMBO_NULL;
}
void GameMainNode::onStageDataChanged(StageUserData* data) {
    unsigned int dirtyMask = data->getDirtyMask();
#ifdef USE_COMBO
    if (dirtyMask & DIRTY_BIT_BONUS_STAR) {
        auto combo_typ = get_combo_typ(data->getLastBonusStar());
        if (combo_typ != COMBO_NULL ) {
            m_pPlyDrawer->do_comboAnim(combo_typ, nullptr);
        }
    }
#endif
#ifdef USE_STAGE_CLEAR
    if (dirtyMask & DIRTY_BIT_STAGE_CLEAR_FLAG) {
        m_pPlyDrawer->do_stateClrAnim(nullptr);
    }
#endif
    if (dirtyMask & DIRTY_BIT_BONUS_TYPE) {
        SoundPool::Inst()->playEffect(SOUND_EFFECT_BONUS);
    }
}
void GameMainNode::onTitleEvent(int event, void* pdata) {
    if (event == GAME_TITLE_EVT_REFRESH_MAP) {
        m_pPlyDrawer->resetGraph();
    }
    else if (event == GAME_TITLE_EVT_RESET_BONUS) {
        StageEntity::Inst()->resetBonus();
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
    CCLayer::onEnter();
}
void GameMainNode::onExit() {
    m_pGraph->serilize(*StageEntity::graphData());
    StageEntity::Inst()->saveRecord();
    
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
        m_pPlyDrawer->do_stateOverAnim(std::vector<Point2i>(), CC_CALLBACK_0(GameMainNode::showResult, this, true));
        preloadWinBoard();
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

void GameMainNode::onLoadTextureOk(cocos2d::Texture2D* tex) {
    m_textures.pushBack(tex);
}
void GameMainNode::preloadWinBoard() {
    TextureCache::getInstance()->addImageAsync(DIR_ZZIMAGE"dialog.png", CC_CALLBACK_1(GameMainNode::onLoadTextureOk, this));
}
void GameMainNode::preloadParticle() {
    for (size_t i = 0; i < 4; ++i) {
        char buf[200]; sprintf(buf, DIR_PARTICLE"blast%ld.png", i + 1);
        TextureCache::getInstance()->addImageAsync(buf, CC_CALLBACK_1(GameMainNode::onLoadTextureOk, this));
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