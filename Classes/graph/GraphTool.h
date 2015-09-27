#pragma once
#include "GraphCommDef.h"
#include "cocos2d.h"

struct GraphDistance {
	inline int distance(Point2i pt1, Point2i pt2);
	inline int distance(NodeCategory category1, NodeCategory category2);
	inline void configure(int rows, int cols, int max_category_num);
protected:
	int        m_rows;
	int		   m_cols;
	int        m_max_category_num;
	int        edge[MAX_EDGE_NUM][MAX_EDGE_NUM];//from 0
};

struct CoordConvert {
	CoordConvert():m_fOffx(0.0f),m_fOffy(0.0f),m_fWidth(0.0f),m_fHigh(0.0f),m_fOdd_off_x(0.0f){}
	void configure(float offx, float offy, float width, float height, float odd_off_x) {
		m_fOffx = offx;
		m_fOffy = offy;
		m_fWidth = width;
		m_fHigh  = height;
		m_fOdd_off_x = odd_off_x;
	}
	cocos2d::Point getCoord(Point2i pos) {
		cocos2d::Point pt;
		bool bOdd = (pos.y%2==1);
		pt.x = m_fOffx + pos.x*m_fWidth + (bOdd?m_fOdd_off_x:0.0f);
		pt.y = m_fOffy + pos.y*m_fHigh;
		return pt;
	}
	Point2i getPos(cocos2d::Point pt) {
		Point2i __pt;
		__pt.y = ( pt.y )/ m_fHigh;
		__pt.x = ( pt.x - (__pt.y%2==1?m_fOdd_off_x:0.0f) ) / m_fWidth;
		return adjust_pos(__pt);
	}
	cocos2d::Size getSize() {
		return cocos2d::Size(m_fWidth, m_fHigh);
	}
private:
	Point2i adjust_pos(Point2i pos) {
		pos.x = pos.x >= 0 ? pos.x : 0;
		pos.y = pos.y >= 0 ? pos.y : 0;
		return pos;
	}
private:
	float m_fOffx;
	float m_fOffy;
	float m_fWidth;
	float m_fHigh;
	float m_fOdd_off_x;
};

struct struct_vis 
{
	inline void reset_all();
	inline void reset(Point2i pt);
	inline void set(Point2i pt);
	inline bool get(Point2i pt);
private:
	std::bitset<256*256> m_bits;
};

#include "GraphTool.hpp"