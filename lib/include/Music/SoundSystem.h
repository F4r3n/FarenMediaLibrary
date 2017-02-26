#pragma once
#include "System.h"
#include <Entity.h>

namespace fmc {
class CTransform;
class CSource;
}

namespace fms {
class SoundSystem : public System<SoundSystem> {
public:
    SoundSystem();
    ~SoundSystem();
    void pre_update(EntityManager& em);

    void update(float dt, EntityManager& em, EventManager& event);
    void init(EntityManager& em, EventManager& event);
    void over();

private:
    void setSettings(fmc::CTransform* transform, fmc::CSource* sound);
};
}