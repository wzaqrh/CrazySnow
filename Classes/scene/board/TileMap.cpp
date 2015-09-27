//
//  TileMap.cpp
//  proj.ios
//
//  Created by root911 on 14-5-13.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#include "TileMap.h"
#include "graph/Graph.h"
#include "data/GameEntity.h"
#include "ResourceDef.h"
using namespace cocos2d;

enum enZorder {
    ZORDER_BEAD_BG = -1
};

TileMapLayer::TileMapLayer()
	:m_LockCount(0)
{
    memset(m_map, 0, sizeof(m_map));
    this->setPosition(Vec2(0, 100));
}
TileMapLayer::~TileMapLayer()
{

}
void TileMapLayer::lockBoard() {
	++m_LockCount;
}
void TileMapLayer::unlockBoard() {
	--m_LockCount;
}
void TileMapLayer::initWith(GraphMat* graph, GraphData* cfg)
{
    m_pGraph = graph;
    
    int rows = graph->m_rows;
    int cols = graph->m_cols;
    this->setContentSize(Size(cfg->box_width*cols, cfg->box_height*rows));

	GraphNodeVisitorGraph vis;
	for (vis.begin(rows,cols); !vis.end(); ++vis)
	{
		GraphNode* node = graph->get_node(vis.m_ptCur);
		assert(node);
		if (node->isEnable()) {
			this->addCell( vis.m_ptCur, &node->get_category(), e_cell_corrt );
		}
	}
}
void TileMapLayer::push_secondary_cells(const Point2i pos) {
	for (int i=0; i<e_cell_max; ++i) {
		if (i != e_cell_corrt && m_map[pos.y][pos.x][i]) {
			m_map[pos.y][pos.x][i]->retain();
			m_map[pos.y][pos.x][i]->removeFromParent();
		}
	}
}
void TileMapLayer::pop_secondary_cells(const Point2i pos) {
	for (int i=0; i<e_cell_max; ++i) {
		if (i != e_cell_corrt && m_map[pos.y][pos.x][i]) {
			Node*& sp_i = m_map[pos.y][pos.x][i];
			m_map[pos.y][pos.x][e_cell_corrt]->addChild(sp_i, 2);
			sp_i->release();

			Size szCell = m_map[pos.y][pos.x][e_cell_corrt]->getContentSize();
			sp_i->setPosition(Point(szCell.width/2, szCell.height/2));
		}
	}
}
void TileMapLayer::push_main_cell(const Point2i pos) {
	m_map[pos.y][pos.x][e_cell_corrt]->retain();
	m_map[pos.y][pos.x][e_cell_corrt]->removeFromParent();
}
void TileMapLayer::pop_main_cell(const Point2i pos) {
	this->addChild(m_map[pos.y][pos.x][e_cell_corrt], 2);
	m_map[pos.y][pos.x][e_cell_corrt]->release();

	Point newPoint =  m_pGraph->m_coordCvt.getCoord(pos);
	m_map[pos.y][pos.x][e_cell_corrt]->setPosition(newPoint);
}
void TileMapLayer::changeCell(Point2i pos, const NodeCategory* categNew, enCellType cellTyp)
{
	assert( m_map[pos.y][pos.x][cellTyp] );

	if (cellTyp == e_cell_corrt)
	{
		push_secondary_cells(pos);
		push_main_cell(pos);
		m_map[pos.y][pos.x][e_cell_corrt]->release();

		Sprite* sp = BeadFactory::createBead(*categNew);
		m_map[pos.y][pos.x][e_cell_corrt] = sp;

		m_map[pos.y][pos.x][e_cell_corrt]->retain();
		pop_main_cell(pos);
		pop_secondary_cells(pos);
	}
	else
	{
		removeCell(pos, cellTyp);
		addCell(pos, categNew, cellTyp);
	}
}
void TileMapLayer::addCell(Point2i pos, const NodeCategory* categ, enCellType cellTyp)
{
    assert( m_map[pos.y][pos.x][cellTyp] == NULL );
    Point newPos =  m_pGraph->m_coordCvt.getCoord(pos);
    
	Sprite* sp = NULL;
	if (cellTyp == e_cell_bg) {
		sp = Sprite::create(DIR_MAIN"block_select.png");
	}
	else {
		sp = BeadFactory::createBead(*categ);
	}
   
	if (cellTyp == e_cell_oppos) {
		this->addChild(sp, 1);
		sp->setPosition(newPos);
	}
	else if (cellTyp == e_cell_corrt) {
		this->addChild(sp, 2);
		sp->setPosition(newPos);
	}
	else {
		assert(cellAt(pos,e_cell_corrt));
		m_map[pos.y][pos.x][e_cell_corrt]->addChild(sp, 2);
		Size szCell = m_map[pos.y][pos.x][e_cell_corrt]->getContentSize();
		sp->setPosition(Point(szCell.width/2, szCell.height/2));
	}
	
    m_map[pos.y][pos.x][cellTyp] = sp;
}
void TileMapLayer::removeCell(Point2i pos, enCellType cellTyp)
{
    assert(pos.y < MAX_VERTEX_ROWS && pos.x < MAX_VERTEX_COLS);
    if( m_map[pos.y][pos.x][cellTyp] ) {
        m_map[pos.y][pos.x][cellTyp]->removeFromParentAndCleanup(true);
        m_map[pos.y][pos.x][cellTyp] = NULL;

		if (cellTyp == e_cell_corrt) {
			for (int i=0; i<e_cell_max; ++i) {
				if( i != e_cell_oppos )
					m_map[pos.y][pos.x][cellTyp] = NULL;
			}
		}
    }
}
cocos2d::Node* TileMapLayer::cellAt(Point2i pos, enCellType cellTyp)
{
	assert( pos.y<MAX_VERTEX_ROWS && pos.x<MAX_VERTEX_COLS );
	Point newPoint =  m_pGraph->m_coordCvt.getCoord(pos);

    return m_map[pos.y][pos.x][cellTyp];
}
void TileMapLayer::swapCellRef(const Point2i posA, const Point2i posB)
{
	for (int i = 0; i < e_cell_max; ++i) {
		std::swap(m_map[posA.y][posA.x][i], m_map[posB.y][posB.x][i]);
	}
}
void TileMapLayer::swapCellRef(const Point2i pos, enCellType cellTypA, enCellType cellTypB)
{
	if(cellTypA == e_cell_corrt || cellTypB == e_cell_corrt) {
		push_secondary_cells(pos);
		push_main_cell(pos);
		std::swap(m_map[pos.y][pos.x][cellTypA], m_map[pos.y][pos.x][cellTypB]);
		pop_main_cell(pos);
		pop_secondary_cells(pos);
	}
	else {
		std::swap(m_map[pos.y][pos.x][cellTypA], m_map[pos.y][pos.x][cellTypB]);
	}
}
void TileMapLayer::removeCellAll(enCellType cellTyp)
{
	GraphNodeVisitorGraph vis;
	for (vis.begin(m_pGraph->m_rows,m_pGraph->m_cols); !vis.end(); ++vis) {
		if (cellAt(vis.m_ptCur)) {
			removeCell(vis.m_ptCur, cellTyp);
		}
	}
}

bool TileMapLayer::containCoord(cocos2d::Point coord, Point2i* pos)
{
	if (isBoardLocked()) {
		return false;
	}
	
    Point2i __pos = m_pGraph->m_coordCvt.getPos(coord);
    if( pos ) *pos = __pos;
    if( ! m_pGraph->check_bdy_enable(__pos) || m_pGraph->isNodeLocked(__pos) )
        return false;
    
    Node* cell = cellAt(__pos, e_cell_corrt);
    assert(cell);
    Rect bounding_rect = cell->boundingBox();
    return bounding_rect.containsPoint(coord);
}

cocos2d::Sprite* BeadFactory::createBead(enBeadCategory spec) {
	if (spec != CHESS_CATEGORY_ARMOR) {
		return createBead(NodeCategory::make(enNodeColor::NODE_COLOR_SPEC, spec, -1));
	}
	else {
		return createBead(NodeCategory::make(enNodeColor::NODE_COLOR_0, spec, -1));
	}
}
cocos2d::Sprite* BeadFactory::createBead(NodeCategory __category)
{
    std::string sPrefix;
	int         nPostfix;
	switch (__category.color) {
        case NODE_COLOR_0:
			nPostfix = 0;
			break;
        case NODE_COLOR_1:
			nPostfix = 1;
			break;
        case NODE_COLOR_2:
			nPostfix = 2;
			break;
        case NODE_COLOR_3:
			nPostfix = 3;
			break;
        case NODE_COLOR_4:
			nPostfix = 4;
			break;
		case NODE_COLOR_SPEC:
			nPostfix = 0;
			break;
        default:
            assert(0);
            break;
    }
	switch (__category.spec) {
	case CHESS_CATEGORY_NORM:
		sPrefix = "stone"; break;
	case CHESS_CATEGORY_UNKOWN:
		sPrefix = "unkown"; break;
	case CHESS_CATEGORY_ARMOR:
		sPrefix = "rook"; break;
	case CHESS_CATEGORY_HORSE:
		sPrefix = "horse"; break;
	case CHESS_CATEGORY_ARTILLERY:
		sPrefix = "artillery"; break;
	case CHESS_CATEGORY_ELEPH:
		sPrefix = "eleph"; break;
	case CHESS_CATEGORY_SDOLIER:
		sPrefix = "soldier"; break;
	case CHESS_CATEGORY_QUEEN:
		sPrefix = "queen"; break;
	case CHESS_CATEGORY_DOUBLE:
		sPrefix = "double"; break;
	default:
		assert(0);
		break;
	}

	char buf[260];
	sprintf(buf, DIR_STONE"%s_%d.png", sPrefix.c_str(), nPostfix);
	Sprite* bead = Sprite::create(buf);
    
    {
        auto bead_bg = Sprite::create(DIR_STONE"ge_bg.png");
        const Size& beadSize = bead->getContentSize();
        bead_bg->setPosition(Vec2(beadSize.width / 2, beadSize.height / 2));
        bead->addChild(bead_bg, ZORDER_BEAD_BG);
    }
    
    return bead;
}
cocos2d::Sprite* BeadFactory::createBlastStar(enNodeColor color, float fscale) {
	Sprite* spBlstStar = Sprite::create("zzImage/stone/star.png");
	static Color3B color_seq[5] = {
		Color3B(235, 0, 161), Color3B(235, 98, 0), Color3B(161, 0, 235), Color3B(0, 161, 235), Color3B(90, 235, 0)
	};
	spBlstStar->setColor(color_seq[color]);
	spBlstStar->setScale(fscale);
	return spBlstStar;
}