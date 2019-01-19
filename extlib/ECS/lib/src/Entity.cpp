#include "Entity.h"
#include "EntityManager.h"
Entity::Entity() {
    ID = std::numeric_limits<id>::max();
    active = false;
}

Entity::Entity(id ID) {
    this->ID = ID;
}

Entity::~Entity() {

}


void Entity::destroy() {
    EntityManager::get().deleteEntity(this);
}

std::vector<BaseComponent*> Entity::getAllComponents() 
{
	return EntityManager::get().getAllComponents(this);
}