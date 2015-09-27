//
//  StateMachine.h
//  crazy_snow
//
//  Created by qrh on 15-9-26.
//
//
#pragma once
#include "common/CommonDef.h"
#include "WindowStageEntityType.h"
#include "data/StageData.h"
#include "graph/GraphCommDef.h"

struct StateMachineEvent {
	enum Type {
		TouchBegin,
		TouchEnd,
		Hammer
	};
	Type    m_type;
	Point2i m_graphPos;
	float   m_duration;
public:
	StateMachineEvent(Type type, Point2i pos);
};

enum enStateType {
    STATE_NORMAL,
    STATE_USE_ROOK,
    STATE_USE_QUEEN,
    STATE_USE_ELEPH,
    STATE_USE_ARTILLERY,
    STATE_USE_SOLDIER,
    STATE_MAX
};

class IState {
public:
    virtual ~IState() {}
    virtual enStateType onEvent(StateMachineEvent event) = 0;
    virtual enStateType getState() const = 0;
};

class StateMachine {
public:
    StateMachine();
    ~StateMachine();
    void onEvent(const StateMachineEvent& event);
public:
    void init();
private:
    IState* m_curState;
    std::vector<IState*> m_states;
};

class StateNormal : public IState {
public:
    virtual enStateType onEvent(StateMachineEvent event) ;
    virtual enStateType getState() const ;
};

class StateSpecialBead : public IState {
public:
    virtual enStateType onEvent(StateMachineEvent event) ;
    virtual enStateType getState() const ;
};

class StateRook : public IState {
public:
    virtual enStateType onEvent(StateMachineEvent event) ;
    virtual enStateType getState() const ;
};

class StateQueen : public IState {
public:
    virtual enStateType onEvent(StateMachineEvent event) ;
    virtual enStateType getState() const ;
};

class StateEleph : public IState {
public:
    virtual enStateType onEvent(StateMachineEvent event) ;
    virtual enStateType getState() const ;
};

class StateArtillery : public IState {
public:
    virtual enStateType onEvent(StateMachineEvent event) ;
    virtual enStateType getState() const ;
};

class StateSoldier : public IState {
public:
    virtual enStateType onEvent(StateMachineEvent event) ;
    virtual enStateType getState() const ;
};