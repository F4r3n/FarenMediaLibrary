#include "Engine.h"
#include "RenderingSystem.h"
#include "Window.h"
#include "Time.h"
#include "PhysicSystem.h"
#include "RenderingSystem.h"
#include "ScriptManagerSystem.h"
#include "CTransform.h"

using namespace fm;
Engine::Engine() {
    // systems.addSystem();
}

Engine::~Engine() {
}

void Engine::run(Window& window) {
    while(!window.isClosed()) {
       

        window.update(60);
        std::cout << 1 / fm::Time::dt << std::endl;
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

    Entity* cam = fm::Engine::createEntity();
    cam->addComponent<fmc::CCamera>(new fmc::CCamera(800, 600));
    cam->addComponent<fmc::CTransform>();

    std::shared_ptr<fms::RenderingSystem> rendering = std::make_shared<fms::RenderingSystem>(800, 600);
    std::shared_ptr<fms::ScriptManagerSystem> scriptSystem = std::make_shared<fms::ScriptManagerSystem>();

    rendering->addCamera(cam);
    addSystem(scriptSystem);
    addSystem(rendering);
    systems.init(EntityManager::get(), EventManager::get());
}

void Engine::update(float dt) {
    systems.update(dt, EntityManager::get(), EventManager::get());
}

Entity* Engine::createEntity() {
    return EntityManager::get().createEntity();
}