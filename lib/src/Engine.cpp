#include "Engine.h"
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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
using namespace fm;
Engine::Engine()
{
}

Engine::~Engine()
{
}



void Engine::Run(Window& window)
{
    fm::Window::setMSAA(4);
    //auto start = std::chrono::system_clock::now();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(loop,&window, 0, true);
#else
    while(!window.isClosed()) {

        RunMainLoop(&window);
    }
#endif

    
}

void Engine::RunMainLoop(void* window)
{
    //auto start = std::chrono::system_clock::now();
    ((Window *)window)->update(120);
    //std::cout << fm::Time::dt << std::endl;
    Update(fm::Time::dt);

    ((Window *)window)->swapBuffers();

    _numberFramesTimer++;
    if(_numberFramesTimer == 200) {
        // auto end = std::chrono::system_clock::now();
        // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        // float time = elapsed.count() / (float)_numberFramesTimer;
        // start = end;
        // std::cout << "Time per frame " << time << " ms" << std::endl;
        //_numberFramesTimer = 0;
    }
    
}

void Engine::Start()
{

#ifdef PHYSIC_SYSTEM
    _systems.addSystem(new fms::PhysicSystem());
    _systems.getSystem<fms::PhysicSystem>()->init(EntityManager::get(), EventManager::get());
#endif

#ifdef SCRIPT_SYSTEM
    _systems.addSystem(new fms::ScriptManagerSystem());
    _systems.getSystem<fms::ScriptManagerSystem>()->init(EntityManager::get(), EventManager::get());
#endif
}

fm::GameObject* Engine::GetMainCamera()
{
    return _mainCamera;
}

void Engine::SetMainCamera(fm::GameObject *go)
{
    _mainCamera = go;
    EntityManager::get().make();
    _systems.getSystem<fms::RenderingSystem>()->setCamera(EntityManager::get().getEntity(go->getID()));
}

void Engine::Init()
{
    _systems.addSystem(new fms::SoundSystem());

    _systems.addSystem(new fms::ScriptManagerSystem());
    _systems.getSystem<fms::ScriptManagerSystem>()->init(EntityManager::get(), EventManager::get());

    //fms::RenderingSystem* renderer = systems.addSystem(new fms::RenderingSystem(fm::Window::width, fm::Window::height));
    _systems.addSystem(new fms::RenderingSystem(fm::Window::width, fm::Window::height));

    //renderer->setCamera(camera);

    _systems.init(EntityManager::get(), EventManager::get());
}

void Engine::Stop()
{
    fm::Time::scale = 0;
}

void Engine::Resume()
{
    fm::Time::scale = 1;
}

void Engine::Reset()
{
    _systems.addSystem(new fms::PhysicSystem());
    _systems.addSystem(new fms::ScriptManagerSystem());
}

void Engine::Update(float dt)
{
  //  auto start = std::chrono::system_clock::now();

    _systems.update(dt * Time::scale, EntityManager::get(), EventManager::get());
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

