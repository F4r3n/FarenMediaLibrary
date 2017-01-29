#include "ScriptManagerSystem.h"
#include "LuaManager.h"
#include "Components/CScriptManager.h"
using namespace fms;
ScriptManagerSystem::ScriptManagerSystem() {
    
    LuaManager::get().openLibraries();
    LuaManager::get().registerComponents();
}

void ScriptManagerSystem::receive(const Collision& collision) {
    Entity* e = EntityManager::get().getEntity(collision.owner);

    if(e && e->has<fmc::CScriptManager>() && e->ID != collision.owner) {
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        scriptManager->event("CollisionEvent", collision);
    }
}

void ScriptManagerSystem::receive(const Collider& collider) {

    processCollision(collider.idA, collider.idB, collider.event);
    processCollision(collider.idB, collider.idA, collider.event);
}

void ScriptManagerSystem::processCollision(size_t idA, size_t idB, EVENT_COLLISION event) {
    Entity* e = EntityManager::get().getEntity(idB);

    if(e && e->has<fmc::CScriptManager>()) {
        ColliderInfo info;
        info.idOther = idA;
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        if(event == EVENT_COLLISION::BEGIN) {
            scriptManager->event("CollisionEventEnter", info);

        } else if(event == EVENT_COLLISION::END) {
            scriptManager->event("CollisionEventExit", info);
        }
    }
}

ScriptManagerSystem::~ScriptManagerSystem() {
}

void ScriptManagerSystem::init(EntityManager& em, EventManager& event) {
    event.subscribe<Collision>(*this);
    event.subscribe<Collider>(*this);
    for(auto e : em.iterate<fmc::CScriptManager>()) {
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        scriptManager->init(e);
    }
}

void ScriptManagerSystem::update(float dt, EntityManager& em, EventManager& event) {
    for(auto e : em.iterate<fmc::CScriptManager>()) {
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        scriptManager->update();
    }
}
