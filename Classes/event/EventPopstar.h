#pragma once
#include <vector>
#include <functional>
#include "TilemapEvtHandler.h"

class GraphSolver_popstar;
class Graph_proxy;
class PlygonDrawer;

class IGraphLockChecker;

class TilEvtHandlerPopStar : public ITilmapEvtHandler
{
public:
	enum statge {
		Origin,
		HighLight,
		PlayAnim
	};
	TilEvtHandlerPopStar();
	virtual ~TilEvtHandlerPopStar();
public:
	virtual void create(PlygonDrawer* pDrawer, TilmapEvtDelegate* pDelegate);
	virtual bool onTouchBegin(Point2i tilepos, bool& swallow) override final;
	virtual void onTouchEnd(Point2i tilepos) override final;
private:
	void procTouchBegin(Point2i tilepos);
	bool check2HighLight(Point2i triggerPt);
	bool doPopStarAnim(std::function<void()> onAnimOk);
private:
	TilmapEvtDelegate*   m_pDelegate;
	IGraphLockChecker* m_pLockChecker;
	GraphSolver_popstar*  m_pSolver;
	Graph_proxy*  m_pProxyes;
	PlygonDrawer* m_pDrawer;
	statge  m_nStep;
	Point2i m_ptTriger;
	Point2i m_ptSecond;
	std::vector<Point2i> m_ptHighlgtSeq;
};