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
    void setHistMaxScore(int score);
    
    int getDiamonds() const { return m_diamonds; }
    void addDiamonds(int num);
    
    int getStageId() const { return m_stageId; }
    void setStageId(int stageId);
private:
    int m_stageId;
    int m_histMaxScore;
    int m_diamonds;
};
