#include "Components/CTransform.h"
#include "Entity.h"
#include <EntityManager.h>
#include "Core/Math/Matrix.h"
#include "Core/Math/Functions.h"
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
	idFather = 0;
	_hasFather = false;

	position = { 0, 0, 0 };
	scale = { 1, 1, 1 };
	_rotation = fm::math::Quaternion();
}

CTransform::CTransform(const fm::math::Vector3f& inPosition,
                       const fm::math::Vector3f& inScale,
                       const fm::math::Vector3f& inRotation,
                       const int& layer)
    :  layer(layer)
{
	position = inPosition;
	scale = inScale;
	_rotation.FromEulerAngles(inRotation);
    _name = "Transform";
	idFather = 0;
	_hasFather = false;

}

CTransform::~CTransform()
{
	
}

bool CTransform::Serialize(json &ioJson) const
{

    ioJson[Keys::position] = position;
    ioJson[Keys::scale] = scale;
    ioJson[Keys::rotation] = (fm::math::vec4)_rotation;
    ioJson[Keys::father] = idFather;
	ioJson[Keys::layer] = layer;

    return true;
}


void CTransform::From(const fmc::CTransform *inTransform)
{
	scale = inTransform->scale;
	position = inTransform->position;
	_rotation = inTransform->_rotation;
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
    _rotation = static_cast<fm::math::vec4>(inJSON[Keys::rotation]);
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
	tr.worldTransform = GetWorldMatrix();
	tr.position = position;//TODO temporary fix
	tr.rotation = _rotation;
	tr.scale = scale;
	return tr;
}

void CTransform::SetLocalMatrixModel(const fm::math::mat &inLocalMatrix)
{
	assert(false);
}

fm::math::mat CTransform::GetWorldMatrix(bool opposePosition) const
{
	if (!_hasFather)
		return GetLocalMatrixModel(opposePosition);

	Entity* e = EntityManager::get().getEntity(idFather);
	if (e != nullptr && e->active)
	{
		fmc::CTransform *fatherTransform = e->get<fmc::CTransform>();
		fm::math::mat&& matrix = fatherTransform->GetWorldMatrix(opposePosition);
		fm::math::mat&& localMatrix = GetLocalMatrixModel(opposePosition);

		return matrix * localMatrix;
	}

	return GetLocalMatrixModel();
}



fm::math::mat CTransform::GetLocalMatrixModel(bool opposePosition) const
{
	fm::math::mat model;
	model.identity();
	if(opposePosition)
		model = fm::math::translate(model, fm::math::vec3(-position.x, -position.y, -position.z));
	else
		model = fm::math::translate(model, fm::math::vec3(position.x, position.y, position.z));

	model = fm::math::rotate(model, _rotation.GetRotationMatrix());
	
	model = fm::math::scale(model, fm::math::vec3(scale.x, scale.y, scale.z));
	return model;
}


void CTransform::setFather(Entity* e)
{
    setFather(e->ID);
}

void CTransform::RemoveFather()
{
	_hasFather = false;
	idFather = 0;
}


void CTransform::setFather(ecs::id id)
{
    idFather = id;
	_hasFather = true;
}
