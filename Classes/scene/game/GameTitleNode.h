#pragma once
#include "common/CommonDef.h"
#include "graph/GraphCommDef.h"

class PauseBoard;
class TileMapLayer;
class LabelNode;
class StageUserData;

enum GameTitleNodeEventType {
    GAME_TITLE_EVT_RESET_BONUS,
    GAME_TITLE_EVT_REFRESH_MAP,
    GAME_TITLE_EVT_USE_HAMMER
};

typedef std::function<void(int, void*)> GameTitleEventCallback;
typedef std::function<void()> GameTitleInitCallback;
class GameTitleNode
    : public cocos2d::Layer
{
public:
	enum enStandPos {
		enStartMid1,
		enStartMid2,
		enStartLft1,
		enStartLft2
	};
public:
    CREATE_FUNC(GameTitleNode);
	GameTitleNode();
	~GameTitleNode();
public:
	void setEventHandler(const GameTitleEventCallback& callback);
    
	void enter(const GameTitleInitCallback& onInitOk);
	void updateCurScore(int curScore);
	void updateStageIdxLabel(int curRound);
	void updateNeedScoreLabel(int needScore);
	void updateTips(int nStar, int nScore);
	void updateBonusLabel(int leftBonusStar);
	void updateBonusImg(int bonusType);

	void pauseGame();
	void resumeGame();
private:
	cocos2d::Vec2 getStandedPos(enStandPos stand);
	void doLabelFlyInAnim(const std::string& text, cocos2d::Vec2 posStart, cocos2d::Vec2 posEnd, float showDuration, const std::function<void()>& moveok);
	void tipDownCb();
    void onMenuCallback(cocos2d::Ref* sender);
    void onStageDataChanged(StageUserData* stageUserData);
private:
    LabelNode* m_lbCurStage;
	LabelNode* m_lbNeedScore;
	LabelNode* m_lbCurScore;
    
    LabelNode* m_curDiamond;
    LabelNode* m_lbNxtBonus;
    
    cocos2d::Sprite* m_spBonus;
    
	cocos2d::Label* m_lbTip;
	PauseBoard*		m_pPauseBoard;
    
	cocos2d::EventListenerTouchOneByOne*  m_pTouchListener;
	GameTitleEventCallback m_callback;
private:
	int m_nCurScore;
	int m_nNeedScore;
	int m_curStageId;
	int m_nNextBonusNeedStar;
	cocos2d::Menu* m_pMenu;
};


