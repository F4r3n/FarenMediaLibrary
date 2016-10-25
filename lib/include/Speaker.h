#pragma once

#include <AL/al.h>
#include <AL/alc.h>

namespace fm {
class Speaker {
public:
    Speaker();
    ~Speaker();

private:
    ALCcontext* context;
    ALCdevice* device;
};
}