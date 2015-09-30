#include "ShopLayer.h"
#include "common/DialogComponent.h"
#include "common/ResourceUtility.h"
using namespace cocos2d;

enum enZorder {
    ZORDER_BG = -1,
    ZORDER_TABLEVIEW = 1,
};

#define PURCHASE_GOODS_NUM (5)
enum enTag {
    TAG_BTN_PURCHASE_BASE = 189,
    TAG_BTN_PURCHASE_MAX = TAG_BTN_PURCHASE_BASE + PURCHASE_GOODS_NUM
};

ShopLayer::ShopLayer() 
:m_tableView(nullptr)
,m_contentLayer(nullptr)
,m_tableAdapter(nullptr)
{}

ShopLayer::~ShopLayer() {
    CC_SAFE_DELETE(m_tableAdapter);
}

bool ShopLayer::init() {
	Layer::init();
    setContentSize(Size(640, 960));
	initGoodList();
	initUI();
	return true;
}

void ShopLayer::initGoodList() {
	ShopGood goods[PURCHASE_GOODS_NUM] = {
		{15,10,3},{30,25,6},{50,50,10},{100,80,15},{200,200,30}
	};
	m_arrGoods.assign(goods, goods + PURCHASE_GOODS_NUM);
}

void ShopLayer::setShopEventCallback(ShopEventCallback callback) {
	m_shopEventcallback = callback;
}

static const int  TABLE_PADD_TOP = 0;
static const int  TABLE_PADD_BOTTOM = 0;
static const int  TABLE_PADD_LEFT = 0;
static const Size TABLE_CELL_SIZE = Size(472, 98);

void ShopLayer::initUI()
{
	Size contentSize;
    
    {
        Sprite* background = ResourceUtility::createSprite(DIR_SHOP, "shop_bg.png");
        this->addChild(background, ZORDER_BG);
        
        contentSize = background->getContentSize();
        background->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));
    }
    
    {
        DialogTitle* title = DialogTitle::create(CS_DIALOG_TITLE_SHOP, "商城");
        title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        title->setPosition(Vec2(contentSize.width / 2, 575));
        this->addChild(title);
    }
    
    {
        Menu* pMenu = Menu::create();
        pMenu->setAnchorPoint(Point::ZERO);
        pMenu->setPosition(Point::ZERO);
        this->addChild(pMenu);
        
        auto _menuItem = ResourceUtility::createMenuItem("Cancel.png", "Cancel.png", DIR_SHOP, CC_CALLBACK_1(ShopLayer::onMenuClose, this));
        _menuItem->setPosition(Vec2(466, 612));
        pMenu->addChild(_menuItem);
    }

    {
        m_tableAdapter = new TableViewDataAdapter;
        m_tableAdapter->setCellSize(TABLE_CELL_SIZE)
        .setGetCellNumCallback(CC_CALLBACK_0(std::vector<ShopGood>::size, &m_arrGoods))
        .setCellCreateCallback(CC_CALLBACK_1(ShopLayer::createTableCell, this));
        
        m_tableView = TableViewUtility::makeTableView(m_tableAdapter, Vec2((contentSize.width - TABLE_CELL_SIZE.width) / 2, 80),
                                                      Size(TABLE_CELL_SIZE.width, TABLE_CELL_SIZE.height * PURCHASE_GOODS_NUM),
                                                      TABLE_PADD_TOP, TABLE_PADD_BOTTOM, TABLE_PADD_LEFT, ZORDER_TABLEVIEW);
        this->addChild(m_tableView);
    }

	setContentSize(contentSize);
	ignoreAnchorPointForPosition(false);
    this->setPosition(Vec2::ANCHOR_MIDDLE);
}

void ShopLayer::onMenuClose(cocos2d::Ref* sender) {
    this->removeFromParent();
}
void ShopLayer::onPurchase(int idx) {
    //do something
}

typedef std::function<void(int)> GoodsPurchseCallback;
class GoodListCell : public cocos2d::Node {
public:
    static GoodListCell* create(const ShopGood& goods);
    void setPurchaseCallback(const GoodsPurchseCallback& purchaseCallback, int tag);
private:
    void onMenuPurchase(cocos2d::Ref* sender);
    void initUI(const ShopGood& goods);
private:
    GoodsPurchseCallback m_purchaseCallback;
    int m_purchaseCbTag;
};
GoodListCell* GoodListCell::create(const ShopGood& goods) {
    GoodListCell* pNode = new GoodListCell;
    if (! pNode->init()) {
        delete pNode;
        return nullptr;
    }
    pNode->initUI(goods);
    pNode->autorelease();
    return pNode;
}
void GoodListCell::onMenuPurchase(cocos2d::Ref* sender) {
    if (m_purchaseCallback) {
        m_purchaseCallback(m_purchaseCbTag);
    }
}
void GoodListCell::setPurchaseCallback(const GoodsPurchseCallback& purchaseCallback, int tag) {
    m_purchaseCallback = purchaseCallback;
    m_purchaseCbTag = tag;
}
void GoodListCell::initUI(const ShopGood& goods) {
    Size contentSize;
    
    {
        Node* background = ResourceUtility::createSprite(DIR_SHOP, "shop_listbg.png");
        this->addChild(background);
        
        Size contentSize = background->getContentSize();
        background->setPosition(Vec2(contentSize.width / 2, contentSize.height / 2));
    }

    {
        Node* diamond = ResourceUtility::createSprite(DIR_SHOP, "zuan_ico.png");
        diamond->setPosition(Vec2(41, 45));
        this->addChild(diamond);
    }
    
    {
        char buf[260]; sprintf(buf, "%d", goods.m_heart);
		Label* label1 = ResourceUtility::createLabel(buf, 24, Size(50, 32), TextHAlignment::CENTER, TextVAlignment::CENTER);
        label1->setColor(Color3B::WHITE);
		label1->setPosition(Point(87, 45));
		this->addChild(label1);
    }
    
    {
        Node* pZeng = ResourceUtility::createSprite(DIR_SHOP, "zheng_ico.png");
        pZeng->setPosition(Vec2(183, 45));
        this->addChild(pZeng);
    }
    
    {
        char buf[260]; sprintf(buf, "%d", goods.m_heart);
		Label* label1 = ResourceUtility::createLabel(buf, 24, Size(50, 32), TextHAlignment::CENTER, TextVAlignment::CENTER);
        label1->setColor(Color3B::YELLOW);
		label1->setPosition(Point(260, 45));
		this->addChild(label1);
    }
    
    Menu* pMenu = Menu::create();
    pMenu->setAnchorPoint(Point::ZERO);
    pMenu->setPosition(Point::ZERO);
    this->addChild(pMenu);
    
    {
        char buf[100]; sprintf(buf, "cny_%d.png", goods.m_price);
        MenuItem* _menuItem = ResourceUtility::createMenuItem(buf, buf, DIR_SHOP, CC_CALLBACK_1(GoodListCell::onMenuPurchase, this));
        _menuItem->setPosition(Vec2(391, 45));
        pMenu->addChild(_menuItem);
    }
}

cocos2d::Node* ShopLayer::createTableCell(ssize_t idx)
{
	GoodListCell* cell = GoodListCell::create(m_arrGoods[idx]);
    cell->setPurchaseCallback(CC_CALLBACK_1(ShopLayer::onPurchase, this), idx);
	return cell;
}



