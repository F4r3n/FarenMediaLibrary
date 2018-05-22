#include "Components/CTransform.h"
#include "Entity.h"
#include <EntityManager.h>
//#include "TransformInspector.h"
using namespace fmc;
const std::string CTransform::name = "Transform";
namespace Keys
{
const std::string position("position");
const std::string scale("scale");
const std::string rotation("rotation");
const std::string father("father");
}
CTransform::CTransform()
{
    _name = "Transform";
}

CTransform::CTransform(const fm::math::Vector2f& position,
                       const fm::math::Vector2f& scale,
                       const float& rotation,
                       const int& layer)
    :  position(position), scale(scale), rotation(rotation), layer(layer)
{
    _name = "Transform";

}

bool CTransform::Serialize(json &ioJson) const
{

    ioJson[Keys::position] = fm::Serialize(&position);
    ioJson[Keys::scale] = fm::Serialize(&scale);
    ioJson[Keys::rotation] =fm::Serialize(&rotation);
    ioJson[Keys::father] = fm::Serialize<size_t, size_t>(&idFather);
    return true;
}

bool CTransform::Read(const json &inJSON)
{
    fm::Read(&position, inJSON[Keys::position]);
    return true;
}

const std::string &CTransform::GetName() const
{
   return _name;
}

fm::math::Vector2f CTransform::getWorldPos()
{
    Entity* father = EntityManager::get().getEntity(idFather);
    if(!father)
        return position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return position;
    return position + fatherTransform->getWorldPos();
}

fm::math::Vector2f CTransform::getWorldPos(EntityManager& manager)
{
    Entity* father = manager.getEntity(idFather);
    if(!father)
        return position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return position;
    return position + fatherTransform->getWorldPos();//TODO missing rotation
}

void CTransform::setFather(Entity* e)
{
    setFather(e->ID);
}


void CTransform::setFather(size_t id)
{
    this->idFather = id;
}
