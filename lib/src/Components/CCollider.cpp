#include "Components/CCollider.h"
#include "btBulletDynamicsCommon.h"
#include "EntityManager.h"
#include "Core/Transform.h"
using namespace fmc;

CCollider::CCollider()
{
	_collisionShape = nullptr;
	_isInWorld = false;
	_scale = fm::math::vec3(1, 1, 1);
	_name = "Collider";
	_shape = SHAPE::BOX;
}

CCollider::~CCollider()
{
	delete _collisionShape;
}

void CCollider::Init(const fm::Transform& inTransform)
{

	switch (_shape)
	{
		case fmc::BOX:
			_collisionShape = new btBoxShape(btVector3(btScalar(_scale.x*inTransform.scale.x*0.5f), btScalar(_scale.y*inTransform.scale.y*0.5f), btScalar(_scale.z*inTransform.scale.z*0.5f)));
			break;
		case fmc::SPHERE:
			_collisionShape = new btSphereShape(btScalar(_scale.x*inTransform.scale.x*0.5f));//TODO fix
			break;
		default:
			break;
	}
}

bool CCollider::IsInit()
{
	return _collisionShape != nullptr;
}


bool CCollider::Serialize(json &ioJson) const
{
	ioJson["scale"] = _scale;
	ioJson["shape"] = _shape;

	return true;
}
bool CCollider::Read(const json &inJSON)
{
	_scale = inJSON["scale"];
	_shape = inJSON["shape"];
	return true;
}

bool CCollider::SetShape(SHAPE inShape)
{
	if (!IsInit())
	{
		_shape = inShape;
		return true;
	}
	return false;
}

SHAPE CCollider::GetShape() const
{
	return _shape;
}


const std::string& CCollider::GetName() const
{
	return _name;
}


void CCollider::Destroy()
{
	EntityManager::get().removeComponent<fmc::CCollider>(BaseComponent::_IDEntity);
}


const fm::math::vec3  CCollider::GetScale() const
{
	return _scale;
}


void CCollider::SetScale(const fm::math::vec3 &inScale)
{
	_scale = inScale;
	if (_collisionShape != nullptr)
	{
		_collisionShape->setLocalScaling(btVector3(inScale.x, inScale.y, inScale.z));
	}
}