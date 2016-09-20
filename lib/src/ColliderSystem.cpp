#include "ColliderSystem.h"
#include "CTransform.h"
#include "Entity.h"
using namespace fms;

ColliderSystem::ColliderSystem() {
    addComponent<fmc::CCollider>();
    addComponent<fmc::CTransform>();
    quadTree = std::make_unique<fm::QuadTree<fmc::CCollider>>(0, fm::Recti(0,0,1000,1000));
}
ColliderSystem::~ColliderSystem() {
    
}
void ColliderSystem::update(float dt, std::shared_ptr<Entity> e) {
    fmc::CCollider *collider = e->get<fmc::CCollider>();
    collider->idCollision = e->ID;
    fmc::CTransform *transform = e->get<fmc::CTransform>();
    std::shared_ptr<fm::Object<fmc::CCollider>> object = std::make_shared<fm::Object<fmc::CCollider>>();
    object->rect = fm::Recti(transform->position.x, transform->position.y, 
                             transform->scale.x, transform->scale.y);
    //object->object = collider;
    //object->object = collider;
    
}
void ColliderSystem::init(std::shared_ptr<Entity> e) {
    
}
void ColliderSystem::over() {
    
}