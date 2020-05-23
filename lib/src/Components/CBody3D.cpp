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
	_body = nullptr;
	_isInWorld = false;
	_mass = 1.0f;
	_isGhost = false;
	_gravity = fm::math::vec3(0.0f, -9.81f, 0.0f);
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
	if (btRigidBody* body = _GetBody())
	{
		btVector3 localInertia(0, 0, 0);

		fmc::CCollider* collider = EntityManager::get().getEntity(_IDEntity)->get<fmc::CCollider>();
		if (collider != nullptr)
		{
			btCollisionShape* shape = collider->GetCollisionShape();

			shape->calculateLocalInertia(_mass, localInertia);
		}
		
		body->setMassProps(_mass, localInertia);
	}
}

float CBody3D::GetMass() const
{
	if (btRigidBody* body = _GetBody())
	{
		_mass = 1.0f / body->getInvMass();
	}

	return _mass;
}

void CBody3D::SetGravity(const fm::math::vec3& inGravity)
{
	_gravity = inGravity;
	if (btRigidBody* body = _GetBody())
	{
		body->setGravity(btVector3(inGravity.x, inGravity.y, inGravity.z));
		body->applyGravity();
		if(!body->isActive())
			body->activate();
	}
}

const fm::math::vec3& CBody3D::GetGravity() const
{
	if (btRigidBody* body = _GetBody())
	{
		btVector3 v = body->getGravity();
		_gravity = fm::math::vec3(v.x(), v.y(), v.z());
	}
	return _gravity;
}

void CBody3D::SetLinearVelocity(const fm::math::vec3& inAcceleration)
{
	if (btRigidBody* body = _GetBody())
	{
		body->setLinearVelocity(btVector3(inAcceleration.x, inAcceleration.y, inAcceleration.z));
		if (!body->isActive())
			body->activate();
	}
}


fm::math::vec3 CBody3D::GetLinearVelocity() const
{
	fm::math::vec3 velocity;
	if (btRigidBody* body = _GetBody())
	{
		btVector3 v = body->getLinearVelocity();
		velocity = fm::math::vec3(v.x(), v.y(), v.z());
	}
	return velocity;
}


bool CBody3D::IsInit() const
{
	return (_body != nullptr) && _isInWorld;
}

btRigidBody* CBody3D::_GetBody() const
{
	return _body != nullptr ? btRigidBody::upcast(_body) : nullptr;
}


btGhostObject* CBody3D::_GetGhost() const
{
	return _body != nullptr ? btGhostObject::upcast(_body) : nullptr;
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
		_GetBody()->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));
		_GetBody()->setRestitution(1.0f);
		_GetBody()->setFriction(0.0f);
	}
	else
	{
		_body = new btGhostObject();
		_body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	}

	_body->setCollisionShape(shape);
	_body->setWorldTransform(object);
	_body->setUserPointer(this);
	
}

void CBody3D::_GetCurrentTransform(btTransform& transform) const
{
	if (btRigidBody* body = _GetBody())
	{
		transform = body->getCenterOfMassTransform();
	}
	else if(_body != nullptr)
	{
		transform = _body->getWorldTransform();
	}
}


void CBody3D::SetPosition(const fm::math::vec3 &inPosition)
{
	btTransform transform;
	_GetCurrentTransform(transform);

	transform.setOrigin(btVector3(inPosition.x, inPosition.y, inPosition.z));

	if (!_isGhost)
	{
		_GetBody()->setCenterOfMassTransform(transform);
	}
	else
	{
		_body->setWorldTransform(transform);
	}
}


void CBody3D::SetRotation(const fm::math::Quaternion &inRotation)
{
	btTransform transform;
	_GetCurrentTransform(transform);

	fm::math::vec4 r = inRotation;
	transform.setRotation(btQuaternion(r.x, r.y, r.z, r.w));

	if (btRigidBody* body = _GetBody())
	{
		body->getMotionState()->setWorldTransform(transform);
	}
	else if (_body != nullptr)
	{
		_body->setWorldTransform(transform);
	}
}


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
		inWorld->addCollisionObject(_body,1,1);
	}
	else
	{
		inWorld->addRigidBody(_GetBody());
		SetGravity(_gravity);
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
	ioJson["gravity"] = _gravity;
	ioJson["ghost"] = _isGhost;


	return true;
}
bool CBody3D::Read(const json &inJSON)
{
	try
	{
		_mass = inJSON.at("mass");
		_isGhost = inJSON.at("ghost");
		_gravity = inJSON.at("gravity");
	}
	catch (std::exception& e)
	{

	}


	return true;
}


CBody3D::~CBody3D()
{

}
