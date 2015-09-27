//
//  TableViewHelper.cpp
//  MushroomDefense
//
//  Created by macos1 on 14-12-12.
//
//

#include "TableViewHelper.h"
USING_NS_CC;

enum enTag {
    TAG_CELL_MAIN = 189
};

cocos2d::extension::TableView* TableViewUtility::makeTableView(cocos2d::extension::TableViewDataSource* pSource, const cocos2d::Point& contentPos, const cocos2d::Size& contentSize,
                                                               int tablePaddToBgTop, int tablePaddToBgBottom, int tablePaddToBgLeft, int localZOrder)
{
    cocos2d::extension::TableView* m_tableView = extension::TableView::create(pSource, Size(contentSize.width, contentSize.height - tablePaddToBgTop - tablePaddToBgBottom), nullptr);
    m_tableView->setVerticalFillOrder(extension::TableView::VerticalFillOrder::BOTTOM_UP);
    //m_tableView->setDelegate(this);
    m_tableView->setPosition(contentPos + Point(tablePaddToBgLeft, tablePaddToBgBottom));
    m_tableView->setLocalZOrder(localZOrder);
    m_tableView->setAnchorPoint(Vec2::ZERO);
    //this->addChild(m_tableView, ZORDER_TABLEVIEW);
    return m_tableView;
}

cocos2d::extension::TableView* TableViewUtility::makeTableView(cocos2d::extension::TableViewDataSource* pSource, Node* pTableBg, int tablePaddToBgTop, int tablePaddToBgBottom, int tablePaddToBgLeft, int localZOrder)
{
    const Size& contentSize = pTableBg->getContentSize();
    const Point& contentPos = pTableBg->getPosition();
    return makeTableView(pSource, contentPos, contentSize, tablePaddToBgTop, tablePaddToBgBottom, tablePaddToBgLeft, localZOrder);
}



/************************************
 * TableViewDataAdapter
 ***********************************/
TableViewDataAdapter::TableViewDataAdapter()
:m_cellSize(Size::ZERO)
,m_cellNumber(0)
{}

TableViewDataAdapter& TableViewDataAdapter::setCellNumber(ssize_t number)
{
    m_cellNumber = number;
    return *this;
}

TableViewDataAdapter& TableViewDataAdapter::setCellTouchCallback(const TableViewTouchCallback& callback)
{
    m_touchCallback = callback;
    return *this;
}

TableViewDataAdapter& TableViewDataAdapter::setCellCreateCallback(const TableViewCreateCellCallback& callback)
{
    m_createCallback = callback;
    return *this;
}

TableViewDataAdapter& TableViewDataAdapter::setGetCellNumCallback(const TableViewCellNumCallback& callback)
{
    m_cellNumCallback = callback;
    return *this;
}

TableViewDataAdapter& TableViewDataAdapter::setCellSize(const cocos2d::Size& cellSize)
{
    m_cellSize = cellSize;
    return *this;
}

cocos2d::Size TableViewDataAdapter::cellSizeForTable(cocos2d::extension::TableView *table)
{
    return m_cellSize;
}

cocos2d::extension::TableViewCell* TableViewDataAdapter::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    cocos2d::extension::TableViewCell* pCell = table->dequeueCell();
    if (pCell == nullptr) {
        pCell = extension::TableViewCell::create();
    }
    else {
        pCell->removeAllChildren();
    }
    
    if (m_createCallback) {
        cocos2d::Node* column = m_createCallback(idx);
        column->setTag(TAG_CELL_MAIN);
        pCell->addChild(column);
    }
    
    return pCell;
}

ssize_t TableViewDataAdapter::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return m_cellNumCallback ? m_cellNumCallback() : m_cellNumber;
}

void TableViewDataAdapter::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    if (m_touchCallback) {
        cocos2d::Node* column = cell->getChildByTag(TAG_CELL_MAIN);
        m_touchCallback(column, cell);
    }
}

void TableViewDataAdapter::tableCellHighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{}

void TableViewDataAdapter::tableCellUnhighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{}