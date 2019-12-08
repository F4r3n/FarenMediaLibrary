#include "Components/CBody3D.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include <EntityManager.h>
#include "Components/CTransform.h"
#include "Components/CCollider.h"
using namespace fmc;

CBody3D::CBody3D()
{
	_Init();
}


void CBody3D::_Init()
{
	_ghostObject = nullptr;
	_body = nullptr;
	_isInWorld = false;
	_mass = 0.0f;
	_isGhost = true;
	_name = "Body3D";
}

bool CBody3D::SetGhost(bool value)
{
	_isGhost = value;
	return !IsInit();
}
bool CBody3D::IsGhost() const
{
	return _isGhost;
}

void CBody3D::SetMass(float inMass)
{
	_mass = inMass;
}


bool CBody3D::IsInit() const
{
	return (_body != nullptr || _ghostObject != nullptr) && _isInWorld;
}


void CBody3D::Init(CCollider *inCollider)
{
	const fm::Transform &&transform = EntityManager::get().getEntity(_IDEntity)->get<fmc::CTransform>()->GetTransform();

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (_mass != 0.f);


	btTransform object;
	object.setIdentity();
	object.setOrigin(btVector3(transform.position.x, transform.position.y, transform.position.z));

	btCollisionShape* shape = inCollider->GetCollisionShape();

	if (!_isGhost)
	{
		btVector3 localInertia(0, 0, 0);

		if (isDynamic)
		{
			shape->calculateLocalInertia(_mass, localInertia);
		}

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, new btDefaultMotionState(object), shape, localInertia);
		_body = new btRigidBody(rbInfo);
	}
	else
	{
		_ghostObject = new btGhostObject();
		_ghostObject->setCollisionShape(shape);
		_ghostObject->setCollisionFlags(_ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		_ghostObject->setWorldTransform(object);
		_ghostObject->setUserPointer(this);
	}
}

void CBody3D::_GetCurrentTransform(btTransform& transform) const
{
	if (_body && _body->getMotionState())
	{
		transform = _body->getCenterOfMassTransform();
	}
	else if (_ghostObject)
	{
		transform = _ghostObject->getWorldTransform();
	}
}


void CBody3D::SetPosition(const fm::math::vec3 &inPosition)
{
	btTransform transform;
	_GetCurrentTransform(transform);

	transform.setOrigin(btVector3(inPosition.x, inPosition.y, inPosition.z));

	if (!_isGhost)
	{
		_body->setCenterOfMassTransform(transform);
	}
	else
	{
		_ghostObject->setWorldTransform(transform);
	}
}


void CBody3D::SetRotation(const fm::math::Quaternion &inRotation)
{
	btTransform transform;
	_GetCurrentTransform(transform);

	fm::math::vec4 r = inRotation;
	transform.setRotation(btQuaternion(r.x, r.y, r.z, r.w));

	if (_body != nullptr)
	{
		_body->getMotionState()->setWorldTransform(transform);
	}
	else if (_ghostObject != nullptr)
	{
		_ghostObject->setWorldTransform(transform);
	}
}


//void CBody3D::SetRadius(float radius)
//{
//	_radius = radius;
//	_shape = SHAPE::SPHERE;
//	if (_body)
//	{
//		_body->getCollisionShape()->setLocalScaling(btVector3(_radius, _radius, _radius));
//	}
//}
//
//
//void CBody3D::SetScale(const fm::math::vec3 &inScale)
//{
//	_scale = inScale;
//	_shape = SHAPE::BOX;
//	if (_body)
//	{
//		_body->getCollisionShape()->setLocalScaling(btVector3(inScale.x, inScale.y, inScale.z));
//	}
//}


void CBody3D::GetPosition(fm::math::vec3& outVec) const
{
	btTransform transform;
	_GetCurrentTransform(transform);

	btVector3 v = transform.getOrigin();
	
	outVec = fm::math::vec3(v.x(), v.y(), v.z());
}


void CBody3D::GetRotation(fm::math::Quaternion &outQuaternion) const
{
	btTransform transform;
	_GetCurrentTransform(transform);

	fm::math::vec3 v;
	btQuaternion q = transform.getRotation();
	outQuaternion = fm::math::Quaternion(fm::math::vec4(q.x(), q.y(), q.z(), q.w()));
}



void CBody3D::AddToWorld(btDiscreteDynamicsWorld *inWorld)
{
	if (_isGhost)
	{
		inWorld->addCollisionObject(_ghostObject,1,1);
	}
	else
	{
		inWorld->addRigidBody(_body);
	}
	_isInWorld = true;
}


const std::string& CBody3D::GetName() const
{
	return _name;
}
void CBody3D::Destroy()
{
	EntityManager::get().removeComponent<fmc::CBody3D>(BaseComponent::_IDEntity);
}

bool CBody3D::Serialize(json &ioJson) const
{
	ioJson["mass"] = _mass;
	ioJson["ghost"] = _isGhost;


	return true;
}
bool CBody3D::Read(const json &inJSON)
{
	_mass = inJSON["mass"];
	_isGhost = inJSON["ghost"];

	return true;
}


CBody3D::~CBody3D()
{
	if (_body && _body->getMotionState())
	{
		delete _body->getMotionState();
	}
	delete _body;
}
