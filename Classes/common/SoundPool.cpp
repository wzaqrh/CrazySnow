//
//  SoundPool.cpp
//  crazy_snow
//
//  Created by qrh on 15-9-29.
//
//

#include "SoundPool.h"
#include "AudioEngine.h"
#include "data/UserInfo.h"

#define DIR_SOUND "sound/"

using namespace cocos2d;
//using namespace experimental;
SoundPool::SoundPool() {
    experimental::AudioEngine::setMaxAudioInstance(80);
}
int SoundPool::play(const std::string& filePath, bool loop, float volume) {
    int audioId = 0;
    if (UserInfo::Inst()->isSoundEnable()) {
        audioId = experimental::AudioEngine::play2d(filePath, loop, volume);
    }
    return audioId;
}
void SoundPool::setEnable(bool enable) {
    if (! enable) {
        experimental::AudioEngine::stopAll();
    }
    else {
        for (auto iter : m_entry) {
            SoundEntry& entry = iter.second;
            entry.audioId = experimental::AudioEngine::play2d(entry._filePath, entry._loop, entry._volume);
        }
    }
}
void SoundPool::addEntry(const std::string& filePath, bool loop, float volume) {
    int audioId = play(DIR_SOUND"rain.mp3", true, 0.7f);
    
    SoundEntry entry = {filePath, loop, volume, audioId};
    auto iter = m_entry.find(filePath);
    if (iter != m_entry.end()) {
        experimental::AudioEngine::stop(iter->second.audioId);
        iter->second = entry;
    }
    else
        m_entry.insert(std::make_pair(filePath, entry));
}
void SoundPool::playBackGroundMusic() {
    addEntry(DIR_SOUND"rain.mp3", true, 0.7f);
    addEntry(DIR_SOUND"wind.mp3", true, 0.3f);
}
void SoundPool::playEffect(enSoundEffectType effectType) {
    struct SoundEffect {
        const char* name;
        float volume;
    };
    static SoundEffect effectName[SOUND_EFFECT_MAX] = {
        {DIR_SOUND"hit.wav", 1.0f},
        {DIR_SOUND"bonus.wav", 0.5f},
        {DIR_SOUND"down.wav", 1.0f},
    };
    play(effectName[effectType].name, false, effectName[effectType].volume);
}

void SoundPool::pauseAll() {
    experimental::AudioEngine::pauseAll();
}
void SoundPool::resumeAll() {
    if (UserInfo::Inst()->isSoundEnable()) {
        experimental::AudioEngine::resumeAll();
    }
}

