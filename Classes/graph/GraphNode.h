//
//  GraphNode.h
//  proj.ios
//
//  Created by root911 on 14-5-12.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//
#pragma once
#include "GraphCommDef.h"
#include "GraphIterator.h"


class GraphNode {
public:
    GraphNode();
	void swap_value(GraphNode& node);
    void assign_value(const GraphNode& node);
    void setAttrib(const GraphAttrib& attrib);
    GraphAttrib&  getAttrib();
	const GraphAttrib&  getAttrib() const;
	void set_category(const NodeCategory& categ);
	const NodeCategory& get_category() const;
	void setEnable(bool bEnable);
	bool isEnable() const;

	static GraphNode* DisabledGraphNode();
	static GraphNode* createEmptyNode();
private:
    GraphAttrib m_attrib;
};

