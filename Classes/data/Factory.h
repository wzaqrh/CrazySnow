#pragma once
#include "graph/GraphCommDef.h"
#include "DataStorage.h"
#include "data/StageData.h"

class StageBaseData;
class UserInfo;
class GraphMat;

class GraphDataFactory {
public:
	static GraphData* create(const std::string& name);
};

class GlobalCfgFactory {
public:
	static StageBaseData* create(const std::string& name);
};

