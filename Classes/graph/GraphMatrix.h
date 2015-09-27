//
//  ConnGraph.h
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//
#pragma once
#include "GraphCommDef.h"
#include "GraphTool.h"
#include "GraphNode.h"
#include "IElemPaddingStragy.h"

class GraphMat {
public:
	GraphMat();
	~GraphMat();

	void buildGraph(const GraphData* cfg);
	
	bool check_bdy(Point2i pt) const { return m_graph_vis.check_bdy(pt); }
	bool check_bdy_enable(Point2i pt) const { return m_graph_vis.check_bdy(pt) && get_node(pt)->isEnable(); }
	
	bool isNodeLocked(Point2i pt) const { return get_node(pt)->getAttrib().bLock; }
	void setNodeLocked(Point2i pt, bool bLock);
	void setAllNodeLocked(bool bLock);
	
	GraphNode* get_node(Point2i pt) const { assert(check_bdy(pt)); return vertex[pt.y][pt.x]; }
	NodeCategory get_nodeCatg(Point2i pt) const { return get_node(pt)->get_category(); }
	void set_node(Point2i pt, const GraphAttrib& node_attrib);
	void create_node(Point2i pt, NodeCategory category);
	void erase_node(Point2i pt);
	void swap_node(Point2i pt1, Point2i pt2);
	
	void dump();
	void getColorSeq(const std::vector<Point2i>& ptSeq, std::vector<enNodeColor>& colorSeq);
	void getEnblSeq(std::vector<Point2i>& ptSeq);
    
    void serilize(GraphData& iniData);
public:
	GraphDistance m_distance;
	CoordConvert  m_coordCvt;
	GraphNodeVisitorGraph m_graph_vis;

	int m_rows;
	int m_cols;
	int m_max_category_num;
private:
	GraphNode* vertex[MAX_VERTEX_ROWS][MAX_VERTEX_COLS];
};
