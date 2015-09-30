//
//  DataStorage.cpp
//  crazy_snow
//
//  Created by qrh on 15-9-26.
//
//

#include "DataStorage.h"
#include "graph/GraphMatrix.h"
#include "data/GameEntity.h"
#include "graph/RollCardUtinity.h"
using namespace cocos2d;

int DataStorage::readFlag() {
    UserDefault* pUser = UserDefault::getInstance();
    
    int lastStageFlag = pUser->getIntegerForKey("lastStageFlag", 0);
    return lastStageFlag;
}
void DataStorage::save(int lastStageFlag) {
    UserDefault* pUser = UserDefault::getInstance();
    
    pUser->setIntegerForKey("lastStageFlag", lastStageFlag);
    pUser->flush();
}

bool DataStorage::read(UserInfo& info) {
    UserDefault* pUser = UserDefault::getInstance();
    CCLOG("UserDefault storage %s", pUser->getXMLFilePath().c_str());
    
    if (! pUser->getBoolForKey("userinfo")) {
		return false;
	}
    
    info.setSoundEnbale(pUser->getBoolForKey("soundEnable", true));
    info.setStageId(pUser->getIntegerForKey("stageId", 1));
    info.addDiamonds(pUser->getIntegerForKey("diamonds", 0));
    info.setHistMaxScore(pUser->getIntegerForKey("histmaxscore", 0));
    info.setCurScore(pUser->getIntegerForKey("curScore", 0));
    info.setTotalNeedScore(pUser->getIntegerForKey("getTotalNeedScore", 0));
    return true;
}
void DataStorage::save(const UserInfo& info) {
    UserDefault* pUser = UserDefault::getInstance();
    
	pUser->setBoolForKey("userinfo", true);
    
    pUser->setBoolForKey("soundEnable", info.isSoundEnable());
    pUser->setIntegerForKey("stageId", info.getStageId());
    pUser->setIntegerForKey("diamonds", info.getDiamonds());
    pUser->setIntegerForKey("histmaxscore", info.getHistMaxScore());
    pUser->setIntegerForKey("curScore", info.getCurScore());
    pUser->setIntegerForKey("getTotalNeedScore", info.getTotalNeedScore());
    pUser->flush();
}

bool DataStorage::test() {
	UserDefault* pUser = UserDefault::getInstance();
	bool ret = false;
    if (pUser->getBoolForKey("user") && pUser->getBoolForKey("graph")) {
        ret = true;
    }
    return ret && readFlag() != -1;
}
bool DataStorage::read(StageUserData& usrDoc) {
	UserDefault* pUser = UserDefault::getInstance();
    
	if (! pUser->getBoolForKey("user")) {
		return false;
	}
    
    //usrDoc.setTotalNeedScore(pUser->getIntegerForKey("getTotalNeedScore", 0));
    usrDoc.addBonusStar(pUser->getIntegerForKey("getBonusStar", 0));
    usrDoc.setCurBonusType(pUser->getIntegerForKey("getCurBonusType", 0));
    usrDoc.setCurBonusType(pUser->getIntegerForKey("getNxtBonusType", 0));
    //usrDoc.addCurScore(pUser->getIntegerForKey("getCurScore", 0));
    
	return true;
}
void DataStorage::save(const StageUserData& usrDoc) {
	UserDefault* pUser = UserDefault::getInstance();
    
	pUser->setBoolForKey("user", true);
    
    //pUser->setIntegerForKey("getTotalNeedScore", usrDoc.getTotalNeedScore());
    pUser->setIntegerForKey("getBonusStar", usrDoc.getBonusStar());
    pUser->setIntegerForKey("getNxtBonusType", usrDoc.getNxtBonusType());
    pUser->setIntegerForKey("getCurBonusType", usrDoc.getCurBonusType());
    //pUser->setIntegerForKey("getCurScore", usrDoc.getCurScore());
    pUser->flush();
}

bool DataStorage::read(GraphData& graphCfg) {
	UserDefault* pUser = UserDefault::getInstance();
    
	if (! pUser->getBoolForKey("graph")) {
		return false;
	}
    
	Data data;
	data = pUser->getDataForKey("graph_data");
	if (data.getSize() != sizeof(graphCfg)) {
		return false;
	}
    
	memcpy((unsigned char*)&graphCfg, data.getBytes(), sizeof(graphCfg));
	return true;
}


void DataStorage::save(const GraphData& graphCfg) {
	UserDefault* pUser = UserDefault::getInstance();
    
	pUser->setBoolForKey("graph", true);
    
	Data data; data.copy((unsigned char*)&graphCfg, sizeof(graphCfg));
	pUser->setDataForKey("graph_data", data);
    pUser->flush();
}