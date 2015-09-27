//
//  ConnGraph.cpp
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//
#include "GraphMatrix.h"
#include "GraphNode.h"
#include "GraphIterator.h"


GraphMat::GraphMat()
:m_rows(0)
,m_cols(0)
,m_max_category_num(0)
{
	memset(vertex, 0, sizeof(vertex));
}

GraphMat::~GraphMat() 
{
	GraphNodeVisitorGraph vis;
	for (vis.begin(m_rows, m_cols); !vis.end(); ++vis) {
		assert(vertex[vis.m_ptCur.y][vis.m_ptCur.x]);
		delete vertex[vis.m_ptCur.y][vis.m_ptCur.x];
	}
}

void GraphMat::buildGraph(const GraphData* cfg)
{
	m_rows = cfg->rows;
	m_cols = cfg->cols;
	m_max_category_num = cfg->max_category;

	GraphNodeVisitorGraph vis;
	for (vis.begin(m_rows, m_cols); !vis.end(); ++vis)
	{
		vertex[vis.m_ptCur.y][vis.m_ptCur.x] = GraphNode::createEmptyNode();
		vertex[vis.m_ptCur.y][vis.m_ptCur.x]->setAttrib(cfg->map[vis.m_ptCur.y][vis.m_ptCur.x]);
		//vertex[vis.m_ptCur.y][vis.m_ptCur.x]->setEnable(true);
	}
	m_graph_vis.begin(m_rows, m_cols);

	m_distance.configure(m_rows, m_cols, cfg->max_category);
	m_coordCvt.configure(cfg->offsert_x, cfg->offsert_y, cfg->box_width, cfg->box_height, cfg->diff_row_off_x);
}

void GraphMat::setNodeLocked(Point2i pt, bool bLock) {
	get_node(pt)->getAttrib().bLock = bLock;
}

void GraphMat::setAllNodeLocked(bool bLock) {
	GraphNodeVisitorGraph vis;
	for (vis.begin(m_rows, m_cols); !vis.end(); ++vis) {
		setNodeLocked(vis.m_ptCur, bLock);
	}
}

void GraphMat::set_node(Point2i pt, const GraphAttrib& node_attrib){
	assert( check_bdy(pt) && get_node(pt)->isEnable() );
	get_node(pt)->setAttrib(node_attrib);
}

void GraphMat::create_node(Point2i pt, NodeCategory category) {
	get_node(pt)->getAttrib().catgy = category;
	get_node(pt)->setEnable(true);
}

void GraphMat::erase_node(Point2i pt){
	assert( check_bdy(pt) );
	get_node(pt)->setEnable(false);
}

void GraphMat::swap_node(Point2i pt1, Point2i pt2){
	assert( check_bdy(pt1) && check_bdy(pt2) );
	get_node(pt1)->swap_value( *get_node(pt2) );
}

void GraphMat::dump() 
{
	GraphNodeVisitorGraph vis;
	for (vis.begin(m_rows, m_cols); !vis.end(); ++vis)
	{
		GraphNode* node = get_node(vis.m_ptCur);
		NodeCategory categ = node->get_category();
		CCLOG("(%d,%d):color=%d spec=%d _data=%d",vis.m_ptCur.x, vis.m_ptCur.y, categ.color, categ.spec, categ.spec_data);
	}
}

void GraphMat::getColorSeq(const std::vector<Point2i>& ptSeq, std::vector<enNodeColor>& colorSeq) {
	colorSeq.clear();
	for (int i = 0; i < ptSeq.size(); ++i) {
		colorSeq.push_back(get_nodeCatg(ptSeq[i]).color);
	}
}

void GraphMat::getEnblSeq(std::vector<Point2i>& ptSeq) {
	GraphNodeVisitorGraph vis;
	for (vis.begin(m_rows, m_cols); !vis.end(); ++vis) {
		if (check_bdy_enable(vis.m_ptCur)) {
			ptSeq.push_back(vis.m_ptCur);
		}
	}
}

void GraphMat::serilize(GraphData& graphCfg) {
	GraphNodeVisitorGraph vis;
	for (vis.begin(m_rows, m_cols); !vis.end(); ++vis) {
		graphCfg.map[vis.m_ptCur.y][vis.m_ptCur.x] = get_node(vis.m_ptCur)->getAttrib();
	}
}
