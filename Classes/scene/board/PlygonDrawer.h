#pragma once
#include "common/CommonDef.h"
#include "graph/Graph.h"

class TileMapLayer;
class GraphEraseElemsProxy;
class GraphMoveElemsProxy;
class GraphPaddElemsProxy;
class Graph_proxy;
class GrapElemsFlyInProxy;

class ActionWaiter :public cocos2d::FiniteTimeAction {
public:
	void configure(int num, float duration) {
		m_cnt = 0;
		m_needCnt = num;
		_duration = duration;
	}
	void set_callback(const std::function<void()>& moveok) {
		m_callback = moveok;
	}
	void ext_cfg(int num, float duration) {
		m_needCnt += num;
		_duration  = _duration>duration?_duration:duration;
	}
	void actionDoneCb() {
		m_cnt++;
	}
public:
	//virtual ~ActionWaiter() {
	//}
	ActionWaiter()
		:m_cnt(0),m_needCnt(0){ _duration=0.0f; }
	static ActionWaiter* create() {
		ActionWaiter* p = new ActionWaiter;
		p->autorelease();
		return p;
	}
	virtual cocos2d::FiniteTimeAction* reverse() const override {
		return this->clone();
	}
	virtual cocos2d::FiniteTimeAction* clone() const override {
		ActionWaiter* act = new ActionWaiter;
		act->autorelease();
		act->m_cnt = this->m_cnt;
		act->m_needCnt = this->m_needCnt;
		return act;
	}
    virtual bool isDone() const {
		return m_cnt >= m_needCnt;
	}
    //virtual void startWithTarget(Node *target);
    virtual void stop() {
		m_cnt = m_needCnt;

		if(m_callback)
		   m_callback();
	}
	virtual void update(float ft) {
		//cocos2d::FiniteTimeAction::update(ft);
		if( m_cnt >= m_needCnt ) _duration = 0.0f;
	}
private:
	int m_cnt;
	int m_needCnt;
	std::function<void()> m_callback;
};

class ZZTopLayer : public cocos2d::Layer {
public:
	CREATE_FUNC(ZZTopLayer);
	ZZTopLayer();
	void addHighlightRect(const cocos2d::Size& size, cocos2d::Point center, 
							cocos2d::Point ptAnchor = cocos2d::Point(0.5f,0.5f), float rotate = 0.0f);
	void addHighLgtRectSeq(const std::vector<cocos2d::Point>& posSeq, const cocos2d::Size& size, cocos2d::Point ptAnchor = cocos2d::Point(0.5f,0.5f));

	void prepareDraw();
	void clearDraw();
private:
	void reRender();
private:
	cocos2d::Layer*			   m_pLayer;
	cocos2d::CCRenderTexture*  m_pRenderText;
	cocos2d::Vector<cocos2d::Sprite*> m_rects;
};

enum enComboType {
    COMBO_NULL,
    //e_combo_good,
    COMBO_COOL,
    COMBO_AWESOME,
    COMBO_FANTASTIC
};
class PlygonDrawer :public cocos2d::Node {
public:
	void onCmdBegin();
	void do_pushCell(Point2i newPt, Point2i oldPt = makeP2(-1,-1) );
	void do_popCell(Point2i oldPt);

	void draw_with_Graph_proxy(const Graph_proxy& graph_proxy, const std::function<void()>& moveok);
	void do_moveCellAnim(const GrapElemsFlyInProxy& flyin_seq, const std::function<void()>& moveok);
	
	void do_comboAnim(enComboType combo_typ, const std::function<void()>& moveok);
	void do_stateClrAnim(const std::function<void()>& moveok);
	void do_stateOverAnim(const std::vector<Point2i>& ptLeftSeq, const std::function<void()>& animok);
	void do_flySpriteAnim(cocos2d::Sprite* sp, cocos2d::Point pos_begin, cocos2d::Point pos_end, float duration, float fscale, bool removeSelfOnFinish, const std::function<void()>& moveok);

	void do_addHighLight(const std::vector<Point2i>& pt_seq);
	void do_clrHighLight();

	void do_changeCellAnim(Point2i pos, const NodeCategory& categ_old, const NodeCategory& categ_new);
	void do_swapAnim(Point2i ptA, Point2i ptB, const std::function<void()>& moveok);

	void do_rollCardAnim(const Change2i& change, const std::function<void()>& rollok);
public:
	void do_playTips(const std::vector<Point2i>& posSeq);
	void do_clrTips();

	void clrGraph();
	void resetGraph();
protected:
	void do_changeCellType(const GraphEraseElemsProxy& erase_proxy);
	void do_eraseOldCell(const GraphEraseElemsProxy& erase_proxy);
	int  get_moveCellAnim(const GraphMoveElemsProxy& move_proxy, ActionWaiter* pWaiter);
	int  get_paddNewCellAnim(const GraphPaddElemsProxy& padd_proxy, ActionWaiter* pWaiter);
private:
	void playBlastAnim(Point2i pos, enNodeColor color);
    void addBlastParticle(const Point2i& pos);
public:
	PlygonDrawer();
	void attach(GraphMat* graph, TileMapLayer* tilemap);
	void setBoardLocked(bool _lock);
private:
	void onMoveDone(Node*);
public:
	TileMapLayer*					 m_tileMap;
	ZZTopLayer*						 m_zzTopLayer;
	GraphMat*        m_graph;
protected:
	int hitNumber;
	int bgNumber;
};