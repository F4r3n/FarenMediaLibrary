#pragma once

struct ALCdevice;
struct ALCcontext;

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