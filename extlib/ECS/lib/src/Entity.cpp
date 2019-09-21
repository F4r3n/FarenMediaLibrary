#include "Entity.h"
#include "EntityManager.h"
Entity::Entity() {
    ID = std::numeric_limits<ecs::id>::max();
    active = false;
}

Entity::Entity(ecs::id ID) {
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