//
//  StateMachine.cpp
//  crazy_snow
//
//  Created by qrh on 15-9-26.
//
//

#include "StateMachine.h"

StateMachine::StateMachine()
:m_curState(nullptr)
{}

inline void deleteStage(IState* state) {
    delete state;
}
StateMachine::~StateMachine() {
    std::for_each(m_states.begin(), m_states.end(), deleteStage);
}

void StateMachine::init() {
    m_states.assign(STATE_MAX, NULL);
    m_states[STATE_NORMAL]      = new StateNormal;
    m_states[STATE_USE_ROOK]    = new StateRook;
    m_states[STATE_USE_QUEEN]   = new StateQueen;
    m_states[STATE_USE_ELEPH]   = new StateEleph;
    m_states[STATE_USE_ARTILLERY]   = new StateArtillery;
    m_states[STATE_USE_SOLDIER]     = new StateSoldier;
    
    m_curState = m_states[STATE_NORMAL];
}
void StateMachine::onEvent(const StateMachineEvent& event) {
    switch (m_curState->getState()) {
        case STATE_NORMAL:
            m_states[m_curState->onEvent(event)];
            break;
        case STATE_USE_ROOK:
        case STATE_USE_QUEEN:
        case STATE_USE_ELEPH:
        case STATE_USE_ARTILLERY:
        case STATE_USE_SOLDIER:
            m_curState = m_states[m_curState->onEvent(event)];
            break;
        default:
            break;
    }
}