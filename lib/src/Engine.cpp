#include "Engine.h"
#include "RenderingSystem.h"
using namespace fm;
Engine::Engine() {
    //systems.addSystem();
}

Engine::~Engine() {
    
}

void Engine::update(float dt) {
    systems.update(dt, EntityManager::get());
}

std::shared_ptr<Entity> Engine::createEntity() {
    return EntityManager::get().createEntity();
}