#pragma once
#include "graph/GraphCommDef.h"


class ITilmapEvtHandler {
public:
	virtual bool onTouchBegin(Point2i tilepos, bool& swallow) = 0;
	virtual void onTouchEnd(Point2i tilepos) = 0;
	virtual ~ITilmapEvtHandler(){}
};

class TilmapEvtDelegate {
public:
	virtual void onTilmapLocked() = 0;
    virtual std::pair<int, int> nextBonus() = 0;
    virtual void popsNewStars(const std::vector<NodeCategory>& starCategSeq, bool preview) = 0;
    
	virtual ~TilmapEvtDelegate(){}
};

class PlygonDrawer;
class TilmapEvtDispatcher;
class TilmapEvtHandlerManager {
public:
    ~TilmapEvtHandlerManager();
	void addHandlerToDispatcher(TilmapEvtDispatcher* dispatcher, PlygonDrawer* pDrawer, TilmapEvtDelegate* pTilEvtDelegate);
	void destroyHandlers();
	std::list<ITilmapEvtHandler*>& getTilmapEvtHandlerList();
private:
    std::list<ITilmapEvtHandler*> m_tileEvtHandlerList;
};