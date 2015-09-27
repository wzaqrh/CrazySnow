#pragma once
#include "TilemapEvtHandler.h"

class IGraphMoveSolver;
class IGraphLockChecker;

class TilEvtHandler_spec : public ITilmapEvtHandler
{
public:
	enum statge {
		Origin,
		ShowTip,
		PlayAnim
	};
	TilEvtHandler_spec();
	virtual ~TilEvtHandler_spec();
public:
	virtual void create(PlygonDrawer* pDrawer, TilmapEvtDelegate* pTilEvtDelegate); 
	virtual bool onTouchBegin(Point2i tilepos, bool& swallow) override final;
	virtual void onTouchEnd(Point2i tilepos) override final;
private:
	virtual bool check2tip(const Point2i& ptTriger) = 0;
	virtual bool check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk) = 0;
	bool check2undo(const Point2i& ptSecond, const Point2i& ptTriger);
protected:
	PlygonDrawer* m_pDrawer;
	TilmapEvtDelegate* m_pDelegate;
private:
	IGraphLockChecker* m_pLockChecker;
	Point2i m_ptTriger;
	Point2i m_ptSec;
	statge  m_nStep;
};

class TilEvtHandler_rook : public TilEvtHandler_spec
{
public:
	TilEvtHandler_rook();
	virtual ~TilEvtHandler_rook();
private:
	virtual bool check2tip(const Point2i& ptTriger);
	virtual bool check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk);
private:
	std::vector<Point2i> vecStk;
	IGraphMoveSolver*  m_pSolverMove;
};

class TilEvtHandler_queen : public TilEvtHandler_spec
{
private:
	virtual bool check2tip(const Point2i& ptTriger);
	virtual bool check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk);
private:
	std::vector<Point2i> vecStk;
};

class TilEvtHandler_eleph : public TilEvtHandler_spec
{
private:
	virtual bool check2tip(const Point2i& ptTriger);
	virtual bool check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk);
private:
	std::vector<Point2i> vecStk;
};

class TilEvtHandler_artillery : public TilEvtHandler_spec
{
private:
	virtual bool check2tip(const Point2i& ptTriger);
	virtual bool check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk);
private:
	std::vector<Point2i> vecStk;
};

class TilEvtHandler_soldier : public TilEvtHandler_spec
{
public:
	TilEvtHandler_soldier();
	virtual ~TilEvtHandler_soldier();
private:
	virtual bool check2tip(const Point2i& ptTriger);
	virtual bool check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk);
private:
	std::vector<Point2i> vecStk;
	IGraphMoveSolver* m_pSolverMove;
};

class TilEvtHandler_horse : public TilEvtHandler_spec
{
public:
	TilEvtHandler_horse();
	virtual ~TilEvtHandler_horse();
private:
	virtual bool check2tip(const Point2i& ptTriger);
	virtual bool check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk);
private:
	std::vector<Point2i> vecStk;
	IGraphMoveSolver* m_pSolverMove;
};