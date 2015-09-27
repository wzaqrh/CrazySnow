//
//  StageEntity.h
//  crazy_snow
//
//  Created by macos1 on 14-12-25.
//
//
#pragma once
#include "common/CommonDef.h"
#include "WindowStageEntityType.h"
#include "data/StageData.h"

class GraphData;
class StageUserData;
class DataStorage;
class GraphMat;

class StageEntity
:public single_base<StageEntity>
{
public:
    StageEntity();
    
    void beginRound(int stageId);
	void nextRound();
	void loadRound();
	void endRound();
    
    void loadRecord();
    void saveRecord();
public:
    int  getStageId() const { return m_curStageId; }
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
    void initRoundData(int round, bool firstTime);
    void onStageGameLoaded(WindowStageID stageID, cocos2d::Node* window);
private:
    bool m_lockFlag;
    int  m_curStageId;
    StageBaseData m_baseData;
    StageUserData m_userData;
    GraphData*  m_graphData;
};