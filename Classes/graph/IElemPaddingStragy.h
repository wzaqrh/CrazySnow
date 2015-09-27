//
//  IElemPaddingStragy.h
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//
#pragma once
#include "common/PreCompile.h"
#include "GraphCommDef.h"

class GraphMat;

struct GraphTriggerElemsProxy {
	std::vector<Point2i> trigger_elems_seq;
public:
	void clear();
};

struct GraphEraseElemsProxy {
	enum en_erase_type {
		e_erase_normal,
		e_erase_special
	};
	std::vector<enNodeColor> erase_elm_colrs;
	std::vector<Point2i> erase_elems_seq;
	std::vector<Point2i> erase_ext_elems_seq;
	std::vector<Change2i> change_elems_seq;
	int nStarNum;
	bool empty() const;
	void clear();
	void makeSolution(std::vector<Point2i>& pt_seq, std::vector<enNodeColor>& colorSeq);
};

struct GraphMoveElemsProxy
{
	std::vector<Move2i>  move_seq;
	std::vector<Point2i> padd_pos_seq;
	void clear();
};

struct GraphPaddElemsProxy
{
	std::vector<Point2i>		padd_pos_seq;
	std::vector<GraphAttrib>	padd_category_seq;
	void clear();
};

struct GrapElemsFlyInProxy
{
	cocos2d::Point glpos_start[MAX_VERTEX_ROWS][MAX_VERTEX_COLS];
	cocos2d::Point glpos_end[MAX_VERTEX_ROWS][MAX_VERTEX_COLS];
	float          cost_time[MAX_VERTEX_ROWS][MAX_VERTEX_COLS];
	int rows;
	int cols;
};

struct Graph_proxy : public cocos2d::CCNode {
	GraphTriggerElemsProxy trigger;
	GraphEraseElemsProxy   erase_solution;
	GraphMoveElemsProxy    move_solution;
	GraphPaddElemsProxy    padd_solution;
	int bOpera_success;
	void clear();
};

struct GraphEraseCfg {
	int nextBonusLeft;
	int nextBonusType;
};

class IGraphEraseSolver {
public:
    virtual ~IGraphEraseSolver() {}
	virtual void configure(const GraphEraseCfg& cfg) = 0;
	virtual int get_erase_solution(GraphMat* graph, const GraphTriggerElemsProxy& trigger_proxy, GraphEraseElemsProxy& erase_solution) = 0;
};

class IGraphMoveSolver {
public:
    virtual ~IGraphMoveSolver() {}
	virtual int get_move_solution(GraphMat* graph, const GraphEraseElemsProxy& erase_proxy, GraphMoveElemsProxy& move_solution) = 0;
};

class IGraphPaddSolver {
public:
    virtual ~IGraphPaddSolver() {}
	virtual int get_padd_solution(GraphMat* graph, const GraphMoveElemsProxy& move_proxy, GraphPaddElemsProxy& padd_solution) = 0;
};

class IGraphLockChecker {
public:
    virtual ~IGraphLockChecker() {}
	virtual int  check_lock(GraphMat* graph) = 0;
};

class IGraphFlyInOnEnterSolver {
public:
    virtual ~IGraphFlyInOnEnterSolver() {}
	virtual int get_flyin_seq(GraphMat* graph, GrapElemsFlyInProxy& flyin_seq) = 0;
};


class GraphEraseSolver_popstar : public IGraphEraseSolver {
public:
	GraphEraseSolver_popstar();
	virtual void configure(const GraphEraseCfg& cfg);
	virtual int get_erase_solution(GraphMat* graph, const GraphTriggerElemsProxy& trigger_proxy, GraphEraseElemsProxy& erase_solution);
private:
	GraphEraseCfg m_eraseCfg;
};

class GraphMoveSolver_down_left : public IGraphMoveSolver {
public:
	virtual int get_move_solution(GraphMat* graph, const GraphEraseElemsProxy& erase_proxy, GraphMoveElemsProxy& move_solution);
};

class GraphPaddSolver_random : public IGraphPaddSolver {
public:
	virtual int get_padd_solution(GraphMat* graph, const GraphMoveElemsProxy& move_proxy, GraphPaddElemsProxy& padd_solution);
};
class GraphPaddSolver_none : public IGraphPaddSolver {
public:
	virtual int  get_padd_solution(GraphMat* graph, const GraphMoveElemsProxy& move_proxy, GraphPaddElemsProxy& padd_solution);
};

class GraphLockChecker_popstar : public IGraphLockChecker {
public:
	virtual int  check_lock(GraphMat* graph);
};

class GraphFlyInOnEnterSolver_popstar 
: public IGraphFlyInOnEnterSolver
, public single_base<GraphFlyInOnEnterSolver_popstar> 
{
public:
	virtual int get_flyin_seq(GraphMat* graph, GrapElemsFlyInProxy& flyin_seq);
};

class IGraphEraseMovePaddSolver 
:public GraphEraseSolver_popstar
,public GraphMoveSolver_down_left
,public GraphPaddSolver_none 
{
public:
	int get_erase_move_padd_solution(GraphMat* graph, Graph_proxy* m_pProxyes) ;
};


class GraphSolver_popstar 
	:public IGraphEraseMovePaddSolver,public GraphLockChecker_popstar {
public:

};