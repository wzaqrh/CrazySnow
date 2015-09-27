//
//  GNodeIter.cpp
//  proj.ios
//
//  Created by root911 on 14-5-22.
//  Copyright (c) 2014Äê OneClick Co.,Ltd. All rights reserved.
//
inline void cast_direct(GraphNodeVisitorLine::en_direct dir_in, GraphNodeVisitorRay::en_direct* dir_out)
{
	switch (dir_in) {
	case GraphNodeVisitorLine::e_dir_ang_0:
		dir_out[0] = GraphNodeVisitorRay::e_dir_ang2_0;
		dir_out[1] = GraphNodeVisitorRay::e_dir_ang2_180;
		break;
	case GraphNodeVisitorLine::e_dir_ang_60:
		dir_out[0] = GraphNodeVisitorRay::e_dir_ang2_60;
		dir_out[1] = GraphNodeVisitorRay::e_dir_ang2_240;
		break;
	case GraphNodeVisitorLine::e_dir_ang_120:
		dir_out[0] = GraphNodeVisitorRay::e_dir_ang2_120;
		dir_out[1] = GraphNodeVisitorRay::e_dir_ang2_300;
		break;
	default:
		assert(0);
		break;
	}
}
void GraphNodeVisitorLine::begin(Point2i pt, en_direct dir, int rows, int cols)
{
	this->m_cnt = 0;
	GraphNodeVisitorRay::en_direct dir_out[2];
	cast_direct(dir, dir_out);

	m_ray[0].begin(pt, dir_out[0], rows, cols);
	m_ray[1].begin(pt, dir_out[1], rows, cols);

	this->m_ptCur = m_ray[m_cnt].m_ptCur;
}
bool GraphNodeVisitorLine::end() const
{
	return m_cnt>=1 && m_ray[1].end();
}
void GraphNodeVisitorLine::operator++()
{
	++m_ray[this->m_cnt];
	if( this->m_cnt==0 && m_ray[this->m_cnt].end() )
		this->m_cnt++;

	this->m_ptCur = m_ray[this->m_cnt].m_ptCur;
}

inline void cast_direct(GraphNodeVisitorLine2::en_direct dir_in, GraphNodeVisitorRay2::en_direct* dir_out)
{
	switch (dir_in) {
	case GraphNodeVisitorLine2::e_dir_ang_0:
		dir_out[0] = GraphNodeVisitorRay2::e_dir_ang2_0;
		dir_out[1] = GraphNodeVisitorRay2::e_dir_ang2_180;
		break;
	case GraphNodeVisitorLine2::e_dir_ang_90:
		dir_out[0] = GraphNodeVisitorRay2::e_dir_ang2_90;
		dir_out[1] = GraphNodeVisitorRay2::e_dir_ang2_270;
		break;
	default:
		assert(0);
		break;
	}
}
void GraphNodeVisitorLine2::begin(Point2i pt, en_direct dir, int rows, int cols)
{
	this->m_cnt = 0;
	GraphNodeVisitorRay2::en_direct dir_out[2];
	cast_direct(dir, dir_out);

	m_ray[0].begin(pt, dir_out[0], rows, cols);
	m_ray[1].begin(pt, dir_out[1], rows, cols);

	this->m_ptCur = m_ray[m_cnt].m_ptCur;
}
bool GraphNodeVisitorLine2::end() const
{
	return m_cnt>=1 && m_ray[1].end();
}
void GraphNodeVisitorLine2::operator++()
{
	++m_ray[this->m_cnt];
	if( this->m_cnt==0 && m_ray[this->m_cnt].end() )
		this->m_cnt++;

	this->m_ptCur = m_ray[this->m_cnt].m_ptCur;
}

//#pragma mark GraphNodeVisitorLine3
inline void cast_direct(GraphNodeVisitorLine3::en_direct dir_in, GraphNodeVisitorRay3::en_direct* dir_out)
{
	switch (dir_in) {
	case GraphNodeVisitorLine3::e_dir_ang_45:
		dir_out[0] = GraphNodeVisitorRay3::e_dir_ang2_45;
		dir_out[1] = GraphNodeVisitorRay3::e_dir_ang2_225;
		break;
	case GraphNodeVisitorLine3::e_dir_ang_135:
		dir_out[0] = GraphNodeVisitorRay3::e_dir_ang2_135;
		dir_out[1] = GraphNodeVisitorRay3::e_dir_ang2_315;
		break;
	default:
		assert(0);
		break;
	}
}
void GraphNodeVisitorLine3::begin(Point2i pt, en_direct dir, int rows, int cols)
{
	this->m_cnt = 0;
	GraphNodeVisitorRay3::en_direct dir_out[2];
	cast_direct(dir, dir_out);

	m_ray[0].begin(pt, dir_out[0], rows, cols);
	m_ray[1].begin(pt, dir_out[1], rows, cols);

	this->m_ptCur = m_ray[m_cnt].m_ptCur;
}
bool GraphNodeVisitorLine3::end() const
{
	return m_cnt>=1 && m_ray[1].end();
}
void GraphNodeVisitorLine3::operator++()
{
	++m_ray[this->m_cnt];
	if( this->m_cnt==0 && m_ray[this->m_cnt].end() )
		this->m_cnt++;

	this->m_ptCur = m_ray[this->m_cnt].m_ptCur;
}

//#pragma mark GraphNodeVisitorGraph
void GraphNodeVisitorGraph::begin(int rows, int cols) {
	m_rows = rows;
	m_cols = cols;

	m_ptCur = makeP2(0,0);
}

bool GraphNodeVisitorGraph::end() const {
	return (m_ptCur.y>=m_rows);
}

void GraphNodeVisitorGraph::operator++() {
	if( m_ptCur.x == m_cols-1 ) {
		m_ptCur.y++;
		m_ptCur.x=0;
	}
	else
		m_ptCur.x++;
}

bool GraphNodeVisitorGraph::check_bdy(Point2i pt) const {
	if( pt.x<0 || pt.y<0 || pt.y>=m_rows || pt.x>=m_cols )
		return false;
	return true;
}
