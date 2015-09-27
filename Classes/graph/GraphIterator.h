//
//  GNodeIter.h
//  proj.ios
//
//  Created by root911 on 14-5-22.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//
#pragma once
#include "GraphCommDef.h"

//#pragma mark GraphNodeVisitorNeib1
class GraphNodeVisitorNeib1 {
	enum {
		enu_step_even = 0,
		enu_step_odd  = 1,
	};
	enum enu_pos_neighb {
		enPos_nb1_lft, enPos_nb1_top, enPos_nb1_rht, enPos_nb1_bot, enPos_nb1_Max
	};
public:
	void begin(Point2i pt) {
		m_cnt   = 0;
		m_ptOrg = pt;

		m_ptCur.x = m_ptOrg.x + get_dir_step()[m_cnt].x;
		m_ptCur.y = m_ptOrg.y + get_dir_step()[m_cnt].y;
		//m_pos     = get_dir_pos()[m_cnt];
	}
	bool end() const {
		return m_cnt >= enPos_nb1_Max;
	}
	void operator++() {
		m_cnt++;

		m_ptCur.x = m_ptOrg.x + get_dir_step()[m_cnt].x;
		m_ptCur.y = m_ptOrg.y + get_dir_step()[m_cnt].y;
		//m_pos     = get_dir_pos()[m_cnt];
	}
	static bool isSibling(Point2i pt1, Point2i pt2) {
		return (pt2.x >= pt1.x-1 && pt2.x <= pt1.x+1) && (pt2.y >= pt1.y-1 && pt2.y <= pt1.y+1);
	}
public:
	Point2i             m_ptCur;
	int					m_cnt;
private:
	enu_pos_neighb      m_pos;
	Point2i  m_ptOrg;
private:
	typedef Point2i ____dir_step[enPos_nb1_Max];
	static ____dir_step& get_dir_step() {
		static Point2i local_dir_step[enPos_nb1_Max] = {
			{-1,0},{0,1},{1,0},{0,-1}
		};
		return local_dir_step;
	}
};

//#pragma mark GraphNodeVisitorNeib2
class GraphNodeVisitorNeib2 {
public:
	inline void begin(Point2i pt);
	inline bool end() const ;
	inline void operator++() ;
public:
	Point2i  m_ptCur ;
};

//#pragma mark GraphNodeVisitorRay
class GraphNodeVisitorRay2 {
public:
	enum en_direct {
		e_dir_ang2_0,
		e_dir_ang2_90,
		e_dir_ang2_180,
		e_dir_ang2_270,
		en_dir_Max
	};
	void begin(Point2i pt, en_direct dir, int rows, int cols)  {
		m_rows = rows;
		m_cols = cols;
		m_dir  = dir;

		m_ptOrg = pt;

		m_ptCur.x = m_ptOrg.x + get_dir_step()[m_dir].x;
		m_ptCur.y = m_ptOrg.y + get_dir_step()[m_dir].y;
	}
	bool end() const  {
		return !check_bdy(m_ptCur);
	}
	void operator++()  {
		m_ptCur.x = m_ptCur.x + get_dir_step()[m_dir].x;
		m_ptCur.y = m_ptCur.y + get_dir_step()[m_dir].y;
	}
private:
	bool check_bdy(Point2i pt) const  {
		if( pt.x<0 || pt.y<0 || pt.y>=m_rows || pt.x>=m_cols )
			return false;
		return true;
	}
public:
	Point2i             m_ptCur ;
private:
	Point2i  m_ptOrg;

	int m_rows;
	int m_cols;
	en_direct m_dir;

	typedef Point2i ____dir_step[en_dir_Max];
	static ____dir_step& get_dir_step() {
		static Point2i local_dir_step[en_dir_Max] = {
			{1,0},{0,1},{-1,0},{0,-1}
		};
		return local_dir_step;
	}
};

//#pragma mark GraphNodeVisitorRay
class GraphNodeVisitorRay {
	enum {
		enu_step_even = 0,
		enu_step_odd  = 1,
		enu_step_Max
	};
public:
	enum en_direct {
		e_dir_ang2_0,
		e_dir_ang2_60,
		e_dir_ang2_120,
		e_dir_ang2_180,
		e_dir_ang2_240,
		e_dir_ang2_300,
		en_dir_Max
	};
	void begin(Point2i pt, en_direct dir, int rows, int cols)  {
		m_rows = rows;
		m_cols = cols;
		m_dir  = dir;

		m_ptOrg = pt;

		m_ptCur.x = m_ptOrg.x + get_dir_step()[m_ptOrg.y&0x01][m_dir].x;
		m_ptCur.y = m_ptOrg.y + get_dir_step()[m_ptOrg.y&0x01][m_dir].y;
	}
	bool end() const  {
		return !check_bdy(m_ptCur);
	}
	void operator++()  {
		m_ptCur.x = m_ptCur.x + get_dir_step()[m_ptCur.y&0x01][m_dir].x;
		m_ptCur.y = m_ptCur.y + get_dir_step()[m_ptCur.y&0x01][m_dir].y;
	}
protected:
	bool check_bdy(Point2i pt) const{
		if( pt.x<0 || pt.y<0 || pt.y>=m_rows || pt.x>=m_cols-(pt.y&0x01) )
			return false;
		return true;
	}
public:
	Point2i             m_ptCur ;
private:
	Point2i  m_ptOrg;

	int m_rows;
	int m_cols;
	en_direct m_dir;

	typedef Point2i ____dir_step[enu_step_Max][en_dir_Max];
	static ____dir_step& get_dir_step() {
		static Point2i local_dir_step[enu_step_Max][en_dir_Max] = {
			{
				{ 1, 0},{ 0, 1},{-1, 1},{-1, 0},{-1,-1},{ 0,-1}
			},
			{
				{ 1, 0},{ 1, 1},{ 0, 1},{-1, 0},{ 0,-1},{ 1,-1}
			}
		};
		return local_dir_step;
	}
};

//#pragma mark NodeVisitor_ray3
class GraphNodeVisitorRay3 {
public:
	enum en_direct {
		e_dir_ang2_45,
		e_dir_ang2_135,
		e_dir_ang2_225,
		e_dir_ang2_315,
		en_dir_Max
	};
	void begin(Point2i pt, en_direct dir, int rows, int cols)  {
		m_rows = rows;
		m_cols = cols;
		m_dir  = dir;

		m_ptOrg = pt;

		m_ptCur.x = m_ptOrg.x + get_dir_step()[m_dir].x;
		m_ptCur.y = m_ptOrg.y + get_dir_step()[m_dir].y;
	}
	bool end() const  {
		return ! check_bdy(m_ptCur);
	}
	void operator++()  {
		m_ptCur.x = m_ptCur.x + get_dir_step()[m_dir].x;
		m_ptCur.y = m_ptCur.y + get_dir_step()[m_dir].y;
	}
private:
	bool check_bdy(Point2i pt) const  {
		if( pt.x<0 || pt.y<0 || pt.y>=m_rows || pt.x>=m_cols )
			return false;
		return true;
	}
	static bool isSibling(Point2i pt1, Point2i pt2)  {
		return (pt2.x >= pt1.x-1 && pt2.x <= pt1.x+1) && (pt2.y >= pt1.y-1 && pt2.y <= pt1.y+1);
	}
public:
	Point2i             m_ptCur ;
private:
	Point2i  m_ptOrg;

	int m_rows;
	int m_cols;
	en_direct m_dir;

	typedef Point2i ____dir_step[en_dir_Max];
	static ____dir_step& get_dir_step() {
		static Point2i local_dir_step[en_dir_Max] = {
			{1,1},{-1,1},{-1,-1},{1,-1}
		};
		return local_dir_step;
	}
};

//#pragma mark GraphNodeVisitorLine
class GraphNodeVisitorLine {
public:
	enum en_direct {
		e_dir_ang_0,
		e_dir_ang_60,
		e_dir_ang_120,
		en_dir_Max
	};
	inline void begin(Point2i pt, en_direct dir, int rows, int cols) ;
	inline bool end() const ;
	inline void operator++() ;
public:
	Point2i  m_ptCur ;
protected:
	GraphNodeVisitorRay m_ray[2];
	int m_cnt;
};

//#pragma mark GraphNodeVisitorLine2
class GraphNodeVisitorLine2 {
public:
	enum en_direct {
		e_dir_ang_0,
		e_dir_ang_90,
		en_dir2_Max
	};
	inline void begin(Point2i pt, en_direct dir, int rows, int cols) ;
	inline bool end() const ;
	inline void operator++() ;
public:
	Point2i  m_ptCur ;
protected:
	GraphNodeVisitorRay2 m_ray[2];
	int m_cnt;
};

//#pragma mark GraphNodeVisitorLine3
class GraphNodeVisitorLine3 {
public:
	enum en_direct {
		e_dir_ang_45,
		e_dir_ang_135,
		en_dir2_Max
	};
	inline void begin(Point2i pt, en_direct dir, int rows, int cols) ;
	inline bool end() const ;
	inline void operator++() ;
public:
	Point2i  m_ptCur ;
protected:
	GraphNodeVisitorRay3 m_ray[2];
	int m_cnt;
};

//#pragma mark GraphNodeVisitorGraph
class GraphNodeVisitorGraph {
public:
	inline void begin(int rows, int cols);
	inline bool end() const;
	inline void operator++();
	inline bool check_bdy(Point2i pt) const;
public:
	Point2i m_ptCur ;
private:
	int     m_rows;
	int     m_cols;
};

#include "GrapgIterator.hpp"