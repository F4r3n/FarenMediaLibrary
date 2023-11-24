#include "Music/SoundSystem.h"
#include "Music/Speaker.h"
#include "Music/Listener.h"
#include <memory>
#include "Components/CTransform.h"
#include "Core/Transform.h"
#include "Components/CSource.h"
#include "Engine.h"
#include <entt/entt.hpp>
#include "Components/CIdentity.h"

using namespace fms;
SoundSystem::SoundSystem()
{
	_speaker = nullptr;
	_listener = nullptr;
}


SoundSystem::~SoundSystem()
{
}


void SoundSystem::update(float , entt::registry& registry, EventManager& )
{
	auto view = registry.view<fmc::CSource, fmc::CTransform, fmc::CIdentity>();
    for(auto e : view)
	{
		const auto identity = registry.get<fmc::CIdentity>(e);
		if (!identity.IsActive())
			continue;

        fmc::CSource& sound = view.get<fmc::CSource>(e);
        fmc::CTransform& transform = view.get<fmc::CTransform>(e);
        if(sound.toUpdate) 
		{
            _SetSettings(transform.GetTransform(), &sound);
        }
    }
}

void SoundSystem::_SetSettings(const fm::Transform &, fmc::CSource*) {
    //alSourcef(sound->source, AL_PITCH, sound->pitch);
    //alSourcef(sound->source, AL_GAIN, sound->volume);
    //alSource3f(sound->source, AL_POSITION, inTransform.position.x, inTransform.position.y, 0);
    //alSource3f(sound->source, AL_VELOCITY, 0, 0, 0);
    //alSourcei(sound->source, AL_LOOPING, sound->isLooping);
}

void SoundSystem::init(EventManager&)
{
	_speaker = std::unique_ptr<fm::Speaker>(new fm::Speaker());
	_listener = std::unique_ptr<fm::Listener>(new fm::Listener());
}
