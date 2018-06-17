#include "Entity.h"

Entity::Entity() {
    ID = std::numeric_limits<size_t>::max();
    active = false;
    toCreate = true;
    allocated = false;
}

Entity::Entity(size_t ID) {
    this->ID = ID;
}

Entity::~Entity() {

}


void Entity::destroy() {
    EntityManager::get().deleteEntity(this);
}

