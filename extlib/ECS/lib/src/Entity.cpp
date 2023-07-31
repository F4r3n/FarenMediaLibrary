#include "Entity.h"
#include "EntityManager.h"
const Entity::Id Entity::INVALID;

Entity::Entity() {
}

Entity::Entity(EntityManager* manager, Id inID) {
    this->_id = inID;
	this->_manager = manager;
}

Entity::~Entity() {

}


void Entity::destroy() {
    EntityManager::get().deleteEntity(*this);
}

std::vector<BaseComponent*> Entity::getAllComponents() 
{
	return EntityManager::get().getAllComponents(*this);
}

bool Entity::Valid()
{
	return _manager->Valid(_id);
}