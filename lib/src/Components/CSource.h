#pragma once

#include "Core/Math/Vector2.h"
#include "component.h"

namespace fmc {
enum AUDIO_STATUS { INITIAL, PLAYING, PAUSED, STOPPED };

class CSource : public FMComponent<CSource> {
    public:
        CSource(const std::string& path);
        CSource();
        ~CSource();
        void play();

        float pitch = 1.0f;
        float volume = 1.0f;
        bool isLooping = false;

        unsigned int source;
        unsigned int buf;

        bool toUpdate = false;

    private:
};
}
