#include "SystemManager.h"
#include "EntityManager.h"




void SystemManager::init(EntityManager& em, EventManager &event) 
{
    EntityManager::get().make();
    for(auto &s : systems) {

        s.second->init(em, event);
    }
    em.make();
}

void SystemManager::Free()
{
    systems.clear();
}

void SystemManager::update(float dt, EntityManager& em, EventManager &event)
{
    for(auto &s : systems) {
        

        s.second->pre_update(em);
        s.second->update(dt, em, event);
        s.second->over();
        em.make();
    }
}
