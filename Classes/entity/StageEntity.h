//
//  StageEntity.h
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//
#pragma once
#include "common/CommonDef.h"
#include "entity/WindowStageEntityType.h"
#include "data/StageData.h"

class GraphData;
class StageUserData;
class DataStorage;
class GraphMat;
class GraphDataStorage;

class StageEntity
:public single_base<StageEntity>
{
public:
    StageEntity();
    ~StageEntity();
    
    void beginRound(int stageId);
	void nextRound();
	void loadRound();
	void endRound();
    
    void loadRecord();
    void saveRecord();
public:
    int  getStageId() const;
    void addRemainStarForScore(int star);
    void addStarForScore(int star, int count);
    bool checkStateSuccess() const;
    std::pair<int, int> nextBonus();
    void resetBonus();
public:
    static GraphData* graphData();
    static StageUserData* getGameUser();
    static const StageBaseData* getGameBase();
private:
    void resetUserScore();
    void flushToUserInfo();
    void initBaseGraphData(bool loadFromRecord, int round);
    void initUserData(bool loadFromRecord, bool firstInit);
    void onStageGameLoaded(WindowStageID stageID, cocos2d::Node* window);
private:
    bool m_lockFlag;
    std::string m_xmlPath;
    StageBaseData m_baseData;
    StageUserData m_userData;
    GraphData*    m_graphData;
    GraphDataStorage* m_pStorage;
};