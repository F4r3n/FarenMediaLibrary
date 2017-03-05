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
using namespace fm;
Engine::Engine() {
}

Engine::~Engine() {
    std::cout << Profiler::get().getTime("Test") << " ms" << std::endl;
}

void Engine::run(Window& window) {
    fm::Window::setMSAA(4);

    auto start = std::chrono::system_clock::now();

    while(!window.isClosed()) {

        window.update(60);
        {
          //  PROFILER_MEASURE(TEST)
            update(fm::Time::dt);
        }
        //PROFILER_DISPLAY(TEST)
        window.swapBuffers();

        numberFramesTimer++;
        if(numberFramesTimer == 200) {
            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            float time = elapsed.count() / (float)numberFramesTimer;
            start = end;
            std::cout << "Time per frame " << time << " ms" << std::endl;
            numberFramesTimer = 0;
        }
    }
}

void Engine::start() {
    systems.addSystem(new fms::PhysicSystem());
    systems.addSystem(new fms::ScriptManagerSystem());

    systems.getSystem<fms::PhysicSystem>()->init(EntityManager::get(), EventManager::get());

    systems.getSystem<fms::ScriptManagerSystem>()->init(EntityManager::get(), EventManager::get());
}

void Engine::setMainCamera() {
    systems.getSystem<fms::RenderingSystem>()->setCamera(camera);
}

void Engine::init() {
    systems.addSystem(new fms::SoundSystem());

    camera = fm::Engine::createEntity();
    fmc::CCamera* cam = camera->addComponent<fmc::CCamera>(new fmc::CCamera(fm::Window::width, fm::Window::height));
    camera->addComponent<fmc::CTransform>();
    fmc::CIdentity* identity = camera->addComponent<fmc::CIdentity>();
    identity->name = "Camera";
    cam->setNewViewPort(0, 0, fm::Window::width, fm::Window::height);
    cam->shader_data.render_mode = fmc::RENDER_MODE::DEFERRED;
    fms::RenderingSystem* renderer = systems.addSystem(new fms::RenderingSystem(fm::Window::width, fm::Window::height));
    renderer->setCamera(camera);
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

Entity* Engine::createEntity() {
    return EntityManager::get().createEntity();
}