#pragma once
#include "common/CommonDef.h"
#include "GameEntity.h"

class UserInfo;
class DataStorage : public single_base<DataStorage>
{
public:
	bool test();
    bool read(UserInfo& info);
	bool read(StageUserData& usrDoc);
	bool read(GraphData& graphCfg);
    int readFlag();
public:
    void save(const UserInfo& info);
	void save(const StageUserData& usrDoc);
	void save(const GraphData& graphCfg);
    void save(int lastStageFlag);
};

