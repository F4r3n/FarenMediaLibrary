#include "Components/CTransform.h"
#include "Entity.h"
#include <EntityManager.h>
using namespace fmc;
const std::string CTransform::name = "Transform";

CTransform::CTransform() {
}

CTransform::CTransform(const fm::Vector2f& position, 
                        const fm::Vector2f& scale, const float& rotation, const int &layer)
    : position(position)
    , scale(scale)
    , rotation(rotation), layer(layer) {
}

fm::Vector2f CTransform::getWorldPos() {
    Entity* father = EntityManager::get().getEntity(idFather);
    if(!father) return position;
    
    CTransform *fatherTransform = father->get<CTransform>();
    if(!fatherTransform) return position;
    return position + fatherTransform->getWorldPos();
}

fm::Vector2f CTransform::getWorldPos(EntityManager &manager) {
    Entity* father = manager.getEntity(idFather);
    if(!father) return position;
    
    CTransform *fatherTransform = father->get<CTransform>();
    if(!fatherTransform) return position;
    return position + fatherTransform->getWorldPos();
}

void CTransform::setFather(Entity *e) {
    setFather(e->ID);
}