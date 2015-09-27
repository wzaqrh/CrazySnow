//
//  GraphNode.cpp
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#include "GraphNode.h"
#include <queue>
#include <vector>
using namespace std;


GraphNode::GraphNode()
{

}
void GraphNode::swap_value(GraphNode& node)
{
	std::swap(node.m_attrib, this->m_attrib);
}
void GraphNode::assign_value(const GraphNode& node)
{
	m_attrib = node.getAttrib();
}
void GraphNode::setAttrib(const GraphAttrib& attrib)
{
	m_attrib = attrib;
}
GraphAttrib&  GraphNode::getAttrib()
{
	return m_attrib;
}
const GraphAttrib&  GraphNode::getAttrib() const
{
	return m_attrib;
}
void GraphNode::set_category(const NodeCategory& categ) {
	m_attrib.catgy = categ;
}
const NodeCategory& GraphNode::get_category() const
{
	return m_attrib.catgy;
}
void GraphNode::setEnable(bool bEnable)
{
	m_attrib.m_bDisable = ! bEnable;
}
bool GraphNode::isEnable() const
{
	return ! m_attrib.m_bDisable;
}

GraphNode* GraphNode::DisabledGraphNode()
{
	static GraphNode* pDisNode = NULL;
	if (pDisNode == NULL)
	{
		pDisNode = new GraphNode;
		pDisNode->setEnable(false);
	}
	return pDisNode;
}
GraphNode* GraphNode::createEmptyNode()
{
	GraphNode* pNode = new GraphNode;
	return pNode;
}