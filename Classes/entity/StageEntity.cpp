//
//  StageEntity.cpp
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//

#include "StageEntity.h"
#include "entity/WindowStageEntity.h"
#include "scene/game/GameMainNode.h"
#include "data/Factory.h"
#include "data/GameEntity.h"
#include "data/StageData.h"
#include "data/DataStorage.h"

using namespace cocos2d;

StageEntity::StageEntity()
:m_curStageId(0)
,m_lockFlag(false) {
    m_graphData = new GraphData;
}

GraphData* StageEntity::graphData() {
    return Inst()->m_graphData;
}
StageUserData* StageEntity::getGameUser() {
    return &Inst()->m_userData;
}
const StageBaseData* StageEntity::getGameBase() {
    return &Inst()->m_baseData;
}

inline std::string getRound(int nRound) {
	char szRound[260]; sprintf(szRound, "round%d", nRound);
	return szRound;
}
void StageEntity::initRoundData(int nRound, bool firstTime) {
    m_curStageId = nRound;
    
    std::string strRound = getRound(m_curStageId);
    m_baseData = *GlobalCfgFactory::create(strRound);
    
    firstTime ? m_userData.init(m_baseData) : m_userData.clear();
    
    *m_graphData = *GraphDataFactory::create(strRound);
}

void StageEntity::loadRecord() {
    m_curStageId = UserInfo::Inst()->getStageId();
    
    DataStorage::Inst()->read(*m_graphData);
    
    m_userData.init(m_baseData);
    DataStorage::Inst()->read(m_userData);
}
void StageEntity::saveRecord() {
    int stageFlag = 0;
    if (m_userData.getStageClearFlag()) stageFlag = 1;
    else if (m_lockFlag) stageFlag = -1;
    DataStorage::Inst()->save(stageFlag);
    
    DataStorage::Inst()->save(*m_graphData);
    DataStorage::Inst()->save(m_userData);
}

void StageEntity::beginRound(int nRound) {
	initRoundData(nRound, true);
    
	WindowStageEntity::Inst()->nextStage(CS_STAGE_GAME, CC_CALLBACK_2(StageEntity::onStageGameLoaded, this));
}
void StageEntity::nextRound() {
	m_curStageId++;
    
    initRoundData(m_curStageId, false);
    
    WindowStageEntity::Inst()->loadStage(CS_STAGE_GAME, CC_CALLBACK_2(StageEntity::onStageGameLoaded, this));
}
void StageEntity::loadRound() {
    initRoundData(m_curStageId, false);
    
    int stageFlag = DataStorage::Inst()->readFlag();
    if (stageFlag == 1) {
        nextRound();
    }
    else if (stageFlag == -1) {
        beginRound(1);
    }
    else {
        loadRecord();
        WindowStageEntity::Inst()->nextStage(CS_STAGE_GAME, CC_CALLBACK_2(StageEntity::onStageGameLoaded, this));
    }
}
void StageEntity::endRound() {
	WindowStageEntity::Inst()->backToPrevStage();
}

void StageEntity::onStageGameLoaded(WindowStageID stageID, cocos2d::Node* window) {
    GameMainNode* m_pLayerRound = static_cast<GameMainNode*>(window);
    m_pLayerRound->enter();
}

inline int CalRemainStarMark(int nRemainStar) {
	int curRound = StageEntity::Inst()->getStageId();
	return std::max(850 + curRound * 50 - 150 * nRemainStar, 0);
}
void StageEntity::addRemainStarForScore(int star) {
    auto lock = m_userData.lock();
    
    int score = CalRemainStarMark(star);
    m_userData.addCurScore(score);
    
    m_lockFlag = true;
}

inline int calStarMark(int nStar) {
	return nStar * nStar * 5;
}
void StageEntity::addStarForScore(int star, int count) {
    auto lock = m_userData.lock();
    
    m_userData.addBonusStar(star);
    m_userData.addCurScore(calStarMark(count));
}

bool StageEntity::checkStateSuccess() const {
    return m_userData.getStageClearFlag();
}
std::pair<int, int> StageEntity::nextBonus() {
    return std::make_pair(m_userData.getBonusNeedStar() - m_userData.getBonusStar(), m_userData.getNxtBonusType());
}

void StageEntity::resetBonus() {
    auto lock = m_userData.lock();
    
    m_userData.addBonusStar(m_userData.getBonusNeedStar());
}