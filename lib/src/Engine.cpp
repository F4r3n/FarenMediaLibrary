#include "Engine.h"
#include "RenderingSystem.h"
using namespace fm;
Engine::Engine() {
    //systems.addSystem();
}

Engine::~Engine() {
    
}

void Engine::init() {
    systems.init(EntityManager::get());
}

void Engine::update(float dt) {
    systems.update(dt, EntityManager::get());
}

Entity* Engine::createEntity() {
    return EntityManager::get().createEntity();
}