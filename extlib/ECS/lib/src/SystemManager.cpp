#include "SystemManager.h"
#include "EntityManager.h"

float currentTime = 0;

void call_back_init(Entity* e) {
   // currentSystem->init(e);
}

void call_back_update(Entity* e) {
    //currentSystem->update(currentTime, e);
}

void SystemManager::init(EntityManager& em, EventManager &event) {
    EntityManager::get().make();
    for(auto &s : systems) {
        //currentSystem = s.get();
        //em.getEntitiesWithComponents(&call_back_init, currentSystem->getMask());
        s.second->init(em, event);
    }
    em.make();
}

void SystemManager::update(float dt, EntityManager& em, EventManager &event) {
    currentTime = dt;
    for(auto &s : systems) {
        

        s.second->pre_update(em);
        s.second->update(dt, em, event);
        s.second->over();
        em.make();
    }
}