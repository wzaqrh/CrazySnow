#pragma once
#include "graph/GraphCommDef.h"

NodeCategory calRollCardResult(const NodeCategory& src_categ, GraphMatrix m_graph, Point2i pos);
NodeCategory calRollCardResult(const NodeCategory& src_categ, GraphMatrix* m_graph, Point2i pos, enBeadCategory spec);