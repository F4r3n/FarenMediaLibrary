#include "EntityManager.h"
#include "Entity.h"
#include <cassert>
//#include <iostream>
EntityManager EntityManager::em;

EntityManager::EntityManager() {
    free_id.resize(POOL_SIZE);
    for(size_t i = 0; i < free_id.size(); ++i) {
        free_id[i] = free_id.size() - i - 1;
    }
    entities_alive.reserve(POOL_SIZE);
    entities_killed.reserve(POOL_SIZE);
}


EntityManager::~EntityManager() {
}

void EntityManager::killAll() {
    for(auto& e : entities_alive) {
        if(e && e->allocated) {
            if(!checkID(e->ID) && entitiesComponents[e->ID]) {
                entitiesComponents[e->ID].reset();
            }
            delete e;
        }
    }

    for(size_t i = 0; i < free_id.size(); ++i) {
        free_id[i] = free_id.size() - i - 1;
    }
    entities_alive.clear();
    entities_killed.clear();
    capacity = 0;
    posIndex = 0;
}

bool EntityManager::isExists(size_t id) const{
    return (id < capacity && entities_alive[id] &&
            (entities_alive[id]->ID != MAX_ID));
}

void EntityManager::make() {
    if(temp_entities.empty())
        return;
    for(pEntity& e : temp_entities) {
        if(e->toCreate == false)
            continue;
        e->active = true;
        entities_alive.push_back(std::move(e));
    }
    temp_entities.clear();
}

Entity* EntityManager::createEntity() {
    
    if(capacity == POOL_SIZE) return nullptr;
    
    if(entities_killed.empty()) {
        posIndex++;
        capacity++;
        Entity* entity = new Entity(free_id.back());
       
        entity->allocated = true;

        temp_entities.push_back(std::move(entity));
        free_id.pop_back();
        
        return temp_entities.back();

    } else {
        capacity++;
        size_t id = entities_killed.back();

        entities_killed.pop_back();
        entities_alive[id]->active = true;
        entities_alive[id]->ID = id;
        return entities_alive[id];
    }
    return nullptr;
}

void EntityManager::getEntities(std::function<void(Entity*)> func) {
    for(pEntity& e : entities_alive)
        if(isExists(e->ID))
            func(e);
}

void EntityManager::getEntitiesWithComponents(std::function<void(Entity*)> func, std::bitset<MAX_COMPONENTS>& bits) {

    for(pEntity& e : entities_alive) {
        if(!e)
            continue;

        if(isExists(e->ID) && hasComponents(e, bits)) {
            func(e);
        }
    }
}

std::vector<size_t> EntityManager::getEntitiesAlive() {
    std::vector<size_t> temp;
    for(pEntity& e : entities_alive)
        if(isExists(e->ID))
            temp.push_back(e->ID);

    return temp;
}

bool EntityManager::hasComponents(Entity* e, const std::vector<std::size_t>& compo) const{
    if(!e || isExists(e->ID))
        return false;
    if(entitiesComponents[e->ID]) {
        for(std::size_t c : compo) {
            if(!entitiesComponents[e->ID]->has(c))
                return false;
        }
        return true;
    }
    return false;
}

bool EntityManager::hasComponents(Entity* e, const Mask& bits) const{
    if(!e)
        return false;
    if(entitiesComponents[e->ID])
        return entitiesComponents[e->ID]->has(bits);
    return false;
}

bool EntityManager::hasComponents(size_t id,const Mask& bits) const {
    //std::cout <<"Has "<< id << " " << entitiesComponents[id]->has(bits) << std::endl;
    if(entitiesComponents[id])
        return entitiesComponents[id]->has(bits);
    return false;
}

void EntityManager::deleteEntity(Entity* e) {
    assert(e);
    if(entitiesComponents[e->ID]) {
        entitiesComponents[e->ID]->resetMask();
        entitiesComponents[e->ID].reset();
    }

    destroyEntity(e->ID, e->active);
    e->toCreate = false;
    e->active = false;
}

void EntityManager::destroyEntity(size_t id, bool isActive) {
    
    if(isActive && !entities_alive.empty()) {
        capacity--;
        entities_alive[id]->ID = MAX_ID;
        entities_killed.push_back(id);
    }
}

size_t EntityManager::getID(Entity* e) {
    return e->ID;
}
