#include "Factory.h"
#include "graph/GraphMatrix.h"
#include "data/GameEntity.h"
#include "graph/RollCardUtinity.h"
using namespace cocos2d;

enum en_colors {
	en_red, en_green,en_blue,en_yelow,en_purpl
};

StageBaseData* GlobalCfgFactory::create(const std::string& name)
{
	int round = 0;
	sscanf(name.c_str(), "round%d", &round);
	static StageBaseData s_GlobalCfg_cur;

	s_GlobalCfg_cur.m_roundIndex   = round + 1;
	s_GlobalCfg_cur.m_totalRound   = 100;
	s_GlobalCfg_cur.m_lbNeedScore  = 1000 * (round + 1);
	s_GlobalCfg_cur.m_bonusNeedStar = 12;
	return &s_GlobalCfg_cur;
}

