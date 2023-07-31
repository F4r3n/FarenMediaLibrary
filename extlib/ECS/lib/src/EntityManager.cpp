#include "EntityManager.h"
#include "Entity.h"
#include <cassert>
#include <functional>

#define DEBUG 0

#if DEBUG
#include <iostream>
#endif
//using namespace ecs;
EntityManager::EntityManager() {
	const uint32_t poolSize = 10;
    for(uint32_t i = poolSize; i >= 1; i--)
    {
        _free_id.push_back(i);
    }
	_entity_version.resize(poolSize + 1);
    _entitiesComponents.resize(poolSize + 1);
	_capacity = 0;
}


EntityManager::~EntityManager()
{
}

void EntityManager::killInativeEntities()
{
	/*
	for (id i = 0; i < _entities_killed.size(); ++i)
	{
		id id = _entities_killed[i];

		_entities_alive[id].ID = _MAX_ID;
		_free_id.push(id);
	}
	_entities_killed.clear();
	*/
}


void EntityManager::killAll() {

	/*
    for(auto& e : _entitiesComponents)
    {
        _entitiesComponents[e.ID].reset();
	}



    const size_t sizeFreeID = _free_id.size();
    for(id i = 0; i < sizeFreeID; ++i)
    {
        _free_id.push(i);
    }
	_entities_alive.clear();
	_entities_killed.clear();
	_capacity = 0;
	*/

}

void EntityManager::Free()
{
	/*
    std::queue<id> empty;
    std::swap( _free_id, empty );


    _entities_alive.clear();
    _entities_killed.clear();

    std::vector<std::unique_ptr<ComponentManager>> emptyC;
    std::swap(_entitiesComponents, emptyC);
	*/
}

bool EntityManager::Valid(const Entity::Id& e) const
{
	 if (e.index() >= _entity_version.size())
		 return false;

	 return e.version() == _entity_version[e.index()];
}

Entity::Id EntityManager::CreateID(uint32_t inID)
{
	return Entity::Id(inID, _entity_version[inID]);
}

Entity EntityManager::GetEntity(Entity::Id inID)
{
	return Entity(this, inID);
}



Entity EntityManager::createEntity() {
	uint32_t returnID = 0;
    if(_free_id.empty())
    {
		uint32_t firstValue = _entity_version.size();
		uint32_t sizeFreeID = firstValue + ADD_SIZE;

        for(long i = sizeFreeID; i >= firstValue; i--) {
            _free_id.push_back(i);
        }

		_entity_version.resize(sizeFreeID + 1);
        _entitiesComponents.resize(sizeFreeID + 1);
    }


	returnID = _free_id.back();
	_free_id.pop_back();
	//_entities_alive.push_back(e);
	_entity_version[returnID] = 1;
	_capacity++;
    
	Entity entity(this, Entity::Id(returnID, _entity_version[returnID]));

    

	return entity;
}





bool EntityManager::hasComponents(const Entity& e, const std::vector<uint16_t>& compo) const{
    if(!Valid(e.id()))
		return false;
    if(_entitiesComponents[e.id().index()]) {
        for(uint16_t c : compo) {
            if(!_entitiesComponents[e.id().index()]->has(c))
                return false;
        }
        return true;
    }
	return false;
}

bool EntityManager::hasComponents(const Entity::Id& id, const Mask& bits) const{

    if(_entitiesComponents[id.index()])
        return _entitiesComponents[id.index()]->has(bits);
	return false;
}



void EntityManager::deleteEntity(const Entity& e) {

    if(_entitiesComponents[e.id().index()]) {
        _entitiesComponents[e.id().index()].reset();
    }

	_entity_version[e.id().index()]++;
	_free_id.push_back(e.id().index());
}



