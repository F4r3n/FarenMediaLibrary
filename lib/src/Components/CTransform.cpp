#include "Components/CTransform.h"
#include "Entity.h"
#include <EntityManager.h>
#include "Core/Math/Matrix.h"
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

CTransform::CTransform(const fm::math::Vector3f& position,
                       const fm::math::Vector3f& scale,
                       const fm::math::Vector3f& rotation,
                       const int& layer)
    :  layer(layer)
{
	transform.position = position;
	transform.scale = scale;
	transform.rotation = rotation;
    _name = "Transform";

}

bool CTransform::Serialize(json &ioJson) const
{

    ioJson[Keys::position] = transform.position;
    ioJson[Keys::scale] = transform.scale;
    ioJson[Keys::rotation] = transform.rotation;
    ioJson[Keys::father] = idFather;
    return true;
}

void CTransform::Destroy()
{
    EntityManager::get().removeComponent<CTransform>(BaseComponent::_IDEntity);
}

bool CTransform::Read(const json &inJSON)
{

    transform.position = inJSON[Keys::position];
    transform.scale = inJSON[Keys::scale];
    transform.rotation = inJSON[Keys::rotation];
    idFather = inJSON[Keys::father];
    return true;
}

const std::string &CTransform::GetName() const
{
   return _name;
}

fm::math::Vector3f CTransform::getWorldPos() const
{
    if(idFather == -1) return transform.position;

    Entity* father = EntityManager::get().getEntity(idFather);
    if(!father)
        return transform.position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return transform.position;

    fm::math::mat m;
    m.identity();
    m = fm::math::rotate(m, fatherTransform->transform.rotation.x, fm::math::vec3(1,0,0));
    m = fm::math::rotate(m, fatherTransform->transform.rotation.y, fm::math::vec3(0,1,0));
    m = fm::math::rotate(m, fatherTransform->transform.rotation.z, fm::math::vec3(0,0,1));
    m = fm::math::translate(m, transform.position + fatherTransform->getWorldPos());

    return fm::math::vec3(m[3][0], m[3][1], m[3][2]);
}

fm::math::Vector3f CTransform::getWorldPos(EntityManager& manager) const
{
    if(idFather == -1) return transform.position;
    Entity* father = manager.getEntity(idFather);
    if(!father)
        return transform.position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return transform.position;
    fm::math::mat m;
    m.identity();
    m = fm::math::rotate(m, fatherTransform->transform.rotation.x, fm::math::vec3(1,0,0));
    m = fm::math::rotate(m, fatherTransform->transform.rotation.y, fm::math::vec3(0,1,0));
    m = fm::math::rotate(m, fatherTransform->transform.rotation.z, fm::math::vec3(0,0,1));
    m = fm::math::translate(m, transform.position + fatherTransform->getWorldPos());

    return fm::math::vec3(m[3][0], m[3][1], m[3][2]);
}

void CTransform::setFather(Entity* e)
{
    setFather(e->ID);
}


void CTransform::setFather(id id)
{
    this->idFather = id;
}
