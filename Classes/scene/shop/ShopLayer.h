#pragma once
#include "common/CommonDef.h"
#include "common/TableViewHelper.h"

typedef struct tagShopGood {
	int m_heart;
	int m_heartExt;
	int m_price;
}ShopGood;

typedef enum tagShopEventType {
    CS_SHOP_EVENT_CLOSE,
}ShopEventType;
typedef int CS_ShopEventType;
typedef std::function<void(CS_ShopEventType)> ShopEventCallback;

class ShopLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(ShopLayer);
	ShopLayer();
    ~ShopLayer();
	virtual bool init();
public:
	void setShopEventCallback(ShopEventCallback callback);
private:
	void initUI();
	void initGoodList();
	void onMenuClose(cocos2d::Ref* sender);
    void onPurchase(int idx);
private:
    cocos2d::Node* createTableCell(ssize_t idx);
private:
    TableViewDataAdapter*    m_tableAdapter;
	cocos2d::extension::TableView* m_tableView;
    cocos2d::Layer*          m_contentLayer;
	ShopEventCallback        m_shopEventcallback;
	std::vector<ShopGood>    m_arrGoods;
};