//
//  TileMap.h
//  proj.ios
//
//  Created by root911 on 14-5-13.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//
#pragma once
#include "common/CommonDef.h"
#include "graph/Graph.h"

class tagGraphData;
class TileMapLayer : public cocos2d::CCLayer
{
public:
	enum enCellType {
		e_cell_corrt,
		e_cell_oppos,
		e_cell_bg,
		e_cell_max
	};
    CREATE_FUNC(TileMapLayer);
    TileMapLayer();
    virtual ~TileMapLayer();
public:
	void lockBoard();
	void unlockBoard();
	bool isBoardLocked() { return m_LockCount > 0; }
public:
	virtual void initWith(GraphMat* graph, GraphData* cfg);

	void removeCell(Point2i pos, enCellType cellTyp = e_cell_corrt);
    void addCell(Point2i pos, const NodeCategory* attr, enCellType cellTyp = e_cell_corrt);
	cocos2d::Node* cellAt(const Point2i pos, enCellType cellTyp = e_cell_corrt);

	void changeCell(Point2i pos, const NodeCategory* categNew, enCellType cellTyp = e_cell_corrt);
	void swapCellRef(const Point2i posA, const Point2i posB);
	void swapCellRef(const Point2i pos, enCellType cellTypA, enCellType cellTypB);

	void removeCellAll(enCellType cellTyp = e_cell_corrt);

	/*void removeBg(Point2i pos);
	void addBg(Point2i pos);
	cocos2d::CCSprite*& BgAt(const Point2i pos);*/

    bool containCoord(cocos2d::Point coord, Point2i* pos);
private:
	void push_secondary_cells(const Point2i posA);
	void pop_secondary_cells(const Point2i posA);
	void push_main_cell(const Point2i pos);
	void pop_main_cell(const Point2i pos);
public:
    GraphMat*  m_pGraph;
private:
    cocos2d::Node* m_map[MAX_VERTEX_ROWS][MAX_VERTEX_COLS][e_cell_max];
	int m_LockCount;
};



struct BeadFactory {
    static cocos2d::Sprite* createBead(NodeCategory category);
	static cocos2d::Sprite* createBead(enBeadCategory spec);
	static cocos2d::Sprite* createBlastStar(enNodeColor color, float fscale);
};





