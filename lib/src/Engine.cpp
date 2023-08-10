#include <ECS.h>
#include "Engine.h"
#include <Window.h>
#include <TimeDef.h>

#include "Physic/PhysicSystem.h"
#include "Rendering/RenderingSystem.h"
#include "Script/ScriptManagerSystem.h"

#include "Music/SoundSystem.h"

#include "Profiler/Profile.hpp"
#include "Profiler/Profiler.hpp"
#include "Profiler/ProfilerMacro.h"
#include "Core/Config.h"
#include "Core/GameObject.h"
#include "Physic/PhysicSystem.h"
#include "Components/cevent.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "Components/CIdentity.h"
#include "Rendering/Vulkan/VkRenderingSystem.h"
using namespace fm;

class GarbageCollector
{
public:
	static void Collect()
	{
		for (auto && e : EntityManager::get().iterate<fmc::CEvent>())
		{
			fmc::CEvent* event = e.get<fmc::CEvent>();
			event->Clear();
		}
	}
};


Engine::Engine()
{
	_systems = std::unique_ptr<SystemManager>(new SystemManager());
}

Engine::~Engine()
{
	EntityManager::get().Free();
}



void Engine::Start()
{
	_systems->Start();
}

SYSTEM_MANAGER_MODE Engine::GetStatus() const 
{
	return _systems->GetStatus(); 
}


void Engine::Init(GRAPHIC_API inAPI, std::shared_ptr<fm::Window> window)
{
    _systems->addSystem(new fms::SoundSystem());
	_systems->addSystem(new fms::PhysicSystem());

    _systems->addSystem(new fms::ScriptManagerSystem());
	if (inAPI == GRAPHIC_API::OPENGL)
	{
		auto size = window->GetSize();
		_systems->addSystem(new fms::RenderingSystem(size.x, size.y));
	}
	else if (inAPI == GRAPHIC_API::VULKAN)
	{
		_systems->addSystem(new fms::VkRenderingSystem(window));
	}

    _systems->init(EntityManager::get(), EventManager::Get());
}

void Engine::Stop()
{
	_systems->Stop();
}

void Engine::Resume()
{
    fm::Time::scale = 1;
}

void Engine::Reset()
{
    _systems->addSystem(new fms::ScriptManagerSystem());
}

void Engine::Update(float dt)
{
  //  auto start = std::chrono::system_clock::now();

    _systems->update(dt * Time::scale, EntityManager::get(), EventManager::Get());
	GarbageCollector::Collect();
//_numberFramesTimer++;
    //if(_numberFramesTimer == 200) {
       //  auto end = std::chrono::system_clock::now();
       //  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
       //  float time = elapsed.count();
       //  start = end;
       //  std::cout << "Time per frame " << time << " ms" << std::endl;
       // _numberFramesTimer = 0;
    //}
}

bool fm::IsEntityActive(EntityManager& em, const Entity::Id& id)
{
	Entity e = em.GetEntity(id);
	fmc::CIdentity* identity = e.get<fmc::CIdentity>();
	if (identity != nullptr && identity->IsActive())
		return true;
	if (identity == nullptr)
		return true;

	return false;
}
