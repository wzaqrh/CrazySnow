#include "EventPopstar.h"
#include "scene/board/PlygonDrawer.h"
#include "graph/IElemPaddingStragy.h"
#include "graph/GraphCommDebug.h"
#include "graph/GraphMatrix.h"
#include "common/SoundPool.h"
using namespace cocos2d;

TilEvtHandlerPopStar::TilEvtHandlerPopStar() 
	:m_pDrawer(nullptr), m_pProxyes(nullptr), m_pSolver(nullptr), m_pDelegate(nullptr) ,m_pLockChecker(nullptr)
{
	m_ptSecond = m_ptTriger = makeP2(-1, -1);
	m_nStep = Origin;
	m_pProxyes = new Graph_proxy;
	m_pSolver  = new GraphSolver_popstar;
	m_pLockChecker = new GraphLockChecker_popstar;
}
TilEvtHandlerPopStar::~TilEvtHandlerPopStar() {
	delete m_pProxyes;
	delete m_pSolver;
	delete m_pLockChecker;
}
void TilEvtHandlerPopStar::create(PlygonDrawer* pDrawer, TilmapEvtDelegate* pDelegate) {
	m_pDrawer   = pDrawer;
	m_pDelegate = pDelegate;
}
bool TilEvtHandlerPopStar::onTouchBegin(Point2i tilepos, bool& swallow)
{
	auto graph = m_pDrawer->m_graph;
    if (graph->get_node(tilepos)->get_category().spec != enBeadCategory::CHESS_CATEGORY_NORM) {
        return false;
    }
    procTouchBegin(tilepos);
    return true;
}
void TilEvtHandlerPopStar::onTouchEnd(Point2i tilepos) {
	if (m_nStep == HighLight) {
		auto iter = std::find(m_ptHighlgtSeq.begin(), m_ptHighlgtSeq.end(), tilepos);
		if (iter != m_ptHighlgtSeq.end() && doPopStarAnim([=](){
			m_nStep = Origin;
			m_pDrawer->setBoardLocked(false);

			if (m_pLockChecker->check_lock(m_pDrawer->m_graph)) {
				m_pDelegate->onTilmapLocked();
			}
		})) 
		{
			m_ptSecond = tilepos;
			m_nStep = PlayAnim;
			m_pDrawer->setBoardLocked(true);
		}
		else {
			CCLOG("failed");
			onTouchEnd(tilepos);
		}
	} 
}
void TilEvtHandlerPopStar::procTouchBegin(Point2i tilepos) {
	if (m_nStep == HighLight) {
		if (check2HighLight(tilepos)) {
			m_ptTriger = tilepos;
			m_nStep = HighLight;
		}
	}
	else if (m_nStep == Origin) {
		if (check2HighLight(tilepos)) {
			m_ptTriger = tilepos;
			m_nStep = HighLight;
		}
	}
}
bool TilEvtHandlerPopStar::check2HighLight(Point2i triggerPt)
{
	Graph_proxy* pProxyes = m_pProxyes;
	pProxyes->clear();
	pProxyes->trigger.trigger_elems_seq.assign(1, triggerPt);
	debug_point2i_seq("trigger seq", pProxyes->trigger.trigger_elems_seq);

	GraphEraseCfg cfg = { m_pDelegate->nextBonus().first, m_pDelegate->nextBonus().second };
	m_pSolver->configure(cfg);
	
	auto m_graph = m_pDrawer->m_graph;
	pProxyes->bOpera_success = m_pSolver->get_erase_solution(m_graph, pProxyes->trigger, pProxyes->erase_solution);
	if( pProxyes->bOpera_success ) {
		m_ptHighlgtSeq.swap(pProxyes->erase_solution.erase_elems_seq);
		for (auto& iter : pProxyes->erase_solution.change_elems_seq) {
			m_ptHighlgtSeq.push_back(iter.pos);
		}

		m_pDrawer->do_clrHighLight();
		m_pDrawer->do_addHighLight(m_ptHighlgtSeq);
		//{
		//	int nStar = m_ptHighlgtSeq.size();
		//	g_glbInfo()->notify_tip(nStar, get_mark(nStar));
		//}
		{
			std::vector<NodeCategory> categSeq;
			for (int i = 0; i < m_ptHighlgtSeq.size(); ++i) {
				categSeq.push_back( m_graph->get_nodeCatg(m_ptHighlgtSeq[i]) );
			}
			m_pDelegate->popsNewStars(categSeq, true);
		}
        
        SoundPool::Inst()->playEffect(SOUND_EFFECT_HIT);
        
		return true;
	}
	return false;
}
bool TilEvtHandlerPopStar::doPopStarAnim(std::function<void()> onAnimOk) {
	
	std::vector<NodeCategory> categSeq;
	for (int i = 0; i < m_ptHighlgtSeq.size(); ++i) {
		categSeq.push_back( m_pDrawer->m_graph->get_nodeCatg(m_ptHighlgtSeq[i]) );
	}
	
	Graph_proxy* pProxyes = m_pProxyes;
	pProxyes->clear();
	pProxyes->trigger.trigger_elems_seq.assign(1, m_ptTriger);
	debug_point2i_seq("trigger seq", pProxyes->trigger.trigger_elems_seq);

	GraphEraseCfg cfg = { m_pDelegate->nextBonus().first, m_pDelegate->nextBonus().second };
	m_pSolver->configure(cfg);

	m_pSolver->get_erase_solution(m_pDrawer->m_graph, pProxyes->trigger, pProxyes->erase_solution);

	m_pDrawer->clrGraph();
	m_pSolver->get_erase_move_padd_solution(m_pDrawer->m_graph, pProxyes);
	if (pProxyes->bOpera_success) {
		m_pDrawer->draw_with_Graph_proxy(*pProxyes, [=](){
			m_pDrawer->do_clrHighLight();
			onAnimOk();
		});

		m_pDelegate->popsNewStars(categSeq, false);
		return true;
	}
	return false;
}