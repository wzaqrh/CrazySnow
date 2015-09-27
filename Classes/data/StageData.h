//
//  StageData.h
//  crazy_snow
//
//  Created by qrh on 15-9-26.
//
//
#pragma once
#include "common/PreCompile.h"

struct StageBaseData {
	int  m_roundIndex;
	int  m_lbNeedScore;
	int  m_totalRound;
	int  m_bonusNeedStar;
};

int randNodeSpec();

class StageUserData;
class StageGameStatusVisitor {
public:
    StageGameStatusVisitor(StageUserData* target);
    ~StageGameStatusVisitor();
private:
    StageUserData* m_target;
};

enum DirtyMask {
    DIRTY_BIT_BONUS_STAR = 1<<0,
    DIRTY_BIT_BONUS_TYPE = 1<<1,
    DIRTY_BIT_CURSCORE = 1<<2,
    DIRTY_BIT_STAGE_CLEAR_FLAG = 1<<3
};
typedef std::function<void(StageUserData*)> StageUserDataObserver;
class StageUserData {
public:
    StageUserData();
    void init(const StageBaseData& baseData);
    void clear();
    
    void addObserver(void* privData, const StageUserDataObserver& observer);
    void removeObserver(void* privData);
    void notifyObservers();
    StageGameStatusVisitor lock();
public:
    int getBonusStar() const { return m_bonusCurStar; }
    void addBonusStar(int num);
    int getBonusNeedStar() const { return m_bonusNeedStar; }
    
    int getNxtBonusType() const { return m_nxtBonusType; }
    void setCurBonusType(int newBonusType);
    int getCurBonusType() const { return m_curBonusType; }
    
    int getCurScore() const { return m_lbCurScore; }
    void addCurScore(int score);
    
    bool getStageClearFlag() const { return m_stageClearFlag; }
    void setStageClearFlag(bool cleared);
    
    unsigned int getDirtyMask() const { return m_dirtyMask; }
private:
    int  m_bonusCurStar,m_bonusNeedStar;
    int  m_nxtBonusType,m_curBonusType;
    
    int  m_lbCurScore,m_lbNeedScore;
	bool m_stageClearFlag;
private:
    std::map<void*, StageUserDataObserver> m_observers;
    unsigned int m_dirtyMask;
    int m_notifyLock;
    friend class StageGameStatusVisitor;
};

