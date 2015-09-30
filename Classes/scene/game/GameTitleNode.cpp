#include "GameTitleNode.h"
#include "scene/board/TileMap.h"
#include "scene/game/PauseBoard.h"
#include "data/GameEntity.h"
#include "entity/StageEntity.h"
#include "StringUpdater.h"
#include "data/UserInfo.h"
using namespace cocos2d;

#define ITEM_COST_DIAMOND (6)

enum enZorder {
    ZORDER_MENU = 1,
};

enum enTag {
    TAG_BTN_PAUSE = 189,
    TAG_BTN_RESET,
    TAG_BTN_REFRESH,
    TAG_BTN_ADD_DIMOND
};

GameTitleNode::GameTitleNode() {
	m_nCurScore  = 1;
	m_nNeedScore = 1000;
	m_curStageId  = 0;
	m_nNextBonusNeedStar = 0;

	m_spBonus = NULL;

	m_pPauseBoard = NULL;
	m_pTouchListener = NULL;
}
GameTitleNode::~GameTitleNode() {
    StageEntity::getGameUser()->removeObserver(this);
}

void GameTitleNode::setEventHandler(const GameTitleEventCallback& callback) {
	m_callback = callback;
}

void GameTitleNode::enter(const GameTitleInitCallback& onInitOk)
{
    setContentSize(Size(640, 960));
    StageEntity::getGameUser()->addObserver(this, CC_CALLBACK_1(GameTitleNode::onStageDataChanged, this));
    
    auto gameStatus = StageEntity::getGameUser();
	m_nCurScore   = gameStatus->getCurScore();
	m_curStageId  = StageEntity::Inst()->getStageId();
	m_nNextBonusNeedStar = gameStatus->getBonusNeedStar() - gameStatus->getBonusStar();
    
    auto gameBase = StageEntity::getGameBase();
    m_nNeedScore = gameBase->m_lbNeedScore + gameStatus->getTotalNeedScore();
    //关卡
	{
        auto pCoumBg = ResourceUtility::createSprite(DIR_MAIN, "coum_bg.png");
        pCoumBg->setPosition(Vec2(143, 834));
        this->addChild(pCoumBg);
        
		auto pGuanka = ResourceUtility::createSprite(DIR_MAIN, "guanka.png");
		pGuanka->setPosition(Vec2(70, 834));
        this->addChild(pGuanka);

        char buf[100];
		sprintf(buf, "%d", m_curStageId);//di ji guang
		m_lbCurStage = LabelNodeUtility(buf).setContentSize(Size(150, 48)).setPlistName(DIR_FONT_ATLAS1).build();
		m_lbCurStage->setPosition(Vec2(123, 810));
		this->addChild(m_lbCurStage);
	}
	
    //目标分
	{
        auto pCoumBg = ResourceUtility::createSprite(DIR_MAIN, "coum_bg.png");
        pCoumBg->setPosition(Vec2(484, 834));
        this->addChild(pCoumBg);
        
		auto pMubiao = ResourceUtility::createSprite(DIR_MAIN, "traget.png");//mu biao fen
		pMubiao->setPosition(Vec2(420, 834));
		this->addChild(pMubiao);

        char buf[100];
		sprintf(buf, "%d", m_nNeedScore);
		m_lbNeedScore = LabelNodeUtility(buf).setContentSize(Size(150, 48)).setPlistName(DIR_FONT_ATLAS1).build();
		m_lbNeedScore->setPosition(Vec2(458, 810));
		this->addChild(m_lbNeedScore);
	}
	
    //历史最高分
	{
        auto game_head = ResourceUtility::createSprite(DIR_MAIN, "game_head.png");
        game_head->setPosition(Vec2(getContentSize().width / 2, 960 + 59));
        game_head->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        this->addChild(game_head, -1);
        
        auto history_mark = ResourceUtility::createSprite(DIR_MAIN, "lszgf.png");//history max mark
        history_mark->setPosition(Vec2(116, 923));
        this->addChild(history_mark);
        
        char buf[100];
		sprintf(buf, "%d", UserInfo::Inst()->getHistMaxScore());
		m_lbCurScore = LabelNodeUtility(buf).setContentSize(Size(100, 45)).setPlistName(DIR_FONT_ATLAS1).build();
		m_lbCurScore->setPosition(Point(240, 899));
		this->addChild(m_lbCurScore);
        
        //char buf[100];
		sprintf(buf, "%d", m_nCurScore);
		m_lbCurScore = LabelNodeUtility(buf).setContentSize(Size(155, 45)).setPlistName(DIR_FONT_ATLAS1).build();
		m_lbCurScore->setPosition(Point(240 + 130, 899));
		this->addChild(m_lbCurScore);
	}

    //下一个道具
	{
        auto nextProp = ResourceUtility::createSprite(DIR_MAIN, "cx.png");
        nextProp->setPosition(Vec2(437, 777));
        this->addChild(nextProp);
        
        char buf[100];
		sprintf(buf, "%d", m_nNextBonusNeedStar);
		m_lbNxtBonus = LabelNodeUtility(buf).setContentSize(Size(50, 25)).setPlistName(DIR_FONT_ATLAS1).build();
		m_lbNxtBonus->setPosition(Vec2(375, 765));
		this->addChild(m_lbNxtBonus);
	}

	m_pMenu = Menu::create();
	m_pMenu->setAnchorPoint(Point::ZERO);
	m_pMenu->setPosition(Point::ZERO);
	this->addChild(m_pMenu, ZORDER_MENU);

	{
        auto _menuItem = ResourceUtility::createMenuItem("stop.png", "stop.png", DIR_MAIN, CC_CALLBACK_1(GameTitleNode::onMenuCallback, this));
		_menuItem->setPosition(Vec2(580, 919));
		_menuItem->setTag(TAG_BTN_PAUSE);
		m_pMenu->addChild(_menuItem);
	}
#ifdef CS_ENBALE_SHOP
	{
        auto _menuItem = ResourceUtility::createMenuItem("cw.png", "cw.png", DIR_MAIN, CC_CALLBACK_1(GameTitleNode::onMenuCallback, this));
		_menuItem->setPosition(Vec2(101, 762));
		_menuItem->setTag(TAG_BTN_RESET);
		m_pMenu->addChild(_menuItem);
	}

	{
        auto _menuItem = ResourceUtility::createMenuItem("next.png", "next.png", DIR_MAIN, CC_CALLBACK_1(GameTitleNode::onMenuCallback, this));
		_menuItem->setPosition(Vec2(181, 762));
		_menuItem->setTag(TAG_BTN_REFRESH);
		m_pMenu->addChild(_menuItem);
	}
    
    {
        auto addMo = ResourceUtility::createSprite(DIR_MAIN, "mon_bg.png");
        addMo->setPosition(Vec2(131, 688));
        this->addChild(addMo);
        
        char buf[100]; sprintf(buf, "%d", UserInfo::Inst()->getDiamonds());
        m_curDiamond = LabelNodeUtility(buf).setContentSize(Size(138, 35)).setPlistName(DIR_FONT_ATLAS1).build();
		m_curDiamond->setPosition(Vec2(62, 671));
		this->addChild(m_curDiamond);
        
        auto _menuItem = ResourceUtility::createMenuItem("add_mo.png", "add_mo.png", DIR_MAIN, CC_CALLBACK_1(GameTitleNode::onMenuCallback, this));
		_menuItem->setPosition(Vec2(220, 688));
		_menuItem->setTag(TAG_BTN_ADD_DIMOND);
		m_pMenu->addChild(_menuItem);
    }
#endif
	updateBonusImg(gameStatus->getNxtBonusType());
    
#ifdef CS_ENBALE_PARTICLE
    {
        auto particle = ParticleSystemQuad::create(DIR_PARTICLE"snow.plist");
        particle->setAutoRemoveOnFinish(false);
        particle->setPosition(Vec2(getContentSize().width / 2, getContentSize().height));
        this->addChild(particle, 1000);
    }
#endif
}



void GameTitleNode::onStageDataChanged(StageUserData* data) {
    unsigned int dirtyMask = data->getDirtyMask();
    if (dirtyMask & DIRTY_BIT_BONUS_STAR) {
        updateBonusLabel(data->getBonusNeedStar() - data->getBonusStar());
    }
    if (dirtyMask & DIRTY_BIT_BONUS_TYPE) {
        updateBonusImg(data->getNxtBonusType());
    }
    if (dirtyMask & DIRTY_BIT_CURSCORE) {
        updateCurScore(data->getCurScore());
    }
}
static long last_clock;
void GameTitleNode::onMenuCallback(cocos2d::Ref* sender) {
    MenuItem* _menuItem = static_cast<MenuItem*>(sender);
    UserInfo* userInfo = UserInfo::Inst();
    switch (_menuItem->getTag()) {
        case TAG_BTN_PAUSE:
            cocos2d::log("popup dialog %ld", last_clock = clock());
            this->pauseGame();
            break;
        case TAG_BTN_RESET:
            if (m_callback && userInfo->getDiamonds() > ITEM_COST_DIAMOND) {
                userInfo->addDiamonds(-ITEM_COST_DIAMOND);
				m_callback(GAME_TITLE_EVT_RESET_BONUS, nullptr);
			}
            break;
        case TAG_BTN_REFRESH:
            if (m_callback && userInfo->getDiamonds() > ITEM_COST_DIAMOND) {
                userInfo->addDiamonds(-ITEM_COST_DIAMOND);
				m_callback(GAME_TITLE_EVT_REFRESH_MAP, nullptr);
			}
            break;
        case TAG_BTN_ADD_DIMOND:
            break;
        default:
            break;
    }
}
void GameTitleNode::pauseGame() {
    if (m_pPauseBoard) {
        m_pPauseBoard->removeFromParent();
    }
    m_pPauseBoard = PauseBoard::create();
    m_pPauseBoard->setPosition(Point(320, 480));
    m_pPauseBoard->setScale(480/m_pPauseBoard->getContentSize().width);
    this->addChild(m_pPauseBoard, 100);
    m_pPauseBoard->addBoardClkListener([this](PauseBoard::enBoardItem what) {
        switch (what)
        {
			case PauseBoard::enItemClose:
			case PauseBoard::enItemRight:
			case PauseBoard::enItemSure:
				this->resumeGame();
				break;
			case PauseBoard::enItemLeft:
				this->resumeGame();
                StageEntity::Inst()->endRound();
				break;
			default:
				break;
        }
    });
    
	Director::getInstance()->pause();
    cocos2d::log("popup ok %ld", clock() - last_clock);
}
void GameTitleNode::resumeGame() {
	Director::getInstance()->resume();
	if (m_pPauseBoard) {
		m_pPauseBoard->removeFromParent();
		m_pPauseBoard = NULL;
	}
}

void GameTitleNode::updateBonusLabel(int leftBonusStar) {
	if (m_nNextBonusNeedStar != leftBonusStar) {
		m_nNextBonusNeedStar = leftBonusStar;
		char buf[260]; sprintf(buf, "%d", m_nNextBonusNeedStar);
		m_lbNxtBonus->setString(buf);
	}
}
void GameTitleNode::updateBonusImg(int bonusType) {
    auto newBonus = BeadFactory::createBead((enBeadCategory)bonusType);
    if (m_spBonus == nullptr) {
        m_spBonus = newBonus;
        m_spBonus->setPosition(Vec2(375 + 200, 765));
        this->addChild(m_spBonus);
    }
    else
        m_spBonus->setSpriteFrame(newBonus->getSpriteFrame());
}
void GameTitleNode::updateCurScore(int curScore) {
	if (m_nCurScore != curScore ) {
		m_nCurScore = curScore;
        char buf[100]; sprintf(buf, "%d", m_nCurScore);
        m_lbCurScore->setString(buf);
	}
}
void GameTitleNode::updateStageIdxLabel(int curRound) {
	m_curStageId = curRound;
	char buf[100]; sprintf(buf, "%d", m_curStageId);
	m_lbCurStage->setString(buf);
}
void GameTitleNode::updateNeedScoreLabel(int needScore) {
	m_nNeedScore = needScore;
	char buf[100]; sprintf(buf, "%d", m_nNeedScore);
	m_lbNeedScore->setString(buf);
}


Point GameTitleNode::getStandedPos(enStandPos stand)
{
	Point pos = Point::ZERO;
	switch (stand)
	{
	case GameTitleNode::enStartMid1:
		pos = Point(320, 530);
		break;
	case GameTitleNode::enStartMid2:
		pos = Point(320, 450);
		break;
	case GameTitleNode::enStartLft1:
		pos = Point(-50, 530);
		break;
	case GameTitleNode::enStartLft2:
		pos = Point(-50, 450);
		break;
	default:
		break;
	}
	return pos;
}
void GameTitleNode::doLabelFlyInAnim(const std::string& text, Point posStart, Point posEnd, float showDuration, const std::function<void()>& moveok)
{
	auto label = Label::createWithSystemFont(text, "fonts/hzgb.ttf", 48, Size(500,60), TextHAlignment::CENTER); 
	label->setPosition(posStart);
	this->addChild(label);

	label->retain();
	label->runAction(Sequence::create(MoveTo::create(0.8f, posEnd),
		CallFunc::create(moveok),
		DelayTime::create(showDuration),
		CallFunc::create([=](){
			label->removeFromParent();
			label->release();
		}),
		NULL)
	);
}

#if 0
void GameTitleNode::updateTips(int nStar, int nScore) {
	Label* lbTip = dynamic_cast<Label*>(this->getChildByTag(CHILD_TIP_TAG));
	if( lbTip == NULL ) {
		{
			m_lbTip = Label::createWithSystemFont("","fonts/hzgb.ttf",32,Size(200,48),TextHAlignment::CENTER);
			m_lbTip->setPosition(Point(320,BASE_OFF_Y+130));
			m_lbTip->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
			m_lbTip->setTag(CHILD_TIP_TAG);
			this->addChild(m_lbTip,2);
		}
		lbTip = m_lbTip;
	}
	else {
		lbTip->stopActionByTag(999);
	}
	std::string str = "连消";
	char szInfo[260]; sprintf(szInfo, "%d", nStar);str = std::string(szInfo) + str;
	sprintf(szInfo, "%d", nScore);str = str + std::string(szInfo);
	lbTip->setString(str.c_str());
	Action* pAct = Sequence::create(
                                    DelayTime::create(2.0f),
                                    FadeOut::create(1.0f),
                                    CCCallFunc::create(CC_CALLBACK_0(GameTitleNode::tipDownCb,this)),
                                    NULL
                                    );
	pAct->setTag(999);
	lbTip->runAction(pAct);
}
void GameTitleNode::tipDownCb() {
	m_lbTip->removeFromParentAndCleanup(true);
}
#endif