//
//  SoundPool.h
//  crazy_snow
//
//  Created by qrh on 15-9-29.
//
//

#ifndef __crazy_snow__SoundPool__
#define __crazy_snow__SoundPool__
#include <map>
#include "common/TmplHelp.h"

enum enSoundEffectType {
    SOUND_EFFECT_HIT,
    SOUND_EFFECT_BONUS,
    SOUND_EFFECT_DOWN,
    SOUND_EFFECT_MAX
};


class SoundPool :public single_base<SoundPool> {
    struct SoundEntry{
        std::string _filePath;
        bool _loop;
        float _volume;
        int audioId;
    };
public:
    SoundPool();
    int play(const std::string& filePath, bool loop = false, float volume = 1.0f);
    void setEnable(bool enable);
public:
    void pauseAll();
    void resumeAll();
    void playBackGroundMusic();
    void playEffect(enSoundEffectType effectType);
private:
    void addEntry(const std::string& filePath, bool loop, float volume);
private:
    std::map<std::string, SoundEntry> m_entry;
};

#endif /* defined(__crazy_snow__SoundPool__) */
