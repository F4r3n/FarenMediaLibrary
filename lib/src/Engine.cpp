#include "Engine.h"
#include "RenderingSystem.h"
#include "Window.h"
#include "Time.h"
#include "PhysicSystem.h"
#include "RenderingSystem.h"
#include "ScriptManagerSystem.h"
#include "Components/CTransform.h"
#include "SoundSystem.h"
using namespace fm;
Engine::Engine() {
    systems = SystemManager(SYSTEMS::LAST_SYSTEM);
}

Engine::~Engine() {
}

void Engine::run(Window& window) {
    while(!window.isClosed()) {

        window.update(60);
        // std::cout << 1 / fm::Time::dt << std::endl;
        // auto start = std::chrono::system_clock::now();
        update(fm::Time::dt);

        window.swapBuffers();

        // auto end = std::chrono::system_clock::now();
        // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        // float time = elapsed.count();
        // std::cout << "Time " << 1/fm::Time::dt << '\n';
    }
}

void Engine::start() {
    systems.addSystem((unsigned int)PHYSIC, std::make_shared<fms::PhysicSystem>());
    systems.addSystem((unsigned int)SCRIPTER, std::make_shared<fms::ScriptManagerSystem>());

    systems.getSystem(PHYSIC)->init(EntityManager::get(), EventManager::get());
    systems.getSystem(SCRIPTER)->init(EntityManager::get(), EventManager::get());
}

void Engine::init() {
    systems.addSystem(SOUND, std::make_shared<fms::SoundSystem>());

    Entity* cam = fm::Engine::createEntity();
    cam->addComponent<fmc::CCamera>(new fmc::CCamera(800, 600));
    cam->addComponent<fmc::CTransform>();
    std::shared_ptr<fms::RenderingSystem> renderer =
        std::make_shared<fms::RenderingSystem>(fm::Window::width, fm::Window::height);
    renderer->addCamera(cam);
    systems.addSystem((unsigned int)RENDERER, renderer);

    // dynamic_cast<fms::RenderingSystem*>(systems[RENDERER])->addCamera(cam);
    systems.init(EntityManager::get(), EventManager::get());
}

void Engine::stop() {
    hasStopped = true;
}
void Engine::resume() {
    hasStopped = false;
}
void Engine::reset() { 
    systems.addSystem((unsigned int)PHYSIC, std::make_shared<fms::PhysicSystem>());
    systems.addSystem((unsigned int)SCRIPTER, std::make_shared<fms::ScriptManagerSystem>());
}

void Engine::update(float dt) {
    if(hasStopped)
        systems.update(0, EntityManager::get(), EventManager::get());
    else systems.update(dt, EntityManager::get(), EventManager::get());
}

Entity* Engine::createEntity() {
    return EntityManager::get().createEntity();
}