#include "EntityManager.h"
#include "Entity.h"
#include <cassert>
#include <functional>


//using namespace ecs;
EntityManager::EntityManager() {
	const uint32_t poolSize = 10;
	_Init(poolSize);
}
void EntityManager::_Init(size_t inSize)
{
	for (uint32_t i = inSize; i >= 1; i--)
	{
		_free_id.push_back(i);
	}
	_entity_version.resize(inSize + 1);
	_entitiesComponents.resize(inSize + 1);
	_capacity = 1;
}


EntityManager::~EntityManager()
{
}





void EntityManager::Free()
{
	_capacity = 0;
	_entity_version.clear();
	_entitiesComponents.clear();
	_free_id.clear();
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



