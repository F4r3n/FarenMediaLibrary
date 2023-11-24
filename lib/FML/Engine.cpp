#include "Engine.h"
#include <Window.h>
#include <TimeDef.h>

#include "Physic/PhysicSystem.h"
#include "Rendering/OpenGL/OGLRenderingSystem.h"
#include "Script/ScriptManagerSystem.h"

#include "Music/SoundSystem.h"

#include "Profiler/Profile.hpp"
#include "Profiler/Profiler.hpp"
#include "Profiler/ProfilerMacro.h"
#include "Core/Config.h"
#include "Core/GameObject.h"
#include "Physic/PhysicSystem.h"
#include "Components/cevent.hpp"
#include "Components/CIdentity.h"
#if WITH_VULKAN
#include "Rendering/Vulkan/VkRenderingSystem.hpp"
#endif
#include "Core/System/SystemManager.hpp"
using namespace fm;
#include "Core/Scene.h"
#include "Core/System/Event.h"


Engine::Engine()
{
}

Engine::~Engine()
{
}





void Engine::Init(RENDERING_MODE inMode, std::array<std::shared_ptr<fm::Window>, (int)GRAPHIC_API::LAST> window)
{
    _systems.emplace_back(new fms::SoundSystem());
	_systems.emplace_back(new fms::PhysicSystem());

	_systems.emplace_back(new fms::ScriptManagerSystem());
	if ((inMode & RENDERING_MODE_OPENGL) == RENDERING_MODE_OPENGL)
	{
		_systems.emplace_back(new fms::OGLRenderingSystem(window[GRAPHIC_API::OPENGL]));
	}
#if WITH_VULKAN
	if ((inMode & RENDERING_MODE_VULKAN) == RENDERING_MODE_VULKAN)
	{
		_systems.emplace_back(new fms::VkRenderingSystem(window[GRAPHIC_API::VULKAN]));
	}
#endif

	for (const auto& system : _systems)
	{
		system->init(EventManager::Get());
	}
}

void Engine::Stop(std::shared_ptr<fm::Scene> inScene)
{
	for (const auto& system : _systems)
	{
		system->Stop(inScene->GetRegistry());
	}
	_running = false;
}

void Engine::Start(std::shared_ptr<fm::Scene> inScene)
{
	_running = true;
	for (const auto& system : _systems)
	{
		system->Start(inScene->GetRegistry());
	}
}

void Engine::Resume()
{
    fm::Time::scale = 1;
}

void Engine::Reset()
{
}

void Engine::Update(float dt, std::shared_ptr<fm::Scene> inScene)
{
	for (const auto& system : _systems)
	{
		system->update(dt * Time::scale, inScene->GetRegistry(), EventManager::Get());
	}
}

