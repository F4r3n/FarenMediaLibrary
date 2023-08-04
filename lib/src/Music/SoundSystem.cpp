#include "Music/SoundSystem.h"
#include "Music/Speaker.h"
#include "Music/Listener.h"
#include <memory>
#include "Components/CTransform.h"
#include "Core/Transform.h"
#include "Components/CSource.h"
#include <AL/al.h>
#include <AL/alc.h>
#include "Engine.h"
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

void SoundSystem::update(float , EntityManager& em, EventManager& ) {
    int error = 0;
    for(auto &&e : em.iterate<fmc::CTransform, fmc::CSource>(fm::IsEntityActive))
	{
        if((error = alGetError()) != 0) 
		{
            //std::cerr << "Error openal" << std::endl;
            return;
        }
        fmc::CSource* sound = e.get<fmc::CSource>();
        fmc::CTransform* transform = e.get<fmc::CTransform>();
        if(sound->toUpdate) 
		{
            _SetSettings(transform->GetTransform(), sound);
        }
    }
}

void SoundSystem::_SetSettings(const fm::Transform &inTransform, fmc::CSource* sound) {
    alSourcef(sound->source, AL_PITCH, sound->pitch);
    alSourcef(sound->source, AL_GAIN, sound->volume);
    alSource3f(sound->source, AL_POSITION, inTransform.position.x, inTransform.position.y, 0);
    alSource3f(sound->source, AL_VELOCITY, 0, 0, 0);
    alSourcei(sound->source, AL_LOOPING, sound->isLooping);
}

void SoundSystem::init(EntityManager& em, EventManager& event)
{
	_speaker = std::unique_ptr<fm::Speaker>(new fm::Speaker());
	_listener = std::unique_ptr<fm::Listener>(new fm::Listener());

    for(auto &&e : em.iterate<fmc::CTransform, fmc::CSource>(fm::IsEntityActive)) {
        fmc::CSource* sound = e.get<fmc::CSource>();
        fmc::CTransform* transform = e.get<fmc::CTransform>();
       

        _SetSettings(transform->GetTransform(), sound);
    }
}
void SoundSystem::over() {
}