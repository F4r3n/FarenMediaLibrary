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
const std::string layer("layer");

}
CTransform::CTransform()
{
    _name = "Transform";
}

CTransform::CTransform(const fm::math::Vector3f& inPosition,
                       const fm::math::Vector3f& inScale,
                       const fm::math::Vector3f& inRotation,
                       const int& layer)
    :  layer(layer)
{
	position = inPosition;
	scale = inScale;
	rotation = inRotation;
    _name = "Transform";

}

bool CTransform::Serialize(json &ioJson) const
{

    ioJson[Keys::position] = position;
    ioJson[Keys::scale] = scale;
    ioJson[Keys::rotation] = rotation;
    ioJson[Keys::father] = idFather;
	ioJson[Keys::layer] = layer;

    return true;
}


void CTransform::From(const fmc::CTransform *inTransform)
{
	scale = inTransform->scale;
	position = inTransform->position;
	rotation = inTransform->rotation;
	idFather = inTransform->idFather;
	layer = inTransform->layer;
}


void CTransform::Destroy()
{
    EntityManager::get().removeComponent<CTransform>(BaseComponent::_IDEntity);
}

bool CTransform::Read(const json &inJSON)
{

    position = inJSON[Keys::position];
    scale = inJSON[Keys::scale];
    rotation = inJSON[Keys::rotation];
    idFather = inJSON[Keys::father];
    return true;
}

const std::string &CTransform::GetName() const
{
   return _name;
}

fm::Transform CTransform::GetTransform() const
{
	fm::Transform tr;
	tr.position = position;
	tr.rotation = rotation;
	tr.scale = scale;
	tr.worldPosition = getWorldPos();
	return tr;
}


fm::math::Vector3f CTransform::getWorldPos() const
{
    if(idFather == -1) return position;

    Entity* father = EntityManager::get().getEntity(idFather);
    if(!father)
        return position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return position;

    fm::math::mat m;
    m.identity();
    m = fm::math::rotate(m, fatherTransform->rotation.x, fm::math::vec3(1,0,0));
    m = fm::math::rotate(m, fatherTransform->rotation.y, fm::math::vec3(0,1,0));
    m = fm::math::rotate(m, fatherTransform->rotation.z, fm::math::vec3(0,0,1));
    m = fm::math::translate(m, position + fatherTransform->getWorldPos());

    return fm::math::vec3(m[3][0], m[3][1], m[3][2]);
}

fm::math::Vector3f CTransform::getWorldPos(EntityManager& manager) const
{
    if(idFather == -1) return position;
    Entity* father = manager.getEntity(idFather);
    if(!father)
        return position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return position;
    fm::math::mat m;
    m.identity();
    m = fm::math::rotate(m, fatherTransform->rotation.x, fm::math::vec3(1,0,0));
    m = fm::math::rotate(m, fatherTransform->rotation.y, fm::math::vec3(0,1,0));
    m = fm::math::rotate(m, fatherTransform->rotation.z, fm::math::vec3(0,0,1));
    m = fm::math::translate(m, position + fatherTransform->getWorldPos());

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
