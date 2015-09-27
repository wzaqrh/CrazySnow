#include "RollCardUtinity.h"
#include "graph/GraphMatrix.h"

NodeCategory s_calRollCardResult_1(const NodeCategory& src_categ, enNodeColor neibColor) 
{
	NodeCategory res_categ;
#if 0
	static int s_seed = 0;
	if (s_seed == 0){
		s_seed = time(NULL);
		srand(s_seed);
	}
	if (src_categ.spec_data >= 6) {
		static enBeadCategory seeds_spec[4] = {CHESS_CATEGORY_QUEEN, CHESS_CATEGORY_ARMOR, CHESS_CATEGORY_DOUBLE, CHESS_CATEGORY_SDOLIER};
		enNodeColor seeds_color[4]      = {neibColor, NODE_COLOR_SPEC, neibColor, neibColor};
		int ch = rand()%4;
		res_categ = NodeCategory::make(seeds_color[ch], seeds_spec[ch], -1);
	}
	else if(src_categ.spec_data >= 4) {
		static enBeadCategory seeds_spec[4] = {CHESS_CATEGORY_ELEPH, CHESS_CATEGORY_ARTILLERY, CHESS_CATEGORY_HORSE, CHESS_CATEGORY_ELEPH};
		enNodeColor seeds_color[4]      = {neibColor, neibColor, neibColor, neibColor};
		int ch = rand()%4;
		res_categ = NodeCategory::make(seeds_color[ch], seeds_spec[ch], -1);
	}
#else
	res_categ = NodeCategory::make(neibColor, CHESS_CATEGORY_ELEPH, -1);
#endif
	return res_categ;
}
NodeCategory calRollCardResult(const NodeCategory& src_categ, GraphMatrix* graph, Point2i trig_pt) {
	NodeCategory categoryB;

	Point2i ptLft = makeP2(trig_pt.x-1,trig_pt.y);
	Point2i ptRht = makeP2(trig_pt.x+1,trig_pt.y);
	if (graph->check_bdy_enable(ptLft)) {
		categoryB = s_calRollCardResult_1(src_categ, graph->get_node(ptLft)->get_category().color);
	}
	else {
		categoryB = s_calRollCardResult_1(src_categ, graph->get_node(ptRht)->get_category().color);
	}
	return categoryB;
}




NodeCategory s_calRollCardResult(const NodeCategory& src_categ, enNodeColor neibColor, enBeadCategory spec) 
{
	NodeCategory res_categ;
	if (CHESS_CATEGORY_ARMOR == spec) {
		res_categ = NodeCategory::make(NODE_COLOR_SPEC, spec, -1);
	}
	else {
		res_categ = NodeCategory::make(neibColor, spec, -1);
	}
	return res_categ;
}
NodeCategory calRollCardResult(const NodeCategory& src_categ, GraphMatrix* graph, Point2i trig_pt, enBeadCategory spec) {
	NodeCategory categoryB = NodeCategory::make(enNodeColor::NODE_COLOR_0, CHESS_CATEGORY_HORSE, -1);

	GraphNodeVisitorNeib1 vis;
	for (vis.begin(trig_pt); !vis.end(); ++vis)
		if (graph->check_bdy_enable(vis.m_ptCur)) {
			categoryB = s_calRollCardResult(src_categ, graph->get_node(vis.m_ptCur)->get_category().color, spec);
		}
	return categoryB;
}