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
#include "data/GameEntity.h"
#include "data/StageData.h"
#include "data/DataStorage.h"
#include "data/GraphDataStorage.h"

using namespace cocos2d;

StageEntity::StageEntity()
:m_lockFlag(false)
,m_baseData()
,m_userData()
{
    m_graphData = new GraphData();
    m_pStorage = new GraphDataStorageXML();
    m_xmlPath = "";//FileUtils::getInstance()->getWritablePath() + "/map/";
}

StageEntity::~StageEntity() {
    CC_SAFE_DELETE(m_graphData);
    CC_SAFE_DELETE(m_pStorage);
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
void StageEntity::initBaseGraphData(bool loadFromRecord, int nRound) {
    std::string strRound = getRound(getStageId());
    
    srand(time(0));
    char buf[200]; sprintf(buf, "map/stage%d.xml", nRound);
    while (! FileUtils::getInstance()->isFileExist(buf)) {
        sprintf(buf, "map/stage%d.xml", rand()%100);
        CCLOG("try random load xml %s", buf);
    }
    m_xmlPath = FileUtils::getInstance()->fullPathForFilename(buf);
    bool res = m_pStorage->load(m_xmlPath.c_str(), *m_graphData, &m_baseData);
    CC_ASSERT(res);
    
    m_baseData.m_roundIndex = getStageId();
    m_baseData.m_lbNeedScore *= 0.5 + nRound / (2.0 * (50.0 + nRound));
    m_baseData.m_lbNeedScore *= 2;
    m_baseData.m_lbNeedScore *= 1.5;
    
    if (loadFromRecord) {
        DataStorage::Inst()->read(*m_graphData);
    }
}
void StageEntity::initUserData(bool loadFromRecord, bool firstInit) {
    if (firstInit) {
        m_userData.init(m_baseData, *UserInfo::Inst());
    }
    else {
        m_userData.next(m_baseData);
    }
    if (loadFromRecord) {
        DataStorage::Inst()->read(m_userData);
    }
    m_lockFlag = false;
}
void StageEntity::resetUserScore() {
    UserInfo::Inst()->setStageId(1);
    UserInfo::Inst()->setCurScore(0);
    UserInfo::Inst()->setTotalNeedScore(0);
}
void StageEntity::flushToUserInfo() {
    UserInfo* userInfo = UserInfo::Inst();
    userInfo->setCurScore(m_userData.getCurScore());
    userInfo->setHistMaxScore(m_userData.getCurScore());
    userInfo->setTotalNeedScore(m_userData.getTotalNeedScore());
}
void StageEntity::saveRecord() {
    int stageFlag = 0;
    if (m_userData.getStageClearFlag()) {
        stageFlag = 1;
    }
    else if (m_lockFlag) {
        stageFlag = -1;
    }
    DataStorage::Inst()->save(stageFlag);
    
    DataStorage::Inst()->save(*m_graphData);
    DataStorage::Inst()->save(m_userData);
    
    flushToUserInfo();
    DataStorage::Inst()->save(*UserInfo::Inst());
}

void StageEntity::beginRound(int nRound) {
    resetUserScore();
	initBaseGraphData(false, nRound);
    initUserData(false, true);
    
	WindowStageEntity::Inst()->loadStage(CS_STAGE_GAME, CC_CALLBACK_2(StageEntity::onStageGameLoaded, this));
}
int  StageEntity::getStageId() const {
    return UserInfo::Inst()->getStageId();
}
void StageEntity::nextRound() {
    UserInfo::Inst()->setStageId(getStageId() + 1);
    flushToUserInfo();
    
    initBaseGraphData(false, getStageId());
    initUserData(false, false);
    
    WindowStageEntity::Inst()->nextStage(CS_STAGE_GAME, CC_CALLBACK_2(StageEntity::onStageGameLoaded, this));
}
void StageEntity::loadRound() {
    int stageFlag = DataStorage::Inst()->readFlag();
    if (stageFlag == 1) {
        UserInfo::Inst()->setStageId(getStageId() + 1);
        
        initBaseGraphData(false, getStageId());
        initUserData(false, true);
        
        WindowStageEntity::Inst()->loadStage(CS_STAGE_GAME, CC_CALLBACK_2(StageEntity::onStageGameLoaded, this));
    }
    else if (stageFlag == -1) {
        //initRoundData(m_curStageId, false);
        //beginRound(1);
    }
    else {
        initBaseGraphData(true, getStageId());
        initUserData(true, true);
        
        WindowStageEntity::Inst()->loadStage(CS_STAGE_GAME, CC_CALLBACK_2(StageEntity::onStageGameLoaded, this));
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