#include "Components/CTransform.h"
#include "Entity.h"
#include <EntityManager.h>
//#include "TransformInspector.h"
using namespace fmc;
const std::string CTransform::name = "Transform";

CTransform::CTransform() {
    _name = "Transform";
    //inspector = new gui::TransformInspector();
}

CTransform::CTransform(const fm::math::Vector2f& position,
                       const fm::math::Vector2f& scale,
                       const float& rotation,
                       const int& layer)
    :  position(position), scale(scale), rotation(rotation), layer(layer) {
    _name = "Transform";
    //inspector = new gui::TransformInspector();

}

bool CTransform::Serialize(json &ioJson) const
{
    ioJson["Position"] = {{"x", position.x}, {"y", position.y}};
    return true;
}

bool CTransform::Read(const json &inJSON)
{
    return false;
}

const std::string &CTransform::GetName() const
{
   return _name;
}

fm::math::Vector2f CTransform::getWorldPos() {
    Entity* father = EntityManager::get().getEntity(idFather);
    if(!father)
        return position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return position;
    return position + fatherTransform->getWorldPos();
}

fm::math::Vector2f CTransform::getWorldPos(EntityManager& manager) {
    Entity* father = manager.getEntity(idFather);
    if(!father)
        return position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return position;
    return position + fatherTransform->getWorldPos();
}

void CTransform::setFather(Entity* e)
{
    setFather(e->ID);
}


void CTransform::setFather(size_t id)
{
    this->idFather = id;
}
