#include "Engine.h"
#include "Time.h"

#include "Physic/PhysicSystem.h"
#include "Rendering/RenderingSystem.h"
#include "Script/ScriptManagerSystem.h"

#include "Music/SoundSystem.h"
#include "Components/CIdentity.h"

#include "Profiler/Profile.hpp"
#include "Profiler/Profiler.hpp"
#include "Profiler/ProfilerMacro.h"
#include "Core/Config.h"
#include "Core/GameObject.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
using namespace fm;
Engine::Engine() {
}

Engine::~Engine() {
}



void Engine::run(Window& window)
{
    fm::Window::setMSAA(0);
    //auto start = std::chrono::system_clock::now();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(loop,&window, 0, true);
#else
    while(!window.isClosed()) {

        loop(&window);
    }
#endif

    
}

void Engine::loop(void* window) {
    ((Window *)window)->update(60);
    //std::cout << fm::Time::dt << std::endl;
    update(fm::Time::dt);

    ((Window *)window)->swapBuffers();

    numberFramesTimer++;
    if(numberFramesTimer == 200) {
        // auto end = std::chrono::system_clock::now();
        // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        // float time = elapsed.count() / (float)numberFramesTimer;
        // start = end;
        // std::cout << "Time per frame " << time << " ms" << std::endl;
        numberFramesTimer = 0;
    }
    
}

void Engine::start() {

#ifdef PHYSIC_SYSTEM
    systems.addSystem(new fms::PhysicSystem());
    systems.getSystem<fms::PhysicSystem>()->init(EntityManager::get(), EventManager::get());
#endif

#ifdef SCRIPT_SYSTEM
    systems.addSystem(new fms::ScriptManagerSystem());
    systems.getSystem<fms::ScriptManagerSystem>()->init(EntityManager::get(), EventManager::get());
#endif
}

fm::GameObject* Engine::getMainCamera() {
    return mainCamera;
}

void Engine::setMainCamera(fm::GameObject *go) {
    mainCamera = go;
    EntityManager::get().make();
    systems.getSystem<fms::RenderingSystem>()->setCamera(EntityManager::get().getEntity(go->getID()));
}

void Engine::init() {
    systems.addSystem(new fms::SoundSystem());

    systems.addSystem(new fms::ScriptManagerSystem());
    systems.getSystem<fms::ScriptManagerSystem>()->init(EntityManager::get(), EventManager::get());

    //camera = fm::Engine::createEntity();
    /*fmc::CCamera* cam = camera->addComponent<fmc::CCamera>(new fmc::CCamera(fm::Window::width, fm::Window::height, fmc::RENDER_MODE::FORWARD));
    camera->addComponent<fmc::CTransform>();
    fmc::CIdentity* identity = camera->addComponent<fmc::CIdentity>();
    identity->name = "Camera";
    //cam->shader_data.render_mode = fmc::RENDER_MODE::DEFERRED;*/

    //fms::RenderingSystem* renderer = systems.addSystem(new fms::RenderingSystem(fm::Window::width, fm::Window::height));
    systems.addSystem(new fms::RenderingSystem(fm::Window::width, fm::Window::height));

    //renderer->setCamera(camera);

    systems.init(EntityManager::get(), EventManager::get());
}

void Engine::stop() {
    fm::Time::scale = 0;
}

void Engine::resume() {
    fm::Time::scale = 1;
}

void Engine::reset() {
    systems.addSystem(new fms::PhysicSystem());
    systems.addSystem(new fms::ScriptManagerSystem());
}

void Engine::update(float dt) {
    systems.update(dt * Time::scale, EntityManager::get(), EventManager::get());
}

