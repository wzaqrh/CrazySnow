//
//  TableViewHelper.h
//  MushroomDefense
//
//  Created by macos1 on 14-12-12.
//
//
#pragma once
#include "PreCompile.h"

class TableViewUtility
{
public:
    static cocos2d::extension::TableView* makeTableView(cocos2d::extension::TableViewDataSource* pSource, const cocos2d::Point& contentPos, const cocos2d::Size& contentSize,
                                                        int tablePaddToBgTop, int tablePaddToBgBottom, int tablePaddToBgLeft, int localZOrder);
    static cocos2d::extension::TableView* makeTableView(cocos2d::extension::TableViewDataSource* pSource, cocos2d::Node* pTableBg,
                                                        int tablePaddToBgTop, int tablePaddToBgBottom, int tablePaddToBgLeft, int localZOrder);
};

typedef std::function<void(cocos2d::Node* column, cocos2d::extension::TableViewCell* cell)> TableViewTouchCallback;
typedef std::function<cocos2d::Node*(ssize_t idx)> TableViewCreateCellCallback;
typedef std::function<size_t()> TableViewCellNumCallback;
class TableViewDataAdapter
: public cocos2d::extension::TableViewDataSource
, public cocos2d::extension::TableViewDelegate
{
public:
    TableViewDataAdapter();
    TableViewDataAdapter& setCellNumber(ssize_t number);
    TableViewDataAdapter& setCellSize(const cocos2d::Size& cellSize);
    TableViewDataAdapter& setCellTouchCallback(const TableViewTouchCallback& callback);
    TableViewDataAdapter& setCellCreateCallback(const TableViewCreateCellCallback& callback);
    TableViewDataAdapter& setGetCellNumCallback(const TableViewCellNumCallback& callback);
protected:
    virtual cocos2d::Size cellSizeForTable(cocos2d::extension::TableView *table) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual void tableCellHighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual void tableCellUnhighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
private:
    cocos2d::Size m_cellSize;
    ssize_t m_cellNumber;
    TableViewTouchCallback m_touchCallback;
    TableViewCreateCellCallback m_createCallback;
    TableViewCellNumCallback m_cellNumCallback;
};