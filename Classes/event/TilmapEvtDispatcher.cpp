#include "TilmapEvtDispatcher.h"
#include "TilemapEvtHandler.h"

TilmapEvent::TilmapEvent(Type type, Point2i pos) {
	m_type = type;
	m_graphPos = pos;
	m_duration = 0.0f;
}

void TilmapEvtDispatcher::dispatchEvent(const TilmapEvent& event) {
	if (event.m_type == TilmapEvent::TouchBegin) {
		for (auto& iter : m_handlerMap) {
			bool swallow = true;
			if (iter.second->onTouchBegin(event.m_graphPos, swallow)) {
				m_claimHandlerMap.insert(std::make_pair(iter.first, iter.second));
				if (swallow) {
					break;
				}
			}
		}
	}
	else if (event.m_type == TilmapEvent::TouchEnd) {
		for (auto& iter : m_claimHandlerMap) {
			iter.second->onTouchEnd(event.m_graphPos);
		}
		m_claimHandlerMap.clear();
	}
}

void TilmapEvtDispatcher::addEventHandler(ITilmapEvtHandler* handler, int nPrority) {
	m_handlerMap.insert(std::make_pair(nPrority, handler));
}

void TilmapEvtDispatcher::clrEventHandlers() {
	m_handlerMap.clear();
}