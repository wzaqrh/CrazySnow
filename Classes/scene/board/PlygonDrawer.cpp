#include "PlygonDrawer.h"
#include "TileMap.h"
#include "graph/GraphMatrix.h"
#include "graph/IElemPaddingStragy.h"
using namespace cocos2d;


PlygonDrawer::PlygonDrawer()
	:m_graph(NULL)
	,m_tileMap(NULL)
	,m_zzTopLayer(NULL)
	,hitNumber(1)
	,bgNumber(1)
{
	m_zzTopLayer = ZZTopLayer::create();
	this->addChild(m_zzTopLayer);
}

void PlygonDrawer::attach(GraphMat* graph, TileMapLayer* tilemap)
{
	m_graph   = graph;
	m_tileMap = tilemap;
}
void PlygonDrawer::onCmdBegin()
{
	bgNumber = 1;
	hitNumber = 1;
}
void PlygonDrawer::do_pushCell(Point2i newPt, Point2i oldPt)
{
	Node* spNew = m_tileMap->cellAt(newPt);
	spNew->setScale(1.1f) ;
	spNew->runAction( CCRotateBy::create(0.3f, 360.0f) );
}
void PlygonDrawer::do_popCell(Point2i oldPt)
{
}

int PlygonDrawer::get_moveCellAnim(const GraphMoveElemsProxy& move_proxy, ActionWaiter* pWaiter)
{
	for(int i=0; i<move_proxy.move_seq.size(); ++i) {
		Point2i src = move_proxy.move_seq[i].src;
		Point2i dst = move_proxy.move_seq[i].dst;

		CoordConvert& convert = m_graph->m_coordCvt;
		CCPoint coord_dst = convert.getCoord(dst);

		assert( m_tileMap->cellAt(src) && !m_tileMap->cellAt(dst) );
		m_tileMap->swapCellRef(src, dst);
		m_tileMap->cellAt(dst)->runAction( Sequence::createWithTwoActions(
												MoveTo::create(0.5f,coord_dst),
												CCCallFunc::create(CC_CALLBACK_0(ActionWaiter::actionDoneCb,pWaiter))
												));
		pWaiter->ext_cfg(1, 0.5f);	
	}
	return move_proxy.move_seq.size();
}
void PlygonDrawer::do_changeCellAnim(Point2i pos, const NodeCategory& categ_old, const NodeCategory& categ_new)
{
	m_tileMap->changeCell(pos, &categ_new);
}
void PlygonDrawer::do_changeCellType(const GraphEraseElemsProxy& erase_proxy)
{
	for(int i=0; i<erase_proxy.change_elems_seq.size(); ++i) {
		const Change2i& chg = erase_proxy.change_elems_seq[i];
		m_tileMap->changeCell(chg.pos, &chg.categoryB);
		CCLOG("change_cell (%d,%d) color %d-%d", chg.pos.x, chg.pos.y, chg.categoryA.color, chg.categoryB.color);
	}
}
void PlygonDrawer::do_eraseOldCell(const GraphEraseElemsProxy& erase_proxy)
{
	for(int i=0; i<erase_proxy.erase_elems_seq.size(); ++i) {
		m_tileMap->removeCell( erase_proxy.erase_elems_seq[i] );
		Point2i pos = erase_proxy.erase_elems_seq[i];
		//playBlastAnim(pos, erase_proxy.erase_elm_colrs[i]);
	}
}
void PlygonDrawer::playBlastAnim(Point2i pos, enNodeColor color) 
{
	auto convt = m_graph->m_coordCvt;
	Point coord = convt.getCoord(pos);

	auto scene = Director::getInstance()->getRunningScene();
	PhysicsWorld* phyWld = scene->getPhysicsWorld();

	for (int i = 0; i < 5; ++i)
	{
		Sprite* spStar = BeadFactory::createBlastStar(color, 0.2 + 0.1 * i);
		float radius = spStar->getContentSize().width/2 * spStar->getScaleX();
		spStar->retain();
		spStar->setPosition(coord);
		/*spStar->runAction( Sequence::createWithTwoActions(MoveBy::create(2.0f, dir),
				CCCallFunc::create([=]() {
				spStar->removeFromParent();
				spStar->release();
		})));*/
		spStar->runAction( Sequence::createWithTwoActions(DelayTime::create(15.0f),
			CCCallFunc::create([=]() {
				spStar->removeFromParent();
				spStar->release();
		})));
#define GravityAmpl 50
		PhysicsMaterial phyMat = PHYSICSBODY_MATERIAL_DEFAULT; phyMat.density = 200.0f;
		auto body = PhysicsBody::createCircle(radius, phyMat);
		Point dir = ccp(rand()%100 - 50, rand()%100 - 50);
		dir.normalize(); dir = dir * body->getMass() * 8.0f * GravityAmpl;
		Vect force(dir);
		body->applyImpulse(force);
		body->setDynamic(true);
		//body->setCategoryBitmask(0x04);
		//body->setCollisionBitmask(0x08);
		body->setGroup(-1);
		body->setGravityEnable(true);

		spStar->setPhysicsBody(body);
		m_tileMap->addChild(spStar, 100);
	}

	static bool isPhyInit = false;
	if (! isPhyInit) {
		isPhyInit = true;
		phyWld->setGravity(Vect(0.0f, -9.8f * GravityAmpl));
		//phyWld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		phyWld->setUpdateRate(2.5f);
	}
}
int PlygonDrawer::get_paddNewCellAnim(const GraphPaddElemsProxy& padd_proxy, ActionWaiter* pWaiter)
{
	assert( padd_proxy.padd_category_seq.size() == padd_proxy.padd_pos_seq.size() );
	for(int i=0; i<padd_proxy.padd_pos_seq.size(); ++i) {
		GraphAttrib attr = padd_proxy.padd_category_seq[i];
		Point2i     pos  = padd_proxy.padd_pos_seq[i];
		m_tileMap->addCell(pos, &attr.catgy);

		CCPoint ptOrg = m_tileMap->cellAt(pos)->getPosition();
		CCPoint ptDec = ccp(-75, ptOrg.y);
		m_tileMap->cellAt(pos)->setPosition(ptDec);
		//m_tileMap->cellAt(pos)->runAction( CCMoveTo::create(0.5f,ptOrg) );
		m_tileMap->cellAt(pos)->runAction( Sequence::createWithTwoActions(
												CCMoveTo::create(0.5f,ptOrg),
												CCCallFunc::create(CC_CALLBACK_0(ActionWaiter::actionDoneCb,pWaiter))
												));
		pWaiter->ext_cfg(1, 0.5f);
	}
	return padd_proxy.padd_pos_seq.size();
}
void PlygonDrawer::do_moveCellAnim(const GrapElemsFlyInProxy& flyin_seq, const std::function<void()>& moveok)
{
	ActionWaiter* pWaiter = ActionWaiter::create();
	pWaiter->configure(0,0.0f);
	{
		int num = 0;
		Point2i ptIter;
		for (ptIter.y=0; ptIter.y<flyin_seq.rows; ++ptIter.y)
			for(ptIter.x=0; ptIter.x<flyin_seq.cols; ++ptIter.x)
				if (m_graph->check_bdy_enable(ptIter)) {
					m_tileMap->cellAt(ptIter)->setPosition(flyin_seq.glpos_start[ptIter.y][ptIter.x]);
					m_tileMap->cellAt(ptIter)->runAction( Sequence::createWithTwoActions(
						CCMoveTo::create(flyin_seq.cost_time[ptIter.y][ptIter.x],flyin_seq.glpos_end[ptIter.y][ptIter.x]),
						CCCallFunc::create(CC_CALLBACK_0(ActionWaiter::actionDoneCb,pWaiter))
						));
					pWaiter->ext_cfg(1,flyin_seq.cost_time[ptIter.y][ptIter.x]+0.1f);
				}
	}
	FiniteTimeAction* pSpawn  = pWaiter;
	CCCallFunc*      pFuncCb  = CCCallFunc::create(moveok);
	this->runAction( Sequence::createWithTwoActions(pSpawn,pFuncCb) );
}
void PlygonDrawer::do_flySpriteAnim(cocos2d::Sprite* sp, cocos2d::Point pos_begin, cocos2d::Point pos_end, float duration, float fscale, const std::function<void()>& moveok)
{
	sp->setPosition(pos_begin);
	m_tileMap->addChild(sp, 1000);

	sp->runAction( Sequence::createWithTwoActions( 
						Spawn::createWithTwoActions(
							EaseIn::create(MoveTo::create(duration, pos_end), 2.0f),
							ScaleTo::create(duration, fscale)
						),
						CCCallFunc::create(moveok)
					));
}
cocos2d::Sprite* get_combo_sp(PlygonDrawer::en_combo_typ combo_typ)
{
	Sprite* local_sp = NULL;
	switch (combo_typ)
	{
	//case PlygonDrawer::e_combo_good:
	//	local_sp = Sprite::create("zzImage/combo/combo_good.png");
	//	break;
	case PlygonDrawer::e_combo_cool:
		local_sp = Sprite::create("zzImage/combo/combo_cool.png");
		break;
	case PlygonDrawer::e_combo_awesome:
		local_sp = Sprite::create("zzImage/combo/combo_awesome.png");
		break;
	case PlygonDrawer::e_combo_fantastic:
		local_sp = Sprite::create("zzImage/combo/combo_fantastic.png");
		break;
	default:
		break;
	}
	return local_sp;
}
void PlygonDrawer::do_comboAnim(en_combo_typ combo_typ, const std::function<void()>& moveok)
{
	Sprite* sp = get_combo_sp(combo_typ);
	Size szWin = Director::getInstance()->getWinSize();
	this->do_flySpriteAnim(sp, Point(szWin.width/2,szWin.height/2), Point(szWin.width/2,szWin.height/2+200), 1.4f, 0.4f, [=]{
		sp->removeFromParentAndCleanup(true);
		moveok();
	});
}
void PlygonDrawer::do_stateClrAnim( const std::function<void()>& moveok)
{
	Sprite* sp = Sprite::create("zzImage/stage_clear.png");
	Size szWin = Director::getInstance()->getWinSize();
	this->do_flySpriteAnim(sp, Point(szWin.width/2,szWin.height/2), Point(640-80,688), 1.5f, 0.35f, moveok);
}
void PlygonDrawer::draw_with_Graph_proxy(const Graph_proxy& graph_proxy, const std::function<void()>& moveok)
{
	ActionWaiter* pActWaiter = ActionWaiter::create();
	pActWaiter->configure(0, 0.0f);

	this->do_changeCellType(graph_proxy.erase_solution);
	this->do_eraseOldCell(graph_proxy.erase_solution);
	this->get_moveCellAnim(graph_proxy.move_solution, pActWaiter);
	this->get_paddNewCellAnim(graph_proxy.padd_solution, pActWaiter);
	
	FiniteTimeAction* pSpawn  = pActWaiter;
	CCCallFunc*      pFuncCb = CCCallFunc::create(moveok);
	this->runAction( Sequence::createWithTwoActions(pSpawn,pFuncCb) );
}
void PlygonDrawer::do_addHighLight(const std::vector<Point2i>& pt_seq)
{
	for (auto& iter : pt_seq)
	{
		m_tileMap->addCell(iter, NULL, TileMapLayer::e_cell_bg);
	}
}
void PlygonDrawer::do_clrHighLight()
{
	m_tileMap->removeCellAll(TileMapLayer::e_cell_bg);
}
#define kRadius		  1
#define kDeltaRadius  0

#define kAngleX       0
#define kDeltaAngleX  0

#define kInAngleZ        270 //里面卡牌的起始Z轴角度
#define kInDeltaZ        90  //里面卡牌旋转的Z轴角度差

#define kOutAngleZ       0   //封面卡牌的起始Z轴角度
#define kOutDeltaZ       90  //封面卡牌旋转的Z轴角度差
void  PlygonDrawer::do_rollCardAnim(const Change2i& change, const std::function<void()>& rollok)
{
	CCLOG("do_rollCardAnim (%d,%d) color%d-%d", change.pos.x, change.pos.y, change.categoryA, change.categoryB);
	
	m_tileMap->addCell(change.pos, &change.categoryB, TileMapLayer::e_cell_oppos);
	Node* sp_corrt = m_tileMap->cellAt(change.pos, TileMapLayer::e_cell_corrt);
	Node* sp_oppos = m_tileMap->cellAt(change.pos, TileMapLayer::e_cell_oppos);
	sp_oppos->setVisible(false);
	assert(sp_corrt && sp_oppos);

	static float duration = 1.0f;
	static CCActionInterval* m_openAnimIn = NULL;
	if (m_openAnimIn == NULL) {
		CCOrbitCamera* pRollCard = CCOrbitCamera::create(duration * 0.5f, kRadius, kDeltaRadius, kInAngleZ, kInDeltaZ, kAngleX, kDeltaAngleX);
		m_openAnimIn = (CCActionInterval*)CCSequence::create(
													CCDelayTime::create(duration * 0.5f),
													CCShow::create(),
													pRollCard,
													NULL);
		m_openAnimIn->retain();
	}

	static CCActionInterval* m_openAnimOut = NULL;
	if (m_openAnimOut == NULL) {
		CCOrbitCamera* pRollCard = CCOrbitCamera::create(duration * 0.5f, kRadius, kDeltaRadius, kOutAngleZ, kOutDeltaZ, kAngleX, kDeltaAngleX);
		m_openAnimOut = (CCActionInterval *)CCSequence::create(
														pRollCard,
														CCHide::create(),
														CCDelayTime::create(duration * 0.5f),
														NULL);
		m_openAnimOut->retain();
	}

	{
		ActionWaiter* pWaiter = ActionWaiter::create();
		float fdelta = duration;
		pWaiter->configure(2, fdelta);
		pWaiter->set_callback([=]{
			
			m_tileMap->swapCellRef(change.pos, TileMapLayer::e_cell_corrt, TileMapLayer::e_cell_oppos);
			m_tileMap->removeCell(change.pos, TileMapLayer::e_cell_oppos);
		});

		sp_oppos->runAction(Sequence::createWithTwoActions(
										m_openAnimIn,
										CCCallFunc::create(CC_CALLBACK_0(ActionWaiter::actionDoneCb,pWaiter))
										));
		sp_corrt->runAction(Sequence::createWithTwoActions(
										m_openAnimOut,
										CCCallFunc::create(CC_CALLBACK_0(ActionWaiter::actionDoneCb,pWaiter))
										));
		FiniteTimeAction* pSpawn  = pWaiter;
		CCCallFunc*      pFuncCb  = CCCallFunc::create(rollok);
		this->runAction( Sequence::createWithTwoActions(pSpawn,pFuncCb) );
		/*sp_oppos->runAction(m_openAnimIn);
		sp_corrt->runAction(m_openAnimOut);*/
	}
}
void PlygonDrawer::do_stateOverAnim(const std::vector<Point2i>& ptLeftSeq, const std::function<void()>& animok)
{
	std::vector<enNodeColor> colrSeq;
	m_graph->getColorSeq(ptLeftSeq, colrSeq);
	float ddelay_base = 1.0f;
	float fdelay = 0.1f;
	for(int i = ptLeftSeq.size() -1; i >= 0; --i) {
		Point2i pos = ptLeftSeq[i];
		enNodeColor color = colrSeq[i];
		float t_delay = (ptLeftSeq.size() - i) * fdelay;
		if (i != 0)
		{
			m_tileMap->runAction(Sequence::createWithTwoActions(DelayTime::create(t_delay),
				CCCallFunc::create([=](){
					m_graph->erase_node(pos);
					m_tileMap->removeCell(pos);
					playBlastAnim(pos, color);
			})));
		}
		else
		{
			m_tileMap->runAction(Sequence::createWithTwoActions(DelayTime::create(t_delay),
				CCCallFunc::create([=](){
					m_graph->erase_node(pos);
					m_tileMap->removeCell(pos);
					playBlastAnim(pos, color);
			})));
		}
		//CCLOG("erase_cell (%d,%d)", erase_proxy.erase_elems_seq[i].x, erase_proxy.erase_elems_seq[i].y);
	}

	m_tileMap->runAction(Sequence::createWithTwoActions(DelayTime::create(ddelay_base + fdelay * ptLeftSeq.size()),
									CCCallFunc::create(animok)));
}
void PlygonDrawer::do_swapAnim(Point2i ptA, Point2i ptB, const std::function<void()>& moveok)
{
	Node* spA = m_tileMap->cellAt(ptA);
	Node* spB = m_tileMap->cellAt(ptB);
	m_tileMap->swapCellRef(ptA, ptB);

	ActionWaiter* pWaiter = ActionWaiter::create();
	float fdelta = 0.3f;
	pWaiter->configure(2, fdelta);
	pWaiter->set_callback(moveok);

	spA->runAction(Sequence::createWithTwoActions(
						CCMoveTo::create(fdelta,spB->getPosition()),
						CCCallFunc::create(CC_CALLBACK_0(ActionWaiter::actionDoneCb,pWaiter))
						));
	spB->runAction(Sequence::createWithTwoActions(
						CCMoveTo::create(fdelta,spA->getPosition()),
						CCCallFunc::create(CC_CALLBACK_0(ActionWaiter::actionDoneCb,pWaiter))
						));
	FiniteTimeAction* pSpawn  = pWaiter;
	CCCallFunc*      pFuncCb  = CCCallFunc::create(moveok);
	this->runAction( Sequence::createWithTwoActions(pSpawn,pFuncCb) );
}
void PlygonDrawer::do_clrTips() {
	m_zzTopLayer->clearDraw();
}

void PlygonDrawer::do_playTips(const std::vector<Point2i>& posSeq)
{
	m_zzTopLayer->prepareDraw();

	auto& coodCvt = m_graph->m_coordCvt;

	std::vector<Point> relposSeq;
	for (int i = 0; i < posSeq.size(); i++) {
		Point relpos = coodCvt.getCoord(posSeq[i]);
		relposSeq.push_back(relpos);
	}

	Size szCell = coodCvt.getSize();

	m_zzTopLayer->addHighLgtRectSeq(relposSeq, szCell);
}

void PlygonDrawer::clrGraph() {
	GraphNodeVisitorGraph grp_vis;
	for (grp_vis.begin(m_graph->m_rows, m_graph->m_cols); ! grp_vis.end(); ++grp_vis)
		if (m_graph->check_bdy_enable(grp_vis.m_ptCur))
		{
			GraphNode* pNode = m_graph->get_node(grp_vis.m_ptCur);
			auto categ = pNode->get_category();
			if(categ.spec != enBeadCategory::CHESS_CATEGORY_NORM && categ.spec != enBeadCategory::CHESS_CATEGORY_DOUBLE) {
				auto categ_new  = categ;
				categ_new.spec  = enBeadCategory::CHESS_CATEGORY_NORM;
				if (categ_new.color == enNodeColor::NODE_COLOR_SPEC) {
					categ_new.color = (enNodeColor)(rand()%enNodeColor::NODE_COLOR_SPEC);
				}
				pNode->set_category(categ_new);

				m_tileMap->changeCell(grp_vis.m_ptCur, &categ_new);
			}
		}
}

void PlygonDrawer::resetGraph() {
	GraphNodeVisitorGraph grp_vis;
	for (grp_vis.begin(m_graph->m_rows, m_graph->m_cols); ! grp_vis.end(); ++grp_vis)
		if (m_graph->check_bdy_enable(grp_vis.m_ptCur))
		{
			GraphNode* pNode = m_graph->get_node(grp_vis.m_ptCur);
			auto categ = pNode->get_category();
			categ.spec  = CHESS_CATEGORY_NORM;
			categ.color = (enNodeColor)(rand()%NODE_COLOR_SPEC);
			m_tileMap->changeCell(grp_vis.m_ptCur, &categ);
			pNode->set_category(categ);
		}
}

void PlygonDrawer::setBoardLocked(bool _lock) {
	if (_lock) {
		m_tileMap->lockBoard();
	}
	else {
		m_tileMap->unlockBoard();
	}
}


ZZTopLayer::ZZTopLayer()
	:m_pLayer(NULL),m_pRenderText(NULL){}
void ZZTopLayer::addHighLgtRectSeq(const std::vector<Point>& posSeq, const cocos2d::Size& size, cocos2d::Point ptAnchor)
{
	assert(m_pLayer);
	for (int i=0; i<posSeq.size(); ++i) {
		CCSprite* spMask = CCSprite::create("zzImage/highLightMask.png");
		spMask->setPosition(posSeq[i]);
		spMask->setAnchorPoint(ptAnchor);
		spMask->setScaleX(size.width/spMask->getContentSize().width);
		spMask->setScaleY(size.height/spMask->getContentSize().height);

		m_rects.pushBack(spMask);
	}

	reRender();
}
void ZZTopLayer::addHighlightRect(const cocos2d::Size& size, cocos2d::Point center, cocos2d::Point ptAnchor, float rotate)
{
	assert(m_pLayer);

	CCSprite* spMask = CCSprite::create("zzImage/highLightMask.png");
	spMask->setPosition(center);
	spMask->setAnchorPoint(ptAnchor);
	spMask->setScaleX(size.width/spMask->getContentSize().width);
	spMask->setScaleY(size.height/spMask->getContentSize().height);
	if (rotate != 0.0f) {
		spMask->setRotation(rotate);
	}
	m_rects.pushBack(spMask);

	reRender();
}
void ZZTopLayer::reRender()
{
	if (m_pRenderText == NULL) {
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin     = CCDirector::sharedDirector()->getVisibleOrigin();
		m_pRenderText = CCRenderTexture::create(visibleSize.width, visibleSize.height, kCCTexture2DPixelFormat_RGBA8888);

		m_pRenderText->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
		m_pLayer->addChild(m_pRenderText, 10000);
	}

	m_pRenderText->clear(0,0,0,0.8f);
	m_pRenderText->begin();
	for (int i=0; i<m_rects.size(); ++i)
	{
		ccBlendFunc blend;
		blend.src = GL_ZERO;
		blend.dst = GL_ONE_MINUS_SRC_ALPHA;
		m_rects.at(i)->setBlendFunc(blend);
		m_rects.at(i)->visit();
	}
	m_pRenderText->end();
}
void ZZTopLayer::prepareDraw() {
	clearDraw();
	m_pLayer = Layer::create();
	this->addChild(m_pLayer);
}
void ZZTopLayer::clearDraw() {
	if (m_pLayer) {
		m_pLayer->removeFromParent();
		m_pLayer = NULL;

		m_rects.clear();
		m_pRenderText = NULL;
	}
}
