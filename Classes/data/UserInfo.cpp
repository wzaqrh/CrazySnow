//
//  UserInfo.cpp
//  crazy_snow
//
//  Created by qrh on 15-9-26.
//
//

#include "UserInfo.h"

UserInfo::UserInfo()
:m_histMaxScore(0)
,m_diamonds(0)
,m_stageId(0)
,m_isSoundEnable(true)
{}
void UserInfo::setHistMaxScore(int score) {
    m_histMaxScore = std::max(m_histMaxScore, score);
}
void UserInfo::setCurScore(int score) {
    m_curScore = score;
}
void UserInfo::setTotalNeedScore(int score) {
    m_totalNeedScore = score;
}
void UserInfo::addDiamonds(int num) {
    m_diamonds += num;
}
void UserInfo::setStageId(int stageId) {
    m_stageId = stageId;
}
void UserInfo::setSoundEnbale(bool enable) {
    m_isSoundEnable = enable;
}