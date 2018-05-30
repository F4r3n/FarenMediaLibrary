#include "Script/ScriptManagerSystem.h"
#include "Script/LuaManager.h"
#include "Components/CScriptManager.h"
#include "Script/cppscript.hpp"
#include "Script/cppmanager.hpp"
using namespace fms;
ScriptManagerSystem::ScriptManagerSystem() {
    
    LuaManager::get().openLibraries();
    LuaManager::get().registerComponents();
    CPPManager::get().LoadLibrary();
}



void ScriptManagerSystem::receive(const Collider& collider) {

    processCollision(collider.idA, collider.idB, collider.event);
    processCollision(collider.idB, collider.idA, collider.event);
}


void ScriptManagerSystem::receive(const CameraInfo &cameraInfo) {
    Entity* e = EntityManager::get().getEntity(cameraInfo.ID);
     
    if(e && e->has<fmc::CScriptManager>()) {
        fmc::CScriptManager* scriptManager = e->get<fmc::CScriptManager>();
        scriptManager->event("OnRenderObject", cameraInfo);
    }
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
    event.subscribe<Collider>(*this);
    event.subscribe<CameraInfo>(*this);
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
