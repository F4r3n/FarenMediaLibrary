#include "EntityManager.h"
#include "Entity.h"
#include <cassert>
#define POOL_SIZE 10000

EntityManager EntityManager::em;

EntityManager::EntityManager() {

    for(size_t i = 0; i < POOL_SIZE; ++i) {
        _free_id.push(i);
    }
    _entities_alive.reserve(POOL_SIZE);
    _entities_killed.reserve(POOL_SIZE);
    _entitiesComponents.resize(POOL_SIZE);
    _currentMAX = POOL_SIZE;
}


EntityManager::~EntityManager() {

    make();
    for(size_t i = 0; i < _entities_alive.size(); ++i)
    {
        delete _entities_alive[i];
    }

    _entities_alive.clear();
    _entities_alive.shrink_to_fit();

    _entities_killed.clear();
    _entities_killed.shrink_to_fit();
}

void EntityManager::killAll() {
    for(auto& e : _entities_alive)
    {
        if(e && e->allocated)
        {
            if(!checkID(e->ID) && _entitiesComponents[e->ID])
            {
                _entitiesComponents[e->ID].reset();
            }
            
			delete e;
		}
	}

    for(size_t i = 0; i < _temp_entities.size(); ++i)
    {
        if(_temp_entities[i] && _temp_entities[i]->allocated)
        {
            delete _temp_entities[i];
        }
    }
    _temp_entities.clear();

    size_t sizeFreeID = _free_id.size();
    for(size_t i = 0; i < sizeFreeID; ++i)
    {
        _free_id.push(i);
    }
	_entities_alive.clear();
	_entities_killed.clear();
	_capacity = 0;
	_posIndex = 0;

}

void EntityManager::Free()
{
    std::queue<size_t> empty;
    std::swap( _free_id, empty );


    _entities_alive.clear();
    _entities_killed.clear();

    std::vector<std::unique_ptr<ComponentManager>> emptyC;
    std::swap(_entitiesComponents, emptyC);
}

bool EntityManager::isExists(size_t id) const{
    return (id < _capacity && _entities_alive[id] &&
            (_entities_alive[id]->ID != _MAX_ID));
}

void EntityManager::make()
{
	if(_temp_entities.empty())
        return;

    for(pEntity& e : _temp_entities)
    {
		if(e->toCreate == false)
        {
			continue;
        }
		e->active = true;
		_entities_alive.push_back(std::move(e));
	}
	_temp_entities.clear();
}

Entity* EntityManager::createEntity() {

    if(_capacity == _currentMAX)
    {
        size_t firstValue = _free_id.back() + 1;
        size_t sizeFreeID = firstValue + ADD_SIZE;

        for(size_t i = firstValue; i < sizeFreeID; ++i) {
            _free_id.push(i);
        }
        _currentMAX = sizeFreeID;

        _entities_alive.reserve(sizeFreeID);
        _entities_killed.reserve(sizeFreeID);
        _entitiesComponents.resize(sizeFreeID);
    }

    if(_entities_killed.empty())
    {
		_posIndex++;
		_capacity++;
        Entity* entity = new Entity(_free_id.front());
		entity->allocated = true;

		_temp_entities.push_back(std::move(entity));
        _free_id.pop();

		return _temp_entities.back();

    } else
    {
		_capacity++;
		size_t id = _entities_killed.back();

		_entities_killed.pop_back();
		_entities_alive[id]->active = true;
		_entities_alive[id]->ID = id;
		return _entities_alive[id];
    }
	return nullptr;
}

void EntityManager::getEntities(std::function<void(Entity*)> func)
{
	for(pEntity& e : _entities_alive)
		if(isExists(e->ID))
			func(e);
}

void EntityManager::getEntitiesWithComponents(std::function<void(Entity*)> func, std::bitset<MAX_COMPONENTS>& bits) {

	for(pEntity& e : _entities_alive) {
		if(!e)
			continue;

		if(isExists(e->ID) && hasComponents(e, bits)) {
			func(e);
		}
	}
}

std::vector<size_t> EntityManager::getEntitiesAlive() {
	std::vector<size_t> temp;
	for(pEntity& e : _entities_alive)
		if(isExists(e->ID))
			temp.push_back(e->ID);

	return temp;
}

bool EntityManager::hasComponents(Entity* e, const std::vector<std::size_t>& compo) const{
	if(!e || isExists(e->ID))
		return false;
    if(_entitiesComponents[e->ID]) {
        for(std::size_t c : compo) {
            if(!_entitiesComponents[e->ID]->has(c))
                return false;
        }
        return true;
    }
	return false;
}

bool EntityManager::hasComponents(Entity* e, const Mask& bits) const{
    if(!e)
        return false;
    if(_entitiesComponents[e->ID])
        return _entitiesComponents[e->ID]->has(bits);
	return false;
}

bool EntityManager::hasComponents(size_t id,const Mask& bits) const {
    //std::cout <<"Has "<< id << " " << entitiesComponents[id]->has(bits) << std::endl;
    if(_entitiesComponents[id])
        return _entitiesComponents[id]->has(bits);
	return false;
}

void EntityManager::deleteEntity(Entity* e) {
    assert(e);
    if(_entitiesComponents[e->ID]) {
        _entitiesComponents[e->ID]->resetMask();
        _entitiesComponents[e->ID].reset();
    }

    _destroyEntity(e->ID, e->active);
    e->toCreate = false;
    e->active = false;
}

void EntityManager::_destroyEntity(size_t id, bool isActive) {

	if(isActive && !_entities_alive.empty()) {
		_capacity--;
		_entities_alive[id]->ID = _MAX_ID;
		_entities_killed.push_back(id);
	}
}

size_t EntityManager::_getID(Entity* e) {
	return e->ID;
}
