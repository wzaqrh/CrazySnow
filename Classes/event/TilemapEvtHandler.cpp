#include "TilemapEvtHandler.h"
#include "TilmapEvtDispatcher.h"
#include "EventSpecial.h"
#include "EventPopstar.h"
#include "data/GameEntity.h"

TilmapEvtHandlerManager::~TilmapEvtHandlerManager() {
    destroyHandlers();
}

void TilmapEvtHandlerManager::addHandlerToDispatcher(TilmapEvtDispatcher* dispatcher, PlygonDrawer* pDrawer, TilmapEvtDelegate* pTilEvtDelegate) {
	m_tileEvtHandlerList.push_back(new TilEvtHandler_rook());
	m_tileEvtHandlerList.push_back(new TilEvtHandler_queen());
	m_tileEvtHandlerList.push_back(new TilEvtHandler_eleph());
	m_tileEvtHandlerList.push_back(new TilEvtHandler_artillery());
	m_tileEvtHandlerList.push_back(new TilEvtHandler_soldier());
	m_tileEvtHandlerList.push_back(new TilEvtHandler_horse());
    m_tileEvtHandlerList.push_back(new TilEvtHandlerPopStar());
	for (auto iter : m_tileEvtHandlerList) {
		((TilEvtHandler_spec*)iter)->create(pDrawer, pTilEvtDelegate);
		dispatcher->addEventHandler(iter);
	}
}

void TilmapEvtHandlerManager::destroyHandlers() {
	for (auto iter : m_tileEvtHandlerList) {
		delete iter;
	}
	m_tileEvtHandlerList.clear();
}

std::list<ITilmapEvtHandler*>& TilmapEvtHandlerManager::getTilmapEvtHandlerList() {
	return m_tileEvtHandlerList;
}