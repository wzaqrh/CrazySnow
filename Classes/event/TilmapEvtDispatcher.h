#pragma once
#include "graph/GraphCommDef.h"

struct TilmapEvent {
	enum Type {
		TouchBegin,
		TouchEnd
	};
	Type    m_type;
	Point2i m_graphPos;
	float   m_duration;
public:
	TilmapEvent(Type type, Point2i pos);
};

class ITilmapEvtHandler;
class TilmapEvtDispatcher 
{
	typedef std::multimap<int, ITilmapEvtHandler*> ClaimedHandlerMap;
	typedef std::multimap<int, ITilmapEvtHandler*> HandlerProrMap;
public:
	void dispatchEvent(const TilmapEvent& event);
	void addEventHandler(ITilmapEvtHandler* handler, int nPrority = 0);
	void clrEventHandlers();
private:
	HandlerProrMap		m_handlerMap;
	ClaimedHandlerMap	m_claimHandlerMap;
};