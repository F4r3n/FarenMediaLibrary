#include "SoundSystem.h"
#include "CTransform.h"
#include "CSource.h"

using namespace fms;
SoundSystem::SoundSystem() {
}
SoundSystem::~SoundSystem() {
}
void SoundSystem::pre_update(EntityManager& em) {
}

void SoundSystem::update(float dt, EntityManager& em, EventManager& event) {
    for(auto e : em.iterate<fmc::CTransform, fmc::CSource>()) {

        fmc::CSource* sound = e->get<fmc::CSource>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        if(sound->toUpdate) {
            setSettings(transform, sound);
        }
    }
}

void SoundSystem::setSettings(fmc::CTransform* transform, fmc::CSource* sound) {
    alGenSources((ALuint)1, &sound->source);
    alSourcef(sound->source, AL_PITCH, sound->pitch);
    alSourcef(sound->source, AL_GAIN, sound->volume);
    alSource3f(sound->source, AL_POSITION, transform->position.x, transform->position.y, 0);
    alSource3f(sound->source, AL_VELOCITY, 0, 0, 0);
    alSourcei(sound->source, AL_LOOPING, sound->isLooping);
}

void SoundSystem::init(EntityManager& em, EventManager& event) {
    for(auto e : em.iterate<fmc::CTransform, fmc::CSource>()) {
        fmc::CSource* sound = e->get<fmc::CSource>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();

        setSettings(transform, sound);
    }
}
void SoundSystem::over() {
}