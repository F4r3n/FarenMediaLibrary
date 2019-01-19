#include "Music/SoundSystem.h"
#include "Music/Speaker.h"
#include "Music/Listener.h"
#include <memory>
#include "Components/CTransform.h"
#include "Components/CSource.h"

//TODO WTF REMAKE ALL
using namespace fms;
SoundSystem::SoundSystem()
{
	_speaker = nullptr;
	_listener = nullptr;
}
SoundSystem::~SoundSystem() {
}
void SoundSystem::pre_update(EntityManager& em) {
}

void SoundSystem::update(float dt, EntityManager& em, EventManager& event) {
    int error = 0;
    for(auto e : em.iterate<fmc::CTransform, fmc::CSource>()) 
	{
        if((error = alGetError()) != 0) 
		{
            //std::cerr << "Error openal" << std::endl;
            return;
        }
        fmc::CSource* sound = e->get<fmc::CSource>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        if(sound->toUpdate) 
		{
            setSettings(transform, sound);
        }
    }
}

void SoundSystem::setSettings(fmc::CTransform* transform, fmc::CSource* sound) {
    alSourcef(sound->source, AL_PITCH, sound->pitch);
    alSourcef(sound->source, AL_GAIN, sound->volume);
    alSource3f(sound->source, AL_POSITION, transform->position.x, transform->position.y, 0);
    alSource3f(sound->source, AL_VELOCITY, 0, 0, 0);
    alSourcei(sound->source, AL_LOOPING, sound->isLooping);
}

void SoundSystem::init(EntityManager& em, EventManager& event)
{
	_speaker = std::unique_ptr<fm::Speaker>(new fm::Speaker());
	_listener = std::unique_ptr<fm::Listener>(new fm::Listener());

    for(auto e : em.iterate<fmc::CTransform, fmc::CSource>()) {
        fmc::CSource* sound = e->get<fmc::CSource>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();
       

        setSettings(transform, sound);
    }
}
void SoundSystem::over() {
}