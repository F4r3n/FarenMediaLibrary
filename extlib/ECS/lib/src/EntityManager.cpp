#include "EntityManager.h"
#include "Entity.h"
#include <cassert>
//#include <iostream>
EntityManager EntityManager::em;

EntityManager::EntityManager() {
	_free_id.resize(POOL_SIZE);
    size_t sizeFreeID = _free_id.size();
	for(size_t i = 0; i < sizeFreeID; ++i) {
		_free_id[i] = sizeFreeID - i - 1;
	}
	_entities_alive.reserve(POOL_SIZE);
	_entities_killed.reserve(POOL_SIZE);
}


EntityManager::~EntityManager() {
    _free_id.clear();
    _free_id.shrink_to_fit();

    _entities_alive.clear();
    _entities_alive.shrink_to_fit();

    _entities_killed.clear();
    _entities_killed.shrink_to_fit();
}

void EntityManager::killAll() {
	for(auto& e : _entities_alive) {
		if(e && e->allocated) {
			if(!checkID(e->ID) && _entitiesComponents[e->ID]) {
				_entitiesComponents[e->ID].reset();
			}
            
			delete e;
		}
	}
	size_t sizeFreeID = _free_id.size();
	for(size_t i = 0; i < sizeFreeID; ++i) {
		_free_id[i] = sizeFreeID - i - 1;
	}
	_entities_alive.clear();
	_entities_killed.clear();
	_capacity = 0;
	_posIndex = 0;
}

bool EntityManager::isExists(size_t id) const{
	return (id < _capacity && _entities_alive[id] &&
			(_entities_alive[id]->ID != _MAX_ID));
}

void EntityManager::make() {
	if(_temp_entities.empty())
		return;
	for(pEntity& e : _temp_entities) {
		if(e->toCreate == false)
			continue;
		e->active = true;
		_entities_alive.push_back(std::move(e));
	}
	_temp_entities.clear();
}

Entity* EntityManager::createEntity() {

	if(_capacity == POOL_SIZE) return nullptr;

	if(_entities_killed.empty()) {
		_posIndex++;
		_capacity++;
		Entity* entity = new Entity(_free_id.back());

		entity->allocated = true;

		_temp_entities.push_back(std::move(entity));
		_free_id.pop_back();

		return _temp_entities.back();

	} else {
		_capacity++;
		size_t id = _entities_killed.back();

		_entities_killed.pop_back();
		_entities_alive[id]->active = true;
		_entities_alive[id]->ID = id;
		return _entities_alive[id];
	}
	return nullptr;
}

void EntityManager::getEntities(std::function<void(Entity*)> func) {
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
