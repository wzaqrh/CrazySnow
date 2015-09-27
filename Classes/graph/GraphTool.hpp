#include "GraphIterator.h"

int GraphDistance::distance(NodeCategory category1, NodeCategory category2)
{
	if( category1.color<m_max_category_num && category2.color<m_max_category_num ) {
		return edge[category1.color][category2.color];
	}
	return MAX_EDGE_VALUE;
}

int GraphDistance::distance(Point2i pt1, Point2i pt2)
{
	GraphNodeVisitorNeib1 vis;
	for (vis.begin(pt1); !vis.end(); ++vis)
		if (vis.m_ptCur==pt2)
		{
			return 0;
		}
	return MAX_EDGE_VALUE;
}

void GraphDistance::configure(int rows, int cols, int max_category_num)
{
	memset(edge, MAX_EDGE_VALUE, sizeof(edge));
	m_rows = rows;
	m_cols = cols;
	m_max_category_num = max_category_num;

	for(int i=0; i<m_max_category_num; ++i)
		edge[i][i] = 0;
}

//struct_vis
inline int merge_pos(int row, int col) {
	return (row<<8) + col;
}
void struct_vis::reset_all()
{
	m_bits.reset();
}
void struct_vis::reset(Point2i pt)
{
	assert(pt.x>=0 && pt.y>=0);
	m_bits.reset( merge_pos(pt.y,pt.x) );
}
void struct_vis::set(Point2i pt)
{
	assert(pt.x>=0 && pt.y>=0);
	m_bits.set( merge_pos(pt.y,pt.x) );
}
bool struct_vis::get(Point2i pt)
{
	assert(pt.x>=0 && pt.y>=0);
	return m_bits.test( merge_pos(pt.y,pt.x) );
}