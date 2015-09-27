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
}

bool DataStorage::read(UserInfo& info) {
    UserDefault* pUser = UserDefault::getInstance();
    
    if (! pUser->getBoolForKey("userinfo")) {
		return false;
	}
    
    info.addDiamonds(pUser->getIntegerForKey("diamonds", 0));
    info.setHistMaxScore(pUser->getIntegerForKey("histmaxscore", 0));
    
    return true;
}
void DataStorage::save(const UserInfo& info) {
    UserDefault* pUser = UserDefault::getInstance();
    
	pUser->setBoolForKey("userinfo", true);
    
    pUser->setIntegerForKey("diamonds", info.getDiamonds());
    pUser->setIntegerForKey("histmaxscore", info.getHistMaxScore());
}

bool DataStorage::test() {
	UserDefault* pUser = UserDefault::getInstance();
	bool ret = false;
    if (pUser->getBoolForKey("user") && pUser->getBoolForKey("graph")) {
        ret = true;
    }
    return ret;
}
bool DataStorage::read(StageUserData& usrDoc) {
	UserDefault* pUser = UserDefault::getInstance();
    
	if (! pUser->getBoolForKey("user")) {
		return false;
	}
    
    usrDoc.addBonusStar(pUser->getIntegerForKey("getBonusStar", 0));
    usrDoc.setCurBonusType(pUser->getIntegerForKey("getCurBonusType", 0));
    usrDoc.setCurBonusType(pUser->getIntegerForKey("getNxtBonusType", 0));
    usrDoc.addCurScore(pUser->getIntegerForKey("getCurScore", 0));
    
	return true;
}
void DataStorage::save(const StageUserData& usrDoc) {
	UserDefault* pUser = UserDefault::getInstance();
    
	pUser->setBoolForKey("user", true);
    
    pUser->setIntegerForKey("getBonusStar", usrDoc.getBonusStar());
    pUser->setIntegerForKey("getNxtBonusType", usrDoc.getNxtBonusType());
    pUser->setIntegerForKey("getCurBonusType", usrDoc.getCurBonusType());
    pUser->setIntegerForKey("getCurScore", usrDoc.getCurScore());
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
}