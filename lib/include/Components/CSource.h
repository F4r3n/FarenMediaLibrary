#pragma once
#include <vector>
#include <map>
#include <AL/al.h>
#include <AL/alc.h>
#include "Core/Math/Vector2.h"
#include "component.h"

namespace fmc {
enum AUDIO_STATUS { INITIAL, PLAYING, PAUSED, STOPPED };

class CSource : public FMComponent<CSource> {
    public:
        CSource(const std::string& path);
        CSource();
        ~CSource();
        void loadAudio(const std::string& path);
        void play();
        void setPitch(float pitchValue);
        void setVolume(float volume);
        void setLoop(bool loop);
        AUDIO_STATUS getStatus();

        float pitch = 1.0f;
        float volume = 1.0f;
        bool isLooping = false;

        ALuint source;
        ALuint buf;

        bool toUpdate = false;
        static const std::string name;
    private:
};
}
