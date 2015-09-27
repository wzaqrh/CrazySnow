//
//  IElemPaddingStragy.cpp
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//
#include "IElemPaddingStragy.h"
#include <algorithm>
#include <iostream>
#include "GraphMatrix.h"
#include "RollCardUtinity.h"
#include "GraphCommDebug.h"

inline bool cmp_Point_inc(const Point2i& pt1, const Point2i& pt2) {
    if( pt1.y != pt2.y )
        return pt1.y < pt2.y;
    return pt1.x < pt2.x;
}
inline bool cmp_Point_dec(const Point2i& pt1, const Point2i& pt2) {
    if( pt1.y != pt2.y )
        return pt1.y > pt2.y;
    return pt1.x > pt2.x;
}
inline bool cmp_Move_inc(const Move2i& m1, const Move2i& m2)
{
    if( m1.dst.y != m2.dst.y )
        return m1.dst.y < m2.dst.y;
    return m1.dst.x < m2.dst.x;
}
inline bool cmp_Move_dec(const Move2i& m1, const Move2i& m2)
{
    if( m1.dst.y != m2.dst.y )
        return m1.dst.y > m2.dst.y;
    return m1.dst.x > m2.dst.x;
}

struct Move2iDistanceComp
{
    Move2iDistanceComp(Point2i pt):pt_chk(pt) {}
    bool operator()(const Move2i& mv) const {
        return mv.dst == pt_chk;
    }
private:
    Point2i pt_chk;
};
GraphEraseSolver_popstar::GraphEraseSolver_popstar() {

}
void GraphEraseSolver_popstar::configure(const GraphEraseCfg& cfg) {
	m_eraseCfg = cfg;
}
int GraphEraseSolver_popstar::get_erase_solution(GraphMat* graph, const GraphTriggerElemsProxy& trigger_proxy, GraphEraseElemsProxy& erase_solution)
{
	assert(erase_solution.erase_elems_seq.empty() && erase_solution.erase_ext_elems_seq.empty());

	struct_vis bVis;
	std::queue<Point2i>  que;
	GraphNodeVisitorNeib1 vis;
	GraphNodeVisitorGraph graph_vis;
	graph_vis.begin(graph->m_rows, graph->m_cols);

	GraphDistance& dis_check = graph->m_distance;

	erase_solution.nStarNum = 1;
	for (size_t i=0; i<trigger_proxy.trigger_elems_seq.size(); ++i)
	if (!bVis.get(trigger_proxy.trigger_elems_seq[i]))
	{
		Point2i ptCur = trigger_proxy.trigger_elems_seq[i];
		que.push(ptCur);
		bVis.set(ptCur);

		NodeCategory src_categ = graph->get_node(ptCur)->get_category();

		while (!que.empty())
		{
			ptCur = que.front();
			que.pop();
			
			for(vis.begin(ptCur); !vis.end(); ++vis)
				if( graph->check_bdy_enable(vis.m_ptCur) && ! bVis.get(vis.m_ptCur) 
					&& dis_check.distance(graph->get_node(vis.m_ptCur)->get_category(),src_categ)==0 )
				{
					que.push(vis.m_ptCur);
					bVis.set(vis.m_ptCur);
					erase_solution.erase_elems_seq.push_back(vis.m_ptCur);

					if (graph->get_node(vis.m_ptCur)->get_category().spec == CHESS_CATEGORY_DOUBLE) {
						++erase_solution.nStarNum;
					}
					++erase_solution.nStarNum;
				}
		}

		ptCur = trigger_proxy.trigger_elems_seq[i];
#ifdef RUSSIA_BOX
		if (erase_solution.nStarNum >= 4) {
			Change2i chg; chg.pos = ptCur; chg.categoryA = src_categ; chg.categoryB = NodeCategory::make(NODE_COLOR_SPEC, CHESS_CATEGORY_UNKOWN, erase_solution.nStarNum);
			erase_solution.change_elems_seq.push_back(chg);
		}
#else
		if (erase_solution.nStarNum >= m_eraseCfg.nextBonusLeft) {
			Change2i chg; chg.pos = ptCur; chg.categoryA = src_categ; 
			chg.categoryB = calRollCardResult(src_categ, graph, ptCur, (enBeadCategory)m_eraseCfg.nextBonusType);
			erase_solution.change_elems_seq.push_back(chg);
		}
#endif
		else {
			erase_solution.erase_elems_seq.push_back(ptCur);
		}
	}
	graph->getColorSeq(erase_solution.erase_elems_seq, erase_solution.erase_elm_colrs);
	//erase_solution.erase_type = GraphEraseElemsProxy::e_erase_normal;
	return ! (erase_solution.change_elems_seq.size() + erase_solution.erase_elems_seq.size() <= 1 && erase_solution.erase_ext_elems_seq.empty());
}


struct move2i_merger
{
	move2i_merger(std::vector<Move2i>& move_seq)
		:m_ref_move_seq(move_seq) {}
	void add_move(Point2i ptSrc, Point2i ptDst) {
		std::vector<Move2i>::iterator iter = std::find_if(m_ref_move_seq.begin(), m_ref_move_seq.end(), Move2iDistanceComp(ptSrc));
		if( iter != m_ref_move_seq.end() ) {
			iter->dst = ptDst;
		}
		else {
			Move2i move = makeMove2i(ptSrc, ptDst);
			m_ref_move_seq.push_back(move);
		}
	}
	std::vector<Move2i>& m_ref_move_seq;
};


int GraphMoveSolver_down_left::get_move_solution(GraphMat* graph, const GraphEraseElemsProxy& erase_proxy, GraphMoveElemsProxy& move_solution)
{
	assert(move_solution.move_seq.empty() && move_solution.padd_pos_seq.empty());

	std::vector<Point2i> vec_pts;
	vec_pts.assign(erase_proxy.erase_elems_seq.begin(), erase_proxy.erase_elems_seq.end());
	std::sort(vec_pts.begin(), vec_pts.end(), cmp_Point_dec);

	std::cout<<"all elimit elem list:"<<std::endl;
	for(size_t i=0; i<vec_pts.size(); ++i)
		std::cout<<"("<<vec_pts[i].x<<","<<vec_pts[i].y<<") ";
	std::cout<<std::endl;

	move2i_merger mv_merger(move_solution.move_seq);

	for(size_t i=0; i<vec_pts.size(); ++i)
	{
		Point2i    ptCur   = vec_pts[i];
		GraphNode* nodeCur = graph->get_node(ptCur);

		GraphNodeVisitorRay2 vis;
		for(vis.begin(ptCur, GraphNodeVisitorRay2::e_dir_ang2_90, graph->m_rows, graph->m_cols); !vis.end(); ++vis)
			if( graph->check_bdy_enable(vis.m_ptCur) )
			{
				GraphNode* nodePre = graph->get_node(vis.m_ptCur);
				nodeCur->assign_value(*nodePre);

				mv_merger.add_move(vis.m_ptCur, ptCur);

				ptCur   = vis.m_ptCur;
				nodeCur = nodePre;
			}

		graph->erase_node(ptCur);
		move_solution.padd_pos_seq.push_back(ptCur);
	}

	std::vector<bool>    vec_bool;
	GraphNodeVisitorGraph grph_vis;
	GraphNodeVisitorRay2 ray_vis,ray_vis2;
	grph_vis.begin(graph->m_rows, graph->m_cols);
	for(Point2i ptIter=makeP2(0,0); grph_vis.check_bdy(ptIter); ptIter.x++)
	{
		bool flag = false;
		for(ray_vis.begin(makeP2(ptIter.x,-1), GraphNodeVisitorRay2::e_dir_ang2_90, graph->m_rows, graph->m_cols); !ray_vis.end(); ++ray_vis)
		{
			if( graph->get_node(ray_vis.m_ptCur)->isEnable() ) {
				flag = true;
				break;
			}
		}
		vec_bool.push_back(flag);
	}

	for(size_t src=0,dst=0; src<vec_bool.size(); )
	{
		while( src<vec_bool.size() && !vec_bool[src] ) {
			++src;
		}

		if( src != dst && src<vec_bool.size() )
		{
			Point2i ptIter_src = makeP2(src,-1);
			Point2i ptIter_dst = makeP2(dst,-1);
			for( ray_vis.begin(ptIter_src, GraphNodeVisitorRay2::e_dir_ang2_90, graph->m_rows, graph->m_cols),
				ray_vis2.begin(ptIter_dst, GraphNodeVisitorRay2::e_dir_ang2_90, graph->m_rows, graph->m_cols); !ray_vis.end()&&!ray_vis2.end(); ++ray_vis,++ray_vis2)
				if( graph->get_node(ray_vis.m_ptCur)->isEnable() && !graph->get_node(ray_vis2.m_ptCur)->isEnable() )
			{
				GraphNode* node_src = graph->get_node(ray_vis.m_ptCur);
				GraphNode* node_dst = graph->get_node(ray_vis2.m_ptCur);
				node_dst->assign_value(*node_src);
				node_src->setEnable(false);
				node_dst->setEnable(true);
				mv_merger.add_move(ray_vis.m_ptCur, ray_vis2.m_ptCur);
			}
		}

		++src;
		++dst;
	}
	
	std::sort(move_solution.move_seq.begin(), move_solution.move_seq.end(), cmp_Move_inc);
	return 1;
}

static void rand_category(GraphMat* graph, Point2i ptSrc)
{
	static int seed = 0;
	if( seed == 0 ) {
		seed = time(NULL);
		srand(seed);
	}

	GraphNode* pNode = graph->get_node(ptSrc);
	assert( pNode && !pNode->isEnable() );
	{
		NodeCategory categ = NodeCategory::make( (enNodeColor)(rand()%graph->m_max_category_num), enBeadCategory::CHESS_CATEGORY_NORM, -1);
		graph->create_node(ptSrc, categ);
	}
}
int GraphPaddSolver_random::get_padd_solution(GraphMat* graph, const GraphMoveElemsProxy& move_proxy, GraphPaddElemsProxy& padd_solution)
{
	assert( padd_solution.padd_category_seq.empty() && padd_solution.padd_pos_seq.empty() );

	//const std::vector<Point2i>&  padd_pos_seq = move_proxy.padd_pos_seq;
	std::vector<Point2i>  padd_pos_seq;
	padd_pos_seq.assign(move_proxy.padd_pos_seq.begin(), move_proxy.padd_pos_seq.end());
	std::sort(padd_pos_seq.begin(), padd_pos_seq.end(), cmp_Point_dec);
	for(size_t i=0; i<padd_pos_seq.size(); ++i) {
		Point2i curpt = padd_pos_seq[i];

		rand_category(graph, curpt);

		GraphAttrib attrib = graph->get_node(curpt)->getAttrib();
		padd_solution.padd_category_seq.push_back(attrib);
	}
	padd_solution.padd_pos_seq.swap(padd_pos_seq);
	return 1;
}
int GraphPaddSolver_none::get_padd_solution(GraphMat* graph, const GraphMoveElemsProxy& move_proxy, GraphPaddElemsProxy& padd_solution)
{
	assert( padd_solution.padd_category_seq.empty() && padd_solution.padd_pos_seq.empty() );
	return 1;
}

#include "AppMacros.h"
int GraphFlyInOnEnterSolver_popstar::get_flyin_seq(GraphMat* graph, GrapElemsFlyInProxy& flyin_seq)
{
	Point2i pt_cur;
	cocos2d::Size  sz_token = graph->m_coordCvt.getSize(); 
	float fspeed = sz_token.height * 45.0f;
	cocos2d::Point pt_iter = cocos2d::Point(0.0f, designResolutionSize.height);
	for(pt_cur.y=0; pt_cur.y<graph->m_rows; ++pt_cur.y)
	{
		cocos2d::Point pt_iter2 = pt_iter;
		for(pt_cur.x=0; pt_cur.x<graph->m_cols; ++pt_cur.x)
		{
			flyin_seq.glpos_end[pt_cur.y][pt_cur.x]   = graph->m_coordCvt.getCoord(pt_cur);
			flyin_seq.glpos_start[pt_cur.y][pt_cur.x] = pt_iter2;
			flyin_seq.glpos_start[pt_cur.y][pt_cur.x].x = flyin_seq.glpos_end[pt_cur.y][pt_cur.x].x;
			flyin_seq.cost_time[pt_cur.y][pt_cur.x] = (pt_iter2.y-flyin_seq.glpos_end[pt_cur.y][pt_cur.x].y)/fspeed;

			pt_iter2.y += sz_token.height/2;
		}
		pt_iter.y += sz_token.height*3;
	}
	flyin_seq.rows = graph->m_rows;
	flyin_seq.cols = graph->m_cols;

	return 1;
}

int GraphLockChecker_popstar::check_lock(GraphMat* graph)
{
	GraphNodeVisitorNeib1 vis_neib;
	GraphNodeVisitorGraph vis_grp;
	for(vis_grp.begin(graph->m_rows,graph->m_cols); !vis_grp.end(); ++vis_grp)
		if( graph->check_bdy_enable(vis_grp.m_ptCur) )
		{
			NodeCategory categ = graph->get_node(vis_grp.m_ptCur)->get_category();
			for (vis_neib.begin(vis_grp.m_ptCur); !vis_neib.end(); ++vis_neib)
				if( graph->check_bdy_enable(vis_neib.m_ptCur) )
				{
					GraphNode* pnode_bdy = graph->get_node(vis_neib.m_ptCur);
					if( pnode_bdy->isEnable() && graph->m_distance.distance(categ,pnode_bdy->get_category())==0 )
						return 0;
				}
		}
	return 1;
}

struct ptSeqGrp {
	ptSeqGrp(std::vector<Point2i>& __pt_seq):pt_seq(__pt_seq){}
	void addpt(Point2i pt) {
		if( !bit_vis.get(pt) )
			pt_seq.push_back(pt);
	}
	void addpts(const std::vector<Point2i>& __pt_seq) {
		for(size_t i=0; i<__pt_seq.size(); ++i)
			addpt(__pt_seq[i]);
	}
private:
	struct_vis bit_vis;
	std::vector<Point2i>& pt_seq;
};
/************************************************************************/
/*  GraphMoveElemsProxy                                                                    */
/************************************************************************/
void GraphMoveElemsProxy::clear() 
{
	move_seq.clear();
	padd_pos_seq.clear();
}

/************************************************************************/
/* GraphPaddElemsProxy                                                                     */
/************************************************************************/
void GraphPaddElemsProxy::clear() 
{
	padd_pos_seq.clear();
	padd_category_seq.clear();
}

/************************************************************************/
/*  GraphTriggerElemsProxy                                                                    */
/************************************************************************/
void GraphTriggerElemsProxy::clear() {
	trigger_elems_seq.clear();
}

/************************************************************************/
/*  GraphEraseElemsProxy                                                                    */
/************************************************************************/
bool GraphEraseElemsProxy::empty() const
{
	return erase_elems_seq.empty()&&erase_ext_elems_seq.empty();
}
void GraphEraseElemsProxy::clear() 
{
	erase_elm_colrs.clear();
	erase_elems_seq.clear();
	erase_ext_elems_seq.clear();
	change_elems_seq.clear();
	nStarNum = 0;
}
void GraphEraseElemsProxy::makeSolution(std::vector<Point2i>& pt_seq, std::vector<enNodeColor>& colorSeq) 
{
	erase_elems_seq.swap(pt_seq);
	nStarNum = erase_elems_seq.size();
	erase_elm_colrs.swap(colorSeq);
}

/************************************************************************/
/*Graph_proxy                                                                      */
/************************************************************************/
void Graph_proxy::clear() 
{
	trigger.clear();
	erase_solution.clear();
	move_solution.clear();
	padd_solution.clear();
	bOpera_success = 0;
}

/************************************************************************/
/*  IGraphEraseMovePaddSolver                                                                    */
/************************************************************************/
int IGraphEraseMovePaddSolver::get_erase_move_padd_solution(GraphMat* graph, Graph_proxy* m_pProxyes) 
{
	for (auto& iter : m_pProxyes->erase_solution.change_elems_seq)
	{
		graph->get_node(iter.pos)->getAttrib().catgy = iter.categoryB;
	}

	if ( !this->get_move_solution(graph, m_pProxyes->erase_solution, m_pProxyes->move_solution) )
		return 0;
	debug_point2i_seq("move_seq", m_pProxyes->move_solution.move_seq);

	if ( !this->get_padd_solution(graph, m_pProxyes->move_solution, m_pProxyes->padd_solution) )
		return 0;
	debug_point2i_seq("padd_seq", m_pProxyes->padd_solution.padd_pos_seq);

	m_pProxyes->bOpera_success = 1;
	return 1;
}
