//
//  UserInfo.h
//  crazy_snow
//
//  Created by qrh on 15-9-26.
//
//
#pragma once
#include "graph/GraphCommDef.h"
#include "common/TmplHelp.h"

class UserInfo {
public:
    UserInfo();
    static UserInfo* Inst() {
		static UserInfo* m_pInst = NULL;
		if (m_pInst == nullptr) {
			m_pInst = new UserInfo;
		}
		return m_pInst;
	}
    
    int  getHistMaxScore() const { return m_histMaxScore; }
    int  getCurScore() const { return m_curScore; }
    int  getTotalNeedScore() const { return m_totalNeedScore; }
    int  getStageId() const { return m_stageId; }
    
    int  getDiamonds() const { return m_diamonds; }
    void addDiamonds(int num);
    
    bool isSoundEnable() const { return m_isSoundEnable; }
    void setSoundEnbale(bool enable);
private:
    void setHistMaxScore(int score);
    void setCurScore(int score);
    void setTotalNeedScore(int score);
    void setStageId(int stageId);
private:
    bool m_isSoundEnable;
    int  m_stageId;
    int  m_histMaxScore,m_curScore,m_totalNeedScore;
    int  m_diamonds;
    friend class StageEntity;
    friend class DataStorage;
};
