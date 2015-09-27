//
//  ConnBead.h
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//
#pragma once
#include "common/PreCompile.h"

#define tmpl_override
#define _interface struct

enum enNodeColor {
	NODE_COLOR_0,
	NODE_COLOR_1,
	NODE_COLOR_2,
	NODE_COLOR_3,
	NODE_COLOR_4,
	NODE_COLOR_SPEC,
    NODE_COLOR_MAX
};

//Point2i
struct Point2i {
    int x;
    int y;
};
inline bool operator==(const Point2i& op1, const Point2i& op2) {
    return op1.x == op2.x && op1.y == op2.y;
}
inline bool operator!=(const Point2i& op1, const Point2i& op2) {
    return !(op1 == op2);
}
inline Point2i makeP2(int x, int y) {
    Point2i pt = {x,y};
    return pt;
}

//Move2i
struct Move2i {
    Point2i src;
    Point2i dst;
};
inline Move2i makeMove2i(Point2i ptSrc, Point2i ptDst) {
	Move2i mv = {ptSrc,ptDst};
	return mv;
}

//0-4 normal star
//5 ?;6 车;7 马;8 炮;9 象;10 兵;11 后;12 double  
enum enBeadCategory {
	CHESS_CATEGORY_NORM,
	CHESS_CATEGORY_UNKOWN,
	CHESS_CATEGORY_ARMOR,
	CHESS_CATEGORY_HORSE,
	CHESS_CATEGORY_ARTILLERY,
	CHESS_CATEGORY_ELEPH,
	CHESS_CATEGORY_SDOLIER,
	CHESS_CATEGORY_QUEEN,
	CHESS_CATEGORY_DOUBLE
};
struct NodeCategory {
	static NodeCategory make(enNodeColor _color, enBeadCategory _spce, int specdata) {
		NodeCategory cate; cate.color = _color; cate.spec = _spce; cate.spec_data = specdata;
		return cate;
	}
	enNodeColor color;
	enBeadCategory  spec;
	int         spec_data;
private:
	bool operator==(const NodeCategory& other);
};

//Change2i
struct Change2i {
	Point2i pos;
	NodeCategory categoryA;
	NodeCategory categoryB;
};

//GraphAttrib
struct GraphAttrib {
	static GraphAttrib make(const NodeCategory& categ) {
		GraphAttrib attr = {categ, false};
		return attr;
	}
	NodeCategory catgy;
	bool		 m_bDisable;
	bool		 bLock;
};
struct Graphattr_pos
{
    GraphAttrib  attrib;
    Point2i      pos;
};


const int MAX_VERTEX_ROWS = 16;
const int MAX_VERTEX_COLS = 16;
const int MAX_EDGE_NUM    = 16;
const int MAX_EDGE_VALUE  = 0x3f3f3f3f;

#define NEIBOUR_NUM (4)

struct GraphData {
	int rows;
	int cols;
	int max_category;
	
	int box_height;        //最外层方框的高
	int box_width;         //最外层方框的宽
    
	int diff_row_off_x;     //奇数行同偶数行比较，x偏移多少
    
	int offsert_x;     //珠子在方框内x偏移
	int offsert_y;     //珠子在方框内y偏移
    
	GraphAttrib map[MAX_VERTEX_ROWS][MAX_VERTEX_COLS];
};

class GraphMat;
typedef GraphMat GraphMatrix;
