#include "EntityManager.h"
#include "Entity.h"
#include <cassert>
#include <functional>

#define DEBUG 0

#if DEBUG
#include <iostream>
#endif
using namespace ecs;
EntityManager::EntityManager() {
	const id poolSize = 10000;
    for(id i = 0; i < poolSize; ++i)
    {
        _free_id.push(i);
    }
    _entities_alive.reserve(poolSize);
    _entities_killed.reserve(poolSize);
    _entitiesComponents.resize(poolSize);
    _currentMAX = poolSize;
}


EntityManager::~EntityManager() {

    make();


    _entities_alive.clear();
    _entities_alive.shrink_to_fit();

    _entities_killed.clear();
    _entities_killed.shrink_to_fit();
}

void EntityManager::killInativeEntities()
{
	for (id i = 0; i < _entities_killed.size(); ++i)
	{
		id id = _entities_killed[i];

		_entities_alive[id].ID = _MAX_ID;
		_free_id.push(id);
	}
	_entities_killed.clear();
}


void EntityManager::killAll() {
    for(auto& e : _entities_alive)
    {

         if(!checkID(e.ID) && _entitiesComponents[e.ID])
         {
             _entitiesComponents[e.ID].reset();
         }
	}



    const size_t sizeFreeID = _free_id.size();
    for(id i = 0; i < sizeFreeID; ++i)
    {
        _free_id.push(i);
    }
	_entities_alive.clear();
	_entities_killed.clear();
	_capacity = 0;

}

void EntityManager::Free()
{
    std::queue<id> empty;
    std::swap( _free_id, empty );


    _entities_alive.clear();
    _entities_killed.clear();

    std::vector<std::unique_ptr<ComponentManager>> emptyC;
    std::swap(_entitiesComponents, emptyC);
}

bool EntityManager::Exists(id id) const{
#if DEBUG
 std::cout << id <<" " <<_capacity << std::endl;
 if(id < _capacity && _entities_alive[id])
 {
     std::cout << _entities_alive[id]->ID << std::endl;
 }
#endif
    return (id < _capacity  && (_entities_alive[id].ID != _MAX_ID));
}

id EntityManager::GetID(Entity *e) const
{
      return e->ID;
}


void EntityManager::make()
{
	/*if(_temp_entities.empty())
        return;

    for(Entity &e : _temp_entities)
    {
        if(e.toCreate)
        {
            e.active = true;
			e.toDelete = false;
			e.toCreate = false;
            _entities_alive.push_back(std::move(e));
            _capacity++;
            //_listEntities.push_back(_entities_alive.back());
        }
	}
	_temp_entities.clear();*/
}

Entity* EntityManager::createEntity() {
	id returnID = _MAX_ID;
    if(_capacity == _currentMAX)
    {
        id firstValue = _free_id.back() + 1;
        id sizeFreeID = firstValue + ADD_SIZE;

        for(id i = firstValue; i < sizeFreeID; ++i) {
            _free_id.push(i);
        }
        _currentMAX = sizeFreeID;

        _entities_alive.reserve(sizeFreeID);
        _entities_killed.reserve(sizeFreeID);
        _entitiesComponents.resize(sizeFreeID);
    }

    if(_entities_killed.empty())
    {
		returnID = _free_id.front();
		Entity e(returnID);

		e.active = true;
		_entities_alive.push_back(e);
		_capacity++;
        _free_id.pop();

    }
	else
    {
        //_capacity++;
		returnID = _entities_killed.back();

		_entities_killed.pop_back();
		_entities_alive[returnID].active = true;
		_entities_alive[returnID].ID = returnID;
    }
	return &_entities_alive[returnID];
}


std::vector<id> EntityManager::getEntitiesAlive() {
	std::vector<id> temp;
	for(Entity& e : _entities_alive)
        if(Exists(e.ID))
			temp.push_back(e.ID);

	return temp;
}

bool EntityManager::hasComponents(const Entity& e, const std::vector<uint16_t>& compo) const{
    if(Exists(e.ID))
		return false;
    if(_entitiesComponents[e.ID]) {
        for(uint16_t c : compo) {
            if(!_entitiesComponents[e.ID]->has(c))
                return false;
        }
        return true;
    }
	return false;
}

bool EntityManager::hasComponents(const Entity& e, const Mask& bits) const{
    if(e.ID != _MAX_ID)
        return false;
    if(_entitiesComponents[e.ID])
        return _entitiesComponents[e.ID]->has(bits);
	return false;
}

bool EntityManager::IsActive(id id) const {
	return _entities_alive[id].active;
}


bool EntityManager::hasComponents(id id,const Mask& bits) const {
    if(_entitiesComponents[id])
        return _entitiesComponents[id]->has(bits);
	return false;
}

void EntityManager::deleteEntity(Entity* e) {
    assert(e);
    if(_entitiesComponents[e->ID]) {
        _entitiesComponents[e->ID].reset();
    }

    _destroyEntity(e->ID, e->active);
    e->active = false;
}

void EntityManager::_destroyEntity(id id, bool isActive) {

	if(isActive && !_entities_alive.empty()) {
        //_capacity--;
		_entities_alive[id].ID = _MAX_ID;
		_entities_killed.push_back(id);
	}
}


bool EntityManager::_IsEntityActive(Entity *e) const
{
    return e && e->active;
}

