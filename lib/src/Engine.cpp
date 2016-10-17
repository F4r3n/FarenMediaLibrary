#include "Engine.h"
#include "RenderingSystem.h"
#include "Window.h"
#include "Time.h"
#include "PhysicSystem.h"
using namespace fm;
Engine::Engine() {
    //systems.addSystem();
}

Engine::~Engine() {
    
}

void Engine::run(Window &window) {
     while(!window.isClosed()) {

        window.update(60);

        // auto start = std::chrono::system_clock::now();
        update(fm::Time::dt);

        window.swapBuffers();

        // auto end = std::chrono::system_clock::now();
        // auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        // float time = elapsed.count();
        // std::cout << "Time " << 1/fm::Time::dt << '\n';
    }
}

void Engine::init() {
    addSystem(std::make_shared<fms::PhysicSystem>());
    systems.init(EntityManager::get(), EventManager::get());
}

void Engine::update(float dt) {
    systems.update(dt, EntityManager::get(),  EventManager::get());
}

Entity* Engine::createEntity() {
    return EntityManager::get().createEntity();
}