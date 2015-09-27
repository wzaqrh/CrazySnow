#include "EventSpecial.h"
#include "graph/Graph.h"
#include "scene/board/PlygonDrawer.h"
#include "TilemapEvtHandler.h"
using namespace cocos2d;

//TilEvtHandler_spec
TilEvtHandler_spec::TilEvtHandler_spec() 
	:m_pDrawer(nullptr), m_pDelegate(nullptr), m_pLockChecker(nullptr) 
{
	m_ptSec = m_ptTriger = makeP2(-1, -1);
	m_nStep = Origin;

	m_pLockChecker = new GraphLockChecker_popstar;
}
TilEvtHandler_spec::~TilEvtHandler_spec() {
	delete m_pLockChecker;
}
void TilEvtHandler_spec::create(PlygonDrawer* pDrawer, TilmapEvtDelegate* pTilEvtDelegate) {
	m_pDrawer = pDrawer;
	m_pDelegate = pTilEvtDelegate;
}
bool TilEvtHandler_spec::onTouchBegin(Point2i tilepos, bool& swallow) {
	bool processed = false;
	if (m_nStep == Origin) {
		if (check2tip(tilepos)) {
			m_ptTriger = tilepos;
			m_nStep = ShowTip;
			processed = true; 
		}
	}
	else if (m_nStep == ShowTip) {
		if (check2undo(tilepos, m_ptTriger)) {
			m_nStep = Origin;
			processed = true;
		}
		else if (check2affect(tilepos, m_ptTriger, [this](){
			m_nStep = Origin;
			m_pDrawer->setBoardLocked(false);

			if (m_pLockChecker->check_lock(m_pDrawer->m_graph)) {
				m_pDelegate->onTilmapLocked();
			}
		})) 
		{
			processed = true;
			m_ptSec = tilepos;
			m_nStep = PlayAnim;

			m_pDrawer->setBoardLocked(true);
		}
	}
	return processed;
}
void TilEvtHandler_spec::onTouchEnd(Point2i tilepos) {

}
bool TilEvtHandler_spec::check2undo(const Point2i& ptSecond, const Point2i& ptTriger) {
	m_ptSec = ptSecond;
	auto graph =  m_pDrawer->m_graph;

	auto trig_node = graph->get_node(m_ptSec);
	if ( ! graph->isNodeLocked(m_ptSec)
		&& graph->check_bdy_enable(m_ptTriger)
		&& m_ptSec == m_ptTriger)
	{
		graph->setAllNodeLocked(false);

		//entity->getStateMachine()->lock_event();
		m_pDrawer->do_clrTips();
		m_pDrawer->runAction(CCCallFunc::create([=](){
			//entity->getStateMachine()->transitTo(e_state_type_spec::e_Sta_initial);
			//entity->getStateMachine()->unlock_event();
		}));

		return true;
	}
	return false;
}

//TilEvtHandler_rook
TilEvtHandler_rook::TilEvtHandler_rook() {
	m_pSolverMove = new GraphMoveSolver_down_left;
}
TilEvtHandler_rook::~TilEvtHandler_rook() {
	delete m_pSolverMove;
}
bool TilEvtHandler_rook::check2tip(const Point2i& ptTriger)
{
	auto graph =  m_pDrawer->m_graph;

	auto trig_node = graph->get_node(ptTriger);
	if (graph->check_bdy_enable(ptTriger) 
		&& trig_node->get_category().spec == CHESS_CATEGORY_ARMOR)
	{
		graph->setAllNodeLocked(true);
		vecStk.clear();
		{
			GraphNodeVisitorLine2 visl;
			for (visl.begin(ptTriger, GraphNodeVisitorLine2::e_dir_ang_0, graph->m_rows, graph->m_cols); ! visl.end(); ++visl) {
				vecStk.push_back(visl.m_ptCur);
				graph->setNodeLocked(visl.m_ptCur, false);
			}
			for (visl.begin(ptTriger, GraphNodeVisitorLine2::e_dir_ang_90, graph->m_rows, graph->m_cols); ! visl.end(); ++visl) {
				vecStk.push_back(visl.m_ptCur);
				graph->setNodeLocked(visl.m_ptCur, false);
			}
		}
		vecStk.push_back(ptTriger);
		graph->setNodeLocked(ptTriger, false);

		m_pDrawer->do_playTips(vecStk);

		return true;
	}
	return false;
}
bool TilEvtHandler_rook::check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk)
{
	auto graph =  m_pDrawer->m_graph;

	auto trig_node = graph->get_node(ptSecond);
	if ( ! graph->isNodeLocked(ptSecond)
		&& (ptSecond.x == ptTriger.x || ptSecond.y == ptTriger.y) 
		&& graph->check_bdy_enable(ptTriger))
	{
		graph->setAllNodeLocked(false);

		Point2i ptOff;
		ptOff.x = (ptSecond.x != ptTriger.x) ? (ptSecond.x - ptTriger.x)/abs(ptSecond.x - ptTriger.x) : 0;
		ptOff.y = (ptSecond.y != ptTriger.y) ? (ptSecond.y - ptTriger.y)/abs(ptSecond.y - ptTriger.y) : 0;
		std::vector<Point2i> pt_erase_seq;
		for (Point2i ptCur = ptTriger; ptCur != ptSecond; ptCur.x += ptOff.x, ptCur.y += ptOff.y) {
			pt_erase_seq.push_back(ptCur);
		}
		pt_erase_seq.push_back(ptSecond);

		std::vector<enNodeColor> pt_erase_colrs;
		graph->getColorSeq(pt_erase_seq, pt_erase_colrs);

		Graph_proxy proxy;
		proxy.erase_solution.makeSolution(pt_erase_seq, pt_erase_colrs);
		m_pSolverMove->get_move_solution(graph, proxy.erase_solution, proxy.move_solution);

		m_pDrawer->do_clrTips();
		m_pDrawer->draw_with_Graph_proxy(proxy, onAnimOk);
		vecStk.clear();
		return true;
	}
	return false;
}

//TilEvtHandler_queen
bool TilEvtHandler_queen::check2tip(const Point2i& ptTriger)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptTriger);
	if (   graph->check_bdy_enable(ptTriger) 
		&& trig_node->get_category().spec == CHESS_CATEGORY_QUEEN)
	{
		graph->setAllNodeLocked(true);
		vecStk.clear();
		{
			GraphNodeVisitorLine2 visl;
			for (visl.begin(ptTriger, GraphNodeVisitorLine2::e_dir_ang_0, graph->m_rows, graph->m_cols); ! visl.end(); ++visl) 
				if (graph->check_bdy_enable(visl.m_ptCur)) {
					vecStk.push_back(visl.m_ptCur);
					graph->setNodeLocked(visl.m_ptCur, false);
				}
			for (visl.begin(ptTriger, GraphNodeVisitorLine2::e_dir_ang_90, graph->m_rows, graph->m_cols); ! visl.end(); ++visl) 
				if (graph->check_bdy_enable(visl.m_ptCur)) {
					vecStk.push_back(visl.m_ptCur);
					graph->setNodeLocked(visl.m_ptCur, false);
				}
		}
		{
			GraphNodeVisitorLine3 visl;
			for (visl.begin(ptTriger, GraphNodeVisitorLine3::e_dir_ang_45, graph->m_rows, graph->m_cols); ! visl.end(); ++visl) 
				if (graph->check_bdy_enable(visl.m_ptCur)) {
					vecStk.push_back(visl.m_ptCur);
					graph->setNodeLocked(visl.m_ptCur, false);
				}
			for (visl.begin(ptTriger, GraphNodeVisitorLine3::e_dir_ang_135, graph->m_rows, graph->m_cols); ! visl.end(); ++visl) 
				if (graph->check_bdy_enable(visl.m_ptCur)) {
					vecStk.push_back(visl.m_ptCur);
					graph->setNodeLocked(visl.m_ptCur, false);
				}
		}
		vecStk.push_back(ptTriger);
		graph->setNodeLocked(ptTriger, false);

		m_pDrawer->do_playTips(vecStk);
		return true;
	}
	return false;
}
bool TilEvtHandler_queen::check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptSecond);
	if ( ! graph->isNodeLocked(ptSecond)
		&& std::find(vecStk.begin(), vecStk.end(), ptSecond) != vecStk.end()
		&& graph->check_bdy_enable(ptTriger))
	{
		graph->setAllNodeLocked(false);
		{
			GraphAttrib& attr = graph->get_node(ptTriger)->getAttrib();
			NodeCategory categ_old = attr.catgy;
			attr.catgy = NodeCategory::make(categ_old.color, enBeadCategory::CHESS_CATEGORY_NORM, -1);

			m_pDrawer->do_changeCellAnim(ptTriger, categ_old, attr.catgy);
		}
		graph->swap_node(ptTriger, ptSecond);
		//entity->getStateMachine()->lock_event();
		m_pDrawer->do_clrTips();
		//m_pDrawer->do_swapAnim(ptTriger, ptSecond, [=](){
		//	entity->getStateMachine()->transitTo(e_state_type_spec::e_Sta_initial);
		//	entity->getStateMachine()->unlock_event();
		//});
		m_pDrawer->do_swapAnim(ptTriger, ptSecond, onAnimOk);
		vecStk.clear();
		return true;
	}
	return false;
}

//TilEvtHandler_eleph
bool TilEvtHandler_eleph::check2tip(const Point2i& ptTriger)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptTriger);
	if (   graph->check_bdy_enable(ptTriger) 
		&& trig_node->get_category().spec == CHESS_CATEGORY_ELEPH)
	{
		graph->setAllNodeLocked(true);
		vecStk.clear();
		{
			GraphNodeVisitorLine3 visl;
			for (visl.begin(ptTriger, GraphNodeVisitorLine3::e_dir_ang_45, graph->m_rows, graph->m_cols); ! visl.end(); ++visl) {
				if( graph->check_bdy_enable(visl.m_ptCur) ) {
					vecStk.push_back(visl.m_ptCur);
					graph->setNodeLocked(visl.m_ptCur, false);
				}
			}
			for (visl.begin(ptTriger, GraphNodeVisitorLine3::e_dir_ang_135, graph->m_rows, graph->m_cols); ! visl.end(); ++visl) {
				if( graph->check_bdy_enable(visl.m_ptCur) ) {
					vecStk.push_back(visl.m_ptCur);
					graph->setNodeLocked(visl.m_ptCur, false);
				}
			}
		}
		vecStk.push_back(ptTriger);
		graph->setNodeLocked(ptTriger, false);

		m_pDrawer->do_playTips(vecStk);

		return true;
	}
	return false;
}
bool TilEvtHandler_eleph::check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptSecond);
	if ( ! graph->isNodeLocked(ptSecond)
		&& graph->check_bdy_enable(ptTriger))
	{
		if (std::find(vecStk.begin(), vecStk.end(), ptSecond) != vecStk.end())
		{
			graph->setAllNodeLocked(false);
			{
				GraphAttrib& attr = graph->get_node(ptTriger)->getAttrib();
				NodeCategory categ_old = attr.catgy;
				attr.catgy = NodeCategory::make(categ_old.color, enBeadCategory::CHESS_CATEGORY_NORM, -1);

				m_pDrawer->do_changeCellAnim(ptTriger, categ_old, attr.catgy);
			}
			graph->swap_node(ptTriger, ptSecond);
			
			//entity->getStateMachine()->lock_event();
			m_pDrawer->do_clrTips();
			//m_pDrawer->do_swapAnim(ptTriger, ptSecond, [=](){
			//	entity->getStateMachine()->transitTo(e_state_type_spec::e_Sta_initial);
			//	entity->getStateMachine()->unlock_event();
			//});
			m_pDrawer->do_swapAnim(ptTriger, ptSecond, onAnimOk);
			vecStk.clear();
			return true;
		}
	}
	return false;
}

//TilEvtHandler_artillery
bool TilEvtHandler_artillery::check2tip(const Point2i& ptTriger)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptTriger);
	if (   graph->check_bdy_enable(ptTriger)
		&& trig_node->get_category().spec == CHESS_CATEGORY_ARTILLERY)
	{
		graph->setAllNodeLocked(true);
		vecStk.clear();
		{
			static GraphNodeVisitorRay2::en_direct e_dirs[4] = {
				GraphNodeVisitorRay2::e_dir_ang2_0, GraphNodeVisitorRay2::e_dir_ang2_90,
				GraphNodeVisitorRay2::e_dir_ang2_180, GraphNodeVisitorRay2::e_dir_ang2_270,
			};
			GraphNodeVisitorRay2 visl;
			for (int dr=0; dr<4; ++dr)
			{
				visl.begin(ptTriger, e_dirs[dr], graph->m_rows, graph->m_cols);
				for (++visl; ! visl.end(); ++visl,++visl) {
					if( graph->check_bdy_enable(visl.m_ptCur) ) {
						vecStk.push_back(visl.m_ptCur);
						graph->setNodeLocked(visl.m_ptCur, false);
					}
				}
			}
		}
		vecStk.push_back(ptTriger);
		graph->setNodeLocked(ptTriger, false);

		m_pDrawer->do_playTips(vecStk);

		return true;
	}
	return false;
}
bool TilEvtHandler_artillery::check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptSecond);
	if ( ! graph->isNodeLocked(ptSecond)
		&& std::find(vecStk.begin(), vecStk.end(), ptSecond) != vecStk.end()
		&& graph->check_bdy_enable(ptTriger))
	{
		graph->setAllNodeLocked(false);
		{
			GraphAttrib& attr = graph->get_node(ptTriger)->getAttrib();
			NodeCategory categ_old = attr.catgy;
			attr.catgy = NodeCategory::make(categ_old.color, enBeadCategory::CHESS_CATEGORY_NORM, -1);

			m_pDrawer->do_changeCellAnim(ptTriger, categ_old, attr.catgy);
		}
		graph->swap_node(ptTriger, ptSecond);
		
		//entity->getStateMachine()->lock_event();
		m_pDrawer->do_clrTips();
		//m_pDrawer->do_swapAnim(m_ptTriger, m_ptSec, [=](){
		//	entity->getStateMachine()->transitTo(e_state_type_spec::e_Sta_initial);
		//	entity->getStateMachine()->unlock_event();
		//});
		m_pDrawer->do_swapAnim(ptTriger, ptSecond, onAnimOk);
		vecStk.clear();
		return true;
	}
	return false;
}

//TilEvtHandler_soldier
TilEvtHandler_soldier::TilEvtHandler_soldier() {
	m_pSolverMove = new GraphMoveSolver_down_left;
}
TilEvtHandler_soldier::~TilEvtHandler_soldier() {
	delete m_pSolverMove;
}
bool TilEvtHandler_soldier::check2tip(const Point2i& ptTriger)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptTriger);
	if (graph->check_bdy_enable(ptTriger) 
		&& trig_node->get_category().spec == CHESS_CATEGORY_SDOLIER)
	{
		graph->setAllNodeLocked(true);
		vecStk.clear();
		{
			static GraphNodeVisitorRay2::en_direct e_dirs[4] = {
				GraphNodeVisitorRay2::e_dir_ang2_0, GraphNodeVisitorRay2::e_dir_ang2_90,
				GraphNodeVisitorRay2::e_dir_ang2_180, GraphNodeVisitorRay2::e_dir_ang2_270,
			};
			GraphNodeVisitorRay2 visl;
			for (int dr=0; dr<4; ++dr)
			{
				visl.begin(ptTriger, e_dirs[dr], graph->m_rows, graph->m_cols);
				if (! visl.end()) {
					vecStk.push_back(visl.m_ptCur);
					graph->setNodeLocked(visl.m_ptCur, false);
				}
			}
		}
		vecStk.push_back(ptTriger);
		graph->setNodeLocked(ptTriger, false);

		m_pDrawer->do_playTips(vecStk);
		return true;
	}
	return false;
}
bool TilEvtHandler_soldier::check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptSecond);
	if ( ! graph->isNodeLocked(ptSecond)
		&& (ptSecond.x == ptTriger.x || ptSecond.y == ptTriger.y) 
		&& graph->check_bdy_enable(ptTriger))
	{
		graph->setAllNodeLocked(false);

		Point2i ptOff;
		ptOff.x = (ptSecond.x != ptTriger.x) ? (ptSecond.x - ptTriger.x)/abs(ptSecond.x - ptTriger.x) : 0;
		ptOff.y = (ptSecond.y != ptTriger.y) ? (ptSecond.y - ptTriger.y)/abs(ptSecond.y - ptTriger.y) : 0;
		std::vector<Point2i> pt_erase_seq;
		for (Point2i ptCur = ptTriger; ptCur != ptSecond; ptCur.x += ptOff.x, ptCur.y += ptOff.y) {
			if( graph->check_bdy_enable(ptCur) ) {
				pt_erase_seq.push_back(ptCur);
			}
		}
		pt_erase_seq.push_back(ptSecond);

		std::vector<enNodeColor> pt_erase_colrs;
		graph->getColorSeq(pt_erase_seq, pt_erase_colrs);

		Graph_proxy proxy;
		proxy.erase_solution.makeSolution(pt_erase_seq, pt_erase_colrs);
		m_pSolverMove->get_move_solution(graph, proxy.erase_solution, proxy.move_solution);
		
		m_pDrawer->do_clrTips();
		m_pDrawer->draw_with_Graph_proxy(proxy, onAnimOk);
		vecStk.clear();
		return true;
	}
	return false;
}

//TilEvtHandler_horse
TilEvtHandler_horse::TilEvtHandler_horse() {
	m_pSolverMove = new GraphMoveSolver_down_left;
}
TilEvtHandler_horse::~TilEvtHandler_horse() {
	delete m_pSolverMove;
}
bool TilEvtHandler_horse::check2tip(const Point2i& ptTriger)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptTriger);
	if (graph->check_bdy_enable(ptTriger) 
		&& trig_node->get_category().spec == CHESS_CATEGORY_HORSE)
	{
		graph->setAllNodeLocked(true);
		vecStk.clear();
		{
			static Point2i e_feets[8] = {
				{ 1, 2},{-1, 2},{ 1,-2},{-1,-2},
				{ 2, 1},{-2, 1},{ 2,-1},{-2,-1}
			};
			for (int dr = 0; dr < 8; ++dr) {
				Point2i ptCur = makeP2(ptTriger.x + e_feets[dr].x, ptTriger.y + e_feets[dr].y);
				if (graph->check_bdy_enable(ptCur)) {
					vecStk.push_back(ptCur);
					graph->setNodeLocked(ptCur, false);
				}
			}
		}
		vecStk.push_back(ptTriger);
		graph->setNodeLocked(ptTriger, false);
		m_pDrawer->do_playTips(vecStk);
		return true;
	}
	return false;
}
bool TilEvtHandler_horse::check2affect(const Point2i& ptSecond, const Point2i& ptTriger, std::function<void()> onAnimOk)
{
	auto graph =  m_pDrawer->m_graph;
	auto trig_node = graph->get_node(ptSecond);
	if ( ! graph->isNodeLocked(ptSecond)
		&& std::find(vecStk.begin(), vecStk.end(), ptSecond) != vecStk.end()
		&& graph->check_bdy_enable(ptTriger))
	{
		graph->setAllNodeLocked(false);

		std::vector<Point2i> pt_erase_seq;
		pt_erase_seq.push_back(ptTriger);
		pt_erase_seq.push_back(ptSecond);

		std::vector<enNodeColor> pt_erase_colrs;
		graph->getColorSeq(pt_erase_seq, pt_erase_colrs);

		Graph_proxy proxy;
		proxy.erase_solution.makeSolution(pt_erase_seq, pt_erase_colrs);
		m_pSolverMove->get_move_solution(graph, proxy.erase_solution, proxy.move_solution);
		
		m_pDrawer->do_clrTips();
		m_pDrawer->draw_with_Graph_proxy(proxy, onAnimOk);
		vecStk.clear();
		return true;
	}
	return false;
}