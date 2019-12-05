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
#include "Physic/PhysicSystem3D.h"
#include "Components/cevent.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
using namespace fm;

class GarbageCollector
{
public:
	static void Collect()
	{
		for (auto && e : EntityManager::get().iterate<fmc::CEvent>())
		{
			fmc::CEvent* event = e->get<fmc::CEvent>();
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
	EntityManager::get().killAll();
	EntityManager::get().Free();

}



void Engine::Run(Window& window)
{
	window.setMSAA(0);

    while(!window.isClosed()) 
	{

        RunMainLoop(&window);
    }
}

void Engine::RunMainLoop(void* window)
{
    //auto start = std::chrono::system_clock::now();
    ((Window *)window)->update(120);
    //std::cout << fm::Time::dt << std::endl;
    Update(fm::Time::dt);

    ((Window *)window)->swapBuffers();

    //_numberFramesTimer++;
    
}

void Engine::Start()
{
	_systems->Start();
}

SYSTEM_MANAGER_MODE Engine::GetStatus() const 
{
	return _systems->GetStatus(); 
}

void Engine::Init()
{
    _systems->addSystem(new fms::SoundSystem());
	_systems->addSystem(new fms::PhysicSystem3D());

    _systems->addSystem(new fms::ScriptManagerSystem());
    _systems->addSystem(new fms::RenderingSystem(fm::Window::kWidth, fm::Window::kHeight));

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
    _systems->addSystem(new fms::PhysicSystem());
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

