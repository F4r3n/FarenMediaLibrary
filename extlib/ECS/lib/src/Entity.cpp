#include "Entity.h"

Entity::Entity() {

}

Entity::Entity(size_t ID) {
    this->ID = ID;
}

Entity::~Entity() {

}


void Entity::destroy() {
    EntityManager::get().deleteEntity(this);
}

