#pragma once

struct ALCdevice_struct;
struct ALCcontext_struct;

/** Opaque device handle */
typedef struct ALCdevice_struct ALCdevice;
/** Opaque context handle */
typedef struct ALCcontext_struct ALCcontext;


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