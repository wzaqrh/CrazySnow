//
//  StringUpdater.h
//  crazy_snow
//
//  Created by qrh on 15-9-26.
//
//

#ifndef crazy_snow_StringUpdater_h
#define crazy_snow_StringUpdater_h
#include "cocos2d.h"

class StringUpdater : public cocos2d::Layer {
public:
	CREATE_FUNC(StringUpdater);
	void begin(float timer, int init_val, int end_val, cocos2d::Label* label) {
		m_label = label;
		m_timer = timer;
		m_init_val = init_val;
		m_end_val  = end_val;
        
		char szInfo[260]; sprintf(szInfo, "%d", m_init_val);
		m_label->setString(szInfo);
        
		this->schedule(schedule_selector(StringUpdater::do_update), timer);
	}
	void do_update(float) {
		if( m_init_val >= m_end_val ) {
			unschedule(schedule_selector(StringUpdater::do_update));
			this->removeFromParentAndCleanup(true);
		}
		else {
			m_init_val++;
			char szInfo[260]; sprintf(szInfo, "%d", m_init_val);
			m_label->setString(szInfo);
		}
	}
private:
	cocos2d::Label* m_label;
    
	float m_timer;
	int   m_init_val;
	int   m_end_val;
};

#endif
