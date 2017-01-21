#include "Engine.h"
#include "RenderingSystem.h"
#include "Window.h"
#include "Time.h"
#include "PhysicSystem.h"
#include "RenderingSystem.h"
#include "ScriptManagerSystem.h"
#include "Components/CTransform.h"
#include "SoundSystem.h"
#include "Components/CIdentity.h"
using namespace fm;
Engine::Engine() {
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
    fmc::CCamera *cam = camera->addComponent<fmc::CCamera>(new fmc::CCamera(fm::Window::width, fm::Window::height));
    camera->addComponent<fmc::CTransform>();
    fmc::CIdentity* identity = camera->addComponent<fmc::CIdentity>();
    identity->name = "Camera";
    cam->setNewViewPort(0, 0, fm::Window::width, fm::Window::height);
    cam->shader_data.render_mode = fmc::RENDER_MODE::FORWARD;
    fms::RenderingSystem* renderer = systems.addSystem(new fms::RenderingSystem(fm::Window::width, fm::Window::height));
    renderer->setCamera(camera);
    systems.init(EntityManager::get(), EventManager::get());
}

void Engine::stop() {
    hasStopped = true;
}
void Engine::resume() {
    hasStopped = false;
}
void Engine::reset() {
    systems.addSystem(new fms::PhysicSystem());
    systems.addSystem(new fms::ScriptManagerSystem());
}

void Engine::update(float dt) {
    if(hasStopped)
        systems.update(0, EntityManager::get(), EventManager::get());
    else
        systems.update(dt, EntityManager::get(), EventManager::get());
}

Entity* Engine::createEntity() {
    return EntityManager::get().createEntity();
}