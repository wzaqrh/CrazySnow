#include "Factory.h"
#include "graph/GraphMatrix.h"
#include "data/GameEntity.h"
#include "graph/RollCardUtinity.h"

#include <iostream>
using namespace cocos2d;

static GraphData s_GraphCfg_round1 = {
	10/*row*/,
	10/*column*/,
	5/*vertex_num*/,
	64/*box_heights*/,
	64/*box_widths*/,
	0/*diff_row_off_x*/,
	32/*offsert_x*/,
	32/*offsert_y*/,
	{
		{{NODE_COLOR_4},{NODE_COLOR_2},{{NODE_COLOR_0}},{{NODE_COLOR_0}},{NODE_COLOR_1},{NODE_COLOR_4},{NODE_COLOR_4},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_3}},
		{{NODE_COLOR_1},{NODE_COLOR_4},{{NODE_COLOR_0}},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_4},{{NODE_COLOR_0}},{NODE_COLOR_1},{NODE_COLOR_3}},
		{{NODE_COLOR_1},{NODE_COLOR_3},{NODE_COLOR_1},{NODE_COLOR_3},{NODE_COLOR_1},{NODE_COLOR_2},{NODE_COLOR_2},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_1}},
		{{{NODE_COLOR_0}},{{NODE_COLOR_0}},{NODE_COLOR_2},{NODE_COLOR_1},{NODE_COLOR_2},{NODE_COLOR_4},{NODE_COLOR_1},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_4}},
		{{{NODE_COLOR_0}},{NODE_COLOR_1},{NODE_COLOR_1},{NODE_COLOR_4},{NODE_COLOR_3},{{NODE_COLOR_0}},{NODE_COLOR_3},{{NODE_COLOR_0}},{NODE_COLOR_2},{{NODE_COLOR_0}}},
		{{NODE_COLOR_3},{NODE_COLOR_4},{NODE_COLOR_4},{{NODE_COLOR_0}},{{NODE_COLOR_0}},{NODE_COLOR_4},{NODE_COLOR_1},{{NODE_COLOR_0}},{NODE_COLOR_2},{{NODE_COLOR_0}}},
		{{NODE_COLOR_4},{NODE_COLOR_4},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_3},{{NODE_COLOR_0}},{NODE_COLOR_2},{NODE_COLOR_2},{{NODE_COLOR_0}}},
		{{{NODE_COLOR_0}},{NODE_COLOR_3},{NODE_COLOR_1},{NODE_COLOR_3},{NODE_COLOR_4},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_2},{NODE_COLOR_4},{NODE_COLOR_1}},
		{{NODE_COLOR_2},{NODE_COLOR_4},{{NODE_COLOR_0}},{{NODE_COLOR_0}},{NODE_COLOR_4},{NODE_COLOR_2},{NODE_COLOR_3},{NODE_COLOR_4},{NODE_COLOR_3},{NODE_COLOR_1}},
		{{NODE_COLOR_1},{{NODE_COLOR_0}},{NODE_COLOR_1},{NODE_COLOR_2},{NODE_COLOR_4},{NODE_COLOR_4},{NODE_COLOR_2},{NODE_COLOR_3},{NODE_COLOR_3},{NODE_COLOR_2}},
	}
};

struct recorder {
	recorder() {
		m_bits.reset();
	}
	int mkCoord(int row, int col) {
		return (row<<4) + col;
	}
	void reset() {
		m_bits.reset();
	}
	void set(const Point2i& pt) {
		this->set(pt.y, pt.x);
	}
	void set(int row, int col) {
		m_bits.set(mkCoord(row, col));
	}
	void reset(const Point2i& pt) {
		this->reset(pt.y, pt.x);
	}
	void reset(int row, int col) {
		m_bits.reset(mkCoord(row, col));
	}
	int get(const Point2i& pt) {
		return this->get(pt.y, pt.x);
	}
	int get(int row, int col) {
		return m_bits.test(mkCoord(row, col));
	}
private:
	std::bitset<1<<8> m_bits;
};

class NodeVisitor_1_ {
public:
	void begin(Point2i ptOrg, int rows, int cols) {
		m_rows = rows;
		m_cols = cols;

		m_ptOrg = ptOrg;
		m_cnt   = 0;
		m_ptCur.x = m_ptOrg.x + get_dir_step()[m_cnt].x;
		m_ptCur.y = m_ptOrg.y + get_dir_step()[m_cnt].y;
	}
	bool end() const {
		return m_cnt >= 4;
	}
	void operator++() {
		m_cnt++;
		m_ptCur.x = m_ptOrg.x + get_dir_step()[m_cnt].x;
		m_ptCur.y = m_ptOrg.y + get_dir_step()[m_cnt].y;
	}
	bool check_bdy(Point2i pt) const {
		if( pt.x<0 || pt.y<0 || pt.y>=m_rows || pt.x>=m_cols )
			return false;
		return true; 
	}
public:
	Point2i m_ptCur;
private:
	Point2i m_ptOrg;
	int     m_rows;
	int     m_cols;

	int     m_cnt;
private:
	typedef Point2i ____dir_step[4];
	static ____dir_step& get_dir_step() {
		static Point2i local_dir_step[4] = {
			{1, 0},{-1, 0}, {0, 1},{0, -1}
		};
		return local_dir_step;
	}
};

struct tagPosMrk {
	Point2i pos;
	int mark;
};
inline bool operator<(const tagPosMrk& p1, const tagPosMrk& p2) {
	return p1.mark < p2.mark;
}
inline bool operator>(const tagPosMrk& p1, const tagPosMrk& p2) {
	return p1.mark > p2.mark;
}
struct GfMap {
	GfMap();
	GfMap(GfMap& other);
	void operator=(const GfMap& other);
public:
	int get(const Point2i& pt) {
		return get(pt.y, pt.x);
	}
	int get(int row, int col) {
		return m_map[row][col];
	}
	void set(int row, int col, int val) {
		m_map[row][col] = val;
	}
	void reset(int row, int col) {
		m_map[row][col] = -1;
	}
	void accept(const GraphData& grphCfg);
	void randMapWithDegree(int nDegree);
private:
	void do_randMap();
	int  normPos(std::vector<Point2i>& posSeq);
	int  randPos(std::vector<Point2i>& posSeq);
	int  calMark(std::vector<Point2i>* posPath = nullptr);
	int  calMark_l(const std::vector<Point2i>& posSeq, std::vector<Point2i>* posPath = nullptr);
	int  eraseElem(int row, int col, bool justEval = false);
	int  evalElem(int row, int col, recorder& rcd);
	void getMaxPos(const std::vector<Point2i>& posSeq, std::vector<tagPosMrk>& maxPosSeq, int maxNum);
	void do_erasePt(Point2i pt);
	void do_addPt(Point2i pt);
private:
	char m_map[10][10];
	std::vector<Point2i>& m_posSeq() {
		static std::vector<Point2i> m_posSeq;
		static bool bInit = false;
		if (! bInit) {
			bInit = true;
			Point2i ptIter;
			for (ptIter.y = 0; ptIter.y < 10; ++ptIter.y) {
				for (ptIter.x = 0; ptIter.x < 10; ++ptIter.x) {
					m_posSeq.push_back(ptIter);
				}
			}
		}
		return m_posSeq;
	}

	friend class GraphDataFactory;
};
GfMap::GfMap() {

}
GfMap::GfMap(GfMap& other) {
	memcpy(m_map, other.m_map, sizeof(m_map));
}
void GfMap::operator=(const GfMap& other) {
	memcpy(m_map, other.m_map, sizeof(m_map));
}
void GfMap::accept(const GraphData& grphCfg)
{
	for (int col = 0; col < 10; ++col) {
		for (int row =0; row < 10; ++row) {
			m_map[row][col] = grphCfg.map[row][col].catgy.color;
		}
	}
}
int GfMap::randPos(std::vector<Point2i>& posSeq) {
	posSeq.assign(m_posSeq().begin(), m_posSeq().end());
	std::random_shuffle(posSeq.begin(), posSeq.end());
	return posSeq.size();
}
int  GfMap::normPos(std::vector<Point2i>& posSeq) {
	static std::vector<Point2i> _normPosSeq;
	static bool isInit = false;
	if (! isInit)
	{
		isInit = true;

		Point2i ptIter;
		for (ptIter.y = 9; ptIter.y >= 0; --ptIter.y) {
			for (ptIter.x = 0; ptIter.x < 10; ++ptIter.x) {
				_normPosSeq.push_back(ptIter);
			}
		}
	}

	posSeq.assign(_normPosSeq.begin(), _normPosSeq.end());
	return posSeq.size();
}
void GfMap::do_randMap()
{
	int nAvg = 100/(1 + NODE_COLOR_4), nLeft = 100;
	int nCount[1 + NODE_COLOR_4];
	for (int i = 0; i < sizeof(nCount)/sizeof(nCount[0]); ++i)
	{
		nCount[i] = std::min(nAvg, nLeft);
		nLeft -= nCount[i];
	}

	for (int col = 0; col < 10; ++col) {
		for (int row =0; row < 10; ++row) {
			int color;
			do {
				color = (NODE_COLOR_0 + rand()%(1 + NODE_COLOR_4 - NODE_COLOR_0));
			} while ( nCount[color] <= 0 );
			nCount[color]--;
			m_map[row][col] = color;
		}
	}
}
int GfMap::calMark_l(const std::vector<Point2i>& posSeq, std::vector<Point2i>* posPath)
{
	if (posPath) posPath->clear();

	int nMark = 0;
	bool remain;
	recorder rcd;
	do 
	{
		remain = false;
		for (int i = 0; i < posSeq.size(); ++i)
		{
			Point2i pos = posSeq[i];
			int ret = eraseElem(pos.y, pos.x);
			if (ret > 0) {
				nMark += ret * ret;
				remain = true;
				if (posPath) posPath->push_back(pos);
				break;
			}
		}
	} while (remain);
	return nMark;
}
int GfMap::evalElem(int row, int col, recorder& rcd) {
	std::deque<Point2i> que;
	Point2i ptCur = makeP2(col, row);
	que.push_back(ptCur);

	int color_org = this->get(row, col);
	if (color_org == -1 || rcd.get(ptCur)) {
		return 0;
	}

	rcd.set(ptCur);

	NodeVisitor_1_ vis_1;
	int nEval = 0;
	while (! que.empty())
	{
		ptCur = que.front();
		que.pop_front();
		nEval++;

		for (vis_1.begin(ptCur, 10, 10); ! vis_1.end(); ++vis_1)
			if (vis_1.check_bdy(vis_1.m_ptCur) && ! rcd.get(vis_1.m_ptCur) && color_org == this->get(vis_1.m_ptCur)) {
				rcd.set(vis_1.m_ptCur);
				que.push_back(vis_1.m_ptCur);
			}
	}

	if (nEval >= 2) {
		return nEval;
	}
	return 0;
}
int GfMap::eraseElem(int row, int col, bool justEval)
{
	std::deque<Point2i> que;
	Point2i ptCur = makeP2(col, row);
	que.push_back(ptCur);

	recorder rcd;
	rcd.set(ptCur);

	int color_org = this->get(row, col);
	if (color_org == -1) {
		return 0;
	}

	NodeVisitor_1_ vis_1;
	std::vector<Point2i> vec;
	while (! que.empty())
	{
		ptCur = que.front();
		que.pop_front();
		vec.push_back(ptCur);

		for (vis_1.begin(ptCur, 10, 10); ! vis_1.end(); ++vis_1)
			if (vis_1.check_bdy(vis_1.m_ptCur) && ! rcd.get(vis_1.m_ptCur) && color_org == this->get(vis_1.m_ptCur)) {
				rcd.set(vis_1.m_ptCur);
				que.push_back(vis_1.m_ptCur);
			}
	}

	if (vec.size() >= 2 && ! justEval) {
		std::sort(vec.begin(), vec.end(), [](const Point2i& ptL, const Point2i& ptR){
			return ptL.y > ptR.y;
		});
		for (int i = 0; i < vec.size(); ++i) {
			Point2i ptCur = vec[i],ptIter = vec[i];
			for (ptIter.y = ptCur.y; ptIter.y < 9; ++ptIter.y) {
				m_map[ptIter.y][ptIter.x] = m_map[ptIter.y + 1][ptIter.x];
			}
			m_map[9][ptIter.x] = -1;
		}
		return vec.size();
	}
	else if (vec.size() >= 2) {
		return vec.size();
	}
	return 0;
}
int GfMap::calMark(std::vector<Point2i>* posPath) {
	int nAvgMark = 0;
	int nMaxMark = 0;
#define CALMARK_TIMES 12
	std::vector<Point2i> l_path;
	for (int i = 0; i < CALMARK_TIMES; ++i) {
		GfMap cpMap(*this); 
		std::vector<Point2i> posSeq;
		if (i == 0) {
			this->normPos(posSeq);
		}
		else {
			this->randPos(posSeq);
		}
		int mark = cpMap.calMark_l(posSeq, posPath);
		nAvgMark += mark;

		if (nMaxMark < mark) {
			nMaxMark = std::max(nMaxMark, mark);
			if (posPath) {
				l_path.assign(posPath->begin(), posPath->end());
			}
		}
	}
	if (posPath) {
		posPath->assign(l_path.begin(), l_path.end());
	}
	//return nAvgMark / CALMARK_TIMES;
	return nMaxMark;
}



void GfMap::getMaxPos(const std::vector<Point2i>& posSeq, std::vector<tagPosMrk>& maxPosSeq, int maxNum) {
	std::multiset<tagPosMrk, std::greater<tagPosMrk>> max_que;
	
	int pos = 0;
	recorder rcd;
	for (int i = 0; i < posSeq.size(); ++i) {
		tagPosMrk tag;
		tag.mark = evalElem(posSeq[i].y, posSeq[i].x, rcd);
		tag.pos  = posSeq[i];
		max_que.insert(tag);
	}
	int val = 0;
	for (auto& iter : max_que)
	{
		if (val++ >= maxNum) {
			break;
		}
		maxPosSeq.push_back(iter);
	}
}
void GfMap::do_erasePt(Point2i pt) {
	bool color[1 + NODE_COLOR_4] = {0};
	NodeVisitor_1_ vis_l;
	for (vis_l.begin(pt, 10, 10); ! vis_l.end(); ++vis_l)
	if (vis_l.check_bdy(vis_l.m_ptCur)) {
		color[ this->get(vis_l.m_ptCur) ] = true;
	}

	int iter = rand() % (1 + NODE_COLOR_4);
	for (int p = iter; p < 1 + NODE_COLOR_4; ++p) {
		if (! color[p]) {
			CCLOG("(%d,%d)--change %d-->%d", pt.x, pt.y, this->get(pt), p);
			set(pt.y, pt.x, p);
			return ;
		}
	}
	for (int p = iter; p >= 0; --p) {
		if (! color[p]) {
			CCLOG("(%d,%d)--change %d-->%d", pt.x, pt.y, this->get(pt), p);
			set(pt.y, pt.x, p);
			return ;
		}
	}

	CCLOG("(%d,%d)--change %d-->%d", pt.x, pt.y, this->get(pt), 0);
	set(pt.y, pt.x, 0);
}
void GfMap::do_addPt(Point2i pt) {

	std::deque<Point2i> que;
	Point2i ptCur = pt;
	que.push_back(ptCur);

	int color_org = this->get(ptCur.y, ptCur.x);
	if (color_org == -1) {
		return;
	}

	recorder rcd;
	rcd.set(ptCur);

	std::vector<Point2i> vec;

	NodeVisitor_1_ vis_1;
	int nEval = 0;
	while (! que.empty())
	{
		ptCur = que.front();
		que.pop_front();
		nEval++;

		for (vis_1.begin(ptCur, 10, 10); ! vis_1.end(); ++vis_1)
			if (vis_1.check_bdy(vis_1.m_ptCur) && ! rcd.get(vis_1.m_ptCur)) {
				if (color_org == this->get(vis_1.m_ptCur)) {
					rcd.set(vis_1.m_ptCur);
					que.push_back(vis_1.m_ptCur);
				}
				else {
					vec.push_back(vis_1.m_ptCur);
				}
			}
	}

	recorder rcd2;
	int max_mrk = 0;
	int idx = 0;
	for (int i = 0; i < vec.size(); ++i) {
		int ret = evalElem(vec[i].y, vec[i].x, rcd2);
		if (max_mrk < ret) {
			max_mrk = ret;
			idx = i;
		}
	}
	if (! vec.empty()) {
		CCLOG("(%d,%d)--change %d-->%d", vec[idx].x, vec[idx].y, this->get(vec[idx]), color_org);
		this->set(vec[idx].y, vec[idx].x, color_org);
	}
}

struct timeLog {
	timeLog(const std::string& name) :m_name(name) {
		clk = clock();
	}
	~timeLog() {
		clk = clock() - clk;
		CCLOG("%s takes %dms", m_name.c_str(), clk*1000/CLOCKS_PER_SEC);
	}
private:
	std::string m_name;
	clock_t clk;
};
void GfMap::randMapWithDegree(int nDegree)
{
	timeLog log("randMapWithDegree");
#if 1
	do_randMap();
#else
	accept(s_GraphCfg_round1);
	return ;
#endif

#define MarkLim 20
#define NewtonIterNum 10
#define MaxMark  350
#define MinMark  70
#define MaxRound 50
#define REDUCE_RATIO (0.1)
#define MMParam ((MaxMark - MinMark) * (1 + REDUCE_RATIO * MaxRound) / (REDUCE_RATIO * MaxRound))
	int needMark = MaxMark - (REDUCE_RATIO * nDegree)/(1 + REDUCE_RATIO * MaxRound)*MMParam;
	CCLOG("mubiao mark=%d", needMark);
	for (int i = 0; i < NewtonIterNum; ++i)
	{
		std::vector<tagPosMrk> posMaxSeq;
		std::vector<Point2i> posSeq;
		randPos(posSeq);

		int curMark = calMark();
		CCLOG("%d: curmark=%d", i, curMark); 
		if (curMark >= needMark + MarkLim)
		{
			int mrk = 0;
			Point2i pos;
			{
				int nRepeat = 1 + abs(curMark - needMark)/25;

				getMaxPos(posSeq, posMaxSeq, nRepeat);
				std::random_shuffle(posMaxSeq.begin(), posMaxSeq.end());
				
				for (int k = 0; k < nRepeat && k < posMaxSeq.size(); ++k) {
					pos = posMaxSeq[k].pos;
					CCLOG("erase_pos (%d,%d)", pos.x, pos.y);
					do_erasePt(pos);
				}
			}
		}
		else if (curMark <= needMark - MarkLim)
		{
			int mrk = 0;
			Point2i pos;
			{
				int nRepeat = 1 + abs(curMark - needMark)/25;

				getMaxPos(posSeq, posMaxSeq, nRepeat);
				std::random_shuffle(posMaxSeq.begin(), posMaxSeq.end());

				for (int k = 0; k < nRepeat && k < posMaxSeq.size(); ++k) {
					pos = posMaxSeq[k].pos;
					CCLOG("add_pos (%d,%d)", pos.x, pos.y);
					do_addPt(pos);
				}
			}
		}
		else {
			break;
		}
	}

	{
		std::vector<Point2i> __path;

		GfMap cpMap(*this); 
		int mark = cpMap.calMark(&__path);

		std::stringstream strIs;
		for (int i = 0; i < __path.size(); ++i)
		{
			strIs<<"("<<__path[i].x<<","<<__path[i].y<<") ";
		}
		strIs<<std::endl;
		CCLOG("%s", strIs.str().c_str());

		char szInf[260]; sprintf(szInf, "mark=%d", mark);
		//cocos2d::MessageBox(szInf, "");
		CCLOG(szInf);
	}
}

GraphData* GraphDataFactory::create(const std::string& name)
{
	int round = 0;
	sscanf(name.c_str(), "round%d", &round);

	static GraphData grpfct = s_GraphCfg_round1;
	
	GfMap gfMap;
	gfMap.randMapWithDegree(round+1);

	for (int col = 0; col < 10; ++col) {
		for (int row =0; row < 10; ++row) {
			int color = gfMap.get(row, col);
			if (color == -1) {
				grpfct.map[row][col].m_bDisable = true;
			}
			else {
				grpfct.map[row][col].catgy.color = (enNodeColor)color;
			}
		}
	}

	return &grpfct;
}



