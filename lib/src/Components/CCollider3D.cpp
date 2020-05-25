#include "Components/CCollider3D.h"
#include "btBulletDynamicsCommon.h"
#include "EntityManager.h"
#include "Core/Transform.h"
#include <nlohmann/json.hpp>
using namespace fmc;

CCollider3D::CCollider3D()
{
	_collisionShape = nullptr;
	_scale = fm::math::vec3(1, 1, 1);
	_name = "Collider";
	_shape = SHAPE::BOX;
}

CCollider3D::~CCollider3D()
{
	delete _collisionShape;
}

void CCollider3D::Init(const fm::Transform& inTransform)
{

	switch (_shape)
	{
	case fmc::CCollider3D::SHAPE::BOX:
			_collisionShape = new btBoxShape(btVector3(btScalar(_scale.x*inTransform.scale.x*0.5f), btScalar(_scale.y*inTransform.scale.y*0.5f), btScalar(_scale.z*inTransform.scale.z*0.5f)));
			break;
	case fmc::CCollider3D::SHAPE::SPHERE:
			_collisionShape = new btSphereShape(btScalar(_scale.x*inTransform.scale.x*0.5f));//TODO fix
			break;
		default:
			break;
	}
}

bool CCollider3D::IsInit()
{
	return _collisionShape != nullptr;
}


bool CCollider3D::Serialize(nlohmann::json &ioJson) const
{
	ioJson["scale"] = _scale;
	ioJson["shape"] = _shape;

	return true;
}
bool CCollider3D::Read(const nlohmann::json &inJSON)
{
	_scale = inJSON["scale"];
	_shape = inJSON["shape"];
	return true;
}

bool CCollider3D::SetShape(SHAPE inShape)
{
	if (!IsInit())
	{
		_shape = inShape;
		return true;
	}
	return false;
}

CCollider3D::SHAPE CCollider3D::GetShape() const
{
	return _shape;
}


const std::string& CCollider3D::GetName() const
{
	return _name;
}


void CCollider3D::Destroy()
{
	EntityManager::get().removeComponent<fmc::CCollider3D>(BaseComponent::_IDEntity);
}


const fm::math::vec3  CCollider3D::GetScale() const
{
	return _scale;
}


void CCollider3D::SetScale(const fm::math::vec3 &inScale)
{
	_scale = inScale;
	if (_collisionShape != nullptr)
	{
		_collisionShape->setLocalScaling(btVector3(inScale.x, inScale.y, inScale.z));
	}
}