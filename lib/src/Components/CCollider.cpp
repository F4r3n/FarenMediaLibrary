#include "Components/CCollider.h"
#include "btBulletCollisionCommon.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "EntityManager.h"
#include "Core/Transform.h"
#include <nlohmann/json.hpp>

using namespace fmc;

CCollider::CCollider()
{
	_collisionShape = nullptr;
	_scale = fm::math::vec3(1, 1, 1);
	_name = "Collider";
	_shape = SHAPE::BOX;
}

CCollider::~CCollider()
{
	delete _collisionShape;
}


void CCollider::Init(const fm::math::vec3& inScale)
{
	if (_collisionShape == nullptr)
	{
		switch (_shape)
		{
		case fmc::CCollider::SHAPE::BOX:
			_collisionShape = new btBoxShape(btVector3(btScalar(_scale.x * inScale.x * 0.5f),
														btScalar(_scale.y * inScale.y * 0.5f),
														btScalar(_scale.z * inScale.z * 0.5f)));
			break;
		case fmc::CCollider::SHAPE::SPHERE:
			_collisionShape = new btSphereShape(btScalar(_scale.x * inScale.x * 0.5f));
			break;
		case fmc::CCollider::SHAPE::BOX2D:
			_collisionShape = new btBox2dShape(btVector3(btScalar(_scale.x * inScale.x * 0.5f),
														btScalar(_scale.y * inScale.y * 0.5f),
														btScalar(_scale.z * inScale.z * 0.5f)));//TODO fix
			break;
		}
	}
	else
	{
		assert(false);
	}
}


bool CCollider::IsInit()
{
	return _collisionShape != nullptr;
}


bool CCollider::Serialize(nlohmann::json &ioJson) const
{
	ioJson["scale"] = _scale;
	ioJson["shape"] = _shape;

	return true;
}
bool CCollider::Read(const nlohmann::json &inJSON)
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

CCollider::SHAPE CCollider::GetShape() const
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