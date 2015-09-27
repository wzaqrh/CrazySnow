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
,m_stageId(1)
{}
void UserInfo::setHistMaxScore(int score) {
    m_histMaxScore = std::max(m_histMaxScore, score);
}
void UserInfo::addDiamonds(int num) {
    m_diamonds += num;
}
void UserInfo::setStageId(int stageId) {
    m_stageId = stageId;
}