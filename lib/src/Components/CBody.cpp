#include "Components/CBody.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include <EntityManager.h>
#include "Components/CTransform.h"
#include "Components/CCollider.h"
using namespace fmc;

CBody::CBody()
{
	_Init();
}


void CBody::_Init()
{
	_body = nullptr;
	_isInWorld = false;
	_mass = 1.0f;
	_isGhost = false;
	_gravity = fm::math::vec3(0.0f, -9.81f, 0.0f);
	_restitution = 1.0f;
	_friction = 0.0f;
	_angularFactor = fm::math::vec3(1.0f, 1.0f, 1.0f);
	_linearFactor = fm::math::vec3(1.0f, 1.0f, 1.0f);
	_name = "Body";
}

bool CBody::SetGhost(bool value)
{
	_isGhost = value;
	return !IsInit();
}
bool CBody::IsGhost() const
{
	return _isGhost;
}

void CBody::SetMass(float inMass)
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

float CBody::GetMass() const
{
	if (btRigidBody* body = _GetBody())
	{
		_mass = 1.0f / body->getInvMass();
	}

	return _mass;
}

void CBody::SetGravity(const fm::math::vec3& inGravity)
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

const fm::math::vec3& CBody::GetGravity() const
{
	if (btRigidBody* body = _GetBody())
	{
		btVector3 v = body->getGravity();
		_gravity = fm::math::vec3(v.x(), v.y(), v.z());
	}
	return _gravity;
}

void CBody::SetLinearVelocity(const fm::math::vec3& inAcceleration)
{
	if (btRigidBody* body = _GetBody())
	{
		body->setLinearVelocity(btVector3(inAcceleration.x, inAcceleration.y, inAcceleration.z));
		if (!body->isActive())
			body->activate();
	}
}


fm::math::vec3 CBody::GetLinearVelocity() const
{
	fm::math::vec3 velocity;
	if (btRigidBody* body = _GetBody())
	{
		btVector3 v = body->getLinearVelocity();
		velocity = fm::math::vec3(v.x(), v.y(), v.z());
	}
	return velocity;
}

void CBody::SetFriction(float inFriction)
{
	_friction = inFriction;
	if (btRigidBody* body = _GetBody())
	{
		body->setFriction(inFriction);
	}
}


float CBody::GetFriction() const
{
	if (btRigidBody* body = _GetBody())
	{
		_friction = body->getFriction();
	}
	return _friction;
}

void CBody::SetRestitution(float inRestitution)
{
	_restitution = inRestitution;
	if (btRigidBody* body = _GetBody())
	{
		body->setRestitution(_restitution);
	}
}


float CBody::GetRestitution() const
{
	if (btRigidBody* body = _GetBody())
	{
		_restitution = body->getFriction();
	}
	return _restitution;
}


void CBody::SetLinearFactor(const fm::math::vec3& inFactor)
{
	_linearFactor = inFactor;
	if (btRigidBody* body = _GetBody())
	{
		body->setLinearFactor(btVector3(_linearFactor.x, _linearFactor.y, _linearFactor.z));
	}

}
const fm::math::vec3& CBody::GetLinearFactor() const
{
	if (btRigidBody* body = _GetBody())
	{
		auto v = body->getLinearFactor();
		_linearFactor.x = v.x();
		_linearFactor.y = v.y();
		_linearFactor.z = v.x();

	}
	return _linearFactor;
}

void CBody::SetAngularFactor(const fm::math::vec3& inFactor)
{
	_angularFactor = inFactor;
	if (btRigidBody* body = _GetBody())
	{
		body->setLinearFactor(btVector3(_angularFactor.x, _angularFactor.y, _angularFactor.z));
	}

}

const fm::math::vec3& CBody::GetAngularFactor() const
{
	if (btRigidBody* body = _GetBody())
	{
		auto v = body->getAngularFactor();
		_angularFactor.x = v.x();
		_angularFactor.y = v.y();
		_angularFactor.z = v.x();

	}
	return _angularFactor;
}


bool CBody::IsInit() const
{
	return (_body != nullptr) && _isInWorld;
}

btRigidBody* CBody::_GetBody() const
{
	return _body != nullptr ? btRigidBody::upcast(_body) : nullptr;
}


btGhostObject* CBody::_GetGhost() const
{
	return _body != nullptr ? btGhostObject::upcast(_body) : nullptr;
}


void CBody::Init(CCollider *inCollider)
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
		_GetBody()->setRestitution(_restitution);
		_GetBody()->setFriction(_friction);
		_GetBody()->setAngularFactor(btVector3(_angularFactor.x, _angularFactor.y, _angularFactor.z));
		_GetBody()->setLinearFactor(btVector3(_linearFactor.x, _linearFactor.y, _linearFactor.z));
		_GetBody()->setCcdMotionThreshold(1e-7);
		_GetBody()->setCcdSweptSphereRadius(0.01f);
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

void CBody::_GetCurrentTransform(btTransform& transform) const
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


void CBody::SetPosition(const fm::math::vec3 &inPosition)
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


void CBody::SetRotation(const fm::math::Quaternion &inRotation)
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


void CBody::GetPosition(fm::math::vec3& outVec) const
{
	btTransform transform;
	_GetCurrentTransform(transform);

	btVector3 v = transform.getOrigin();
	
	outVec = fm::math::vec3(v.x(), v.y(), v.z());
}


void CBody::GetRotation(fm::math::Quaternion &outQuaternion) const
{
	btTransform transform;
	_GetCurrentTransform(transform);

	fm::math::vec3 v;
	btQuaternion q = transform.getRotation();
	outQuaternion = fm::math::Quaternion(fm::math::vec4(q.x(), q.y(), q.z(), q.w()));
}



void CBody::AddToWorld(btDiscreteDynamicsWorld *inWorld)
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


const std::string& CBody::GetName() const
{
	return _name;
}
void CBody::Destroy()
{
	EntityManager::get().removeComponent<fmc::CBody>(BaseComponent::_IDEntity);
}

bool CBody::Serialize(nlohmann::json &ioJson) const
{
	ioJson["mass"] = _mass;
	ioJson["gravity"] = _gravity;
	ioJson["ghost"] = _isGhost;
	ioJson["restitution"] = _restitution;
	ioJson["friction"] = _friction;
	ioJson["angularFactor"] = _angularFactor;
	ioJson["linearFactor"] = _linearFactor;

	return true;
}
bool CBody::Read(const nlohmann::json &inJSON)
{
	try
	{
		_mass = inJSON.at("mass");
		_isGhost = inJSON.at("ghost");
		_gravity = inJSON.at("gravity");
		_restitution = inJSON.at("restitution");
		_friction = inJSON.at("friction");
		_angularFactor = inJSON.at("angularFactor");
		_linearFactor = inJSON.at("linearFactor");

	}
	catch (std::exception&)
	{

	}


	return true;
}


CBody::~CBody()
{

}
