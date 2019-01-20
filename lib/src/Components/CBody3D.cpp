#include "Components/CBody3D.h"
#include "btBulletDynamicsCommon.h"
#include <EntityManager.h>
#include "Components/CTransform.h"
using namespace fmc;

CBody3D::CBody3D()
{
	_body = nullptr;
	_isInWorld = false;
	_mass = 0.0f;
	_shape = SHAPE::BOX;
	_scale = fm::math::vec3(1, 1, 1);
	_radius = 1.0f;
}

CBody3D::CBody3D(const fm::math::vec3 &inScale)
{
	_body = nullptr;
	_isInWorld = false;
	_mass = 0.001f;
	_shape = SHAPE::BOX;
	_scale = inScale;
}

CBody3D::CBody3D(float radius)
{
	_radius = radius;
	_shape = SHAPE::SPHERE;
}

void CBody3D::SetMass(float inMass)
{
	_mass = inMass;
}


bool CBody3D::IsInit() const
{
	return _body != nullptr && _isInWorld;
}


void CBody3D::Init()
{
	fmc::CTransform *transform = EntityManager::get().getEntity(_IDEntity)->get<fmc::CTransform>();
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (_mass != 0.f);


	btTransform object;
	object.setIdentity();
	object.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));

	btCollisionShape* shape = nullptr;

	switch (_shape)
	{
	case fmc::BOX:
		shape = new btBoxShape(btVector3(btScalar(_scale.x*transform->scale.x*0.5f), btScalar(_scale.y*transform->scale.y*0.5f), btScalar(_scale.z*transform->scale.z*0.5f)));
		break;
	case fmc::SPHERE:
		shape = new btSphereShape(btScalar(_radius*transform->scale.x));//TODO fix
		break;
	default:
		break;
	}

	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
	{
		shape->calculateLocalInertia(_mass, localInertia);
	}

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, new btDefaultMotionState(object), shape, localInertia);
	_body = new btRigidBody(rbInfo);

}

void CBody3D::SetPosition(const fm::math::vec3 &inPosition)
{
	btTransform transform;
	if (_body && _body->getMotionState())
	{
		transform = _body->getCenterOfMassTransform();
	}
	
	transform.setOrigin(btVector3(inPosition.x, inPosition.y, inPosition.z));
	_body->setCenterOfMassTransform(transform);
}


void CBody3D::SetRotation(const fm::math::vec3 &inRotation)
{
	btTransform transform;
	if (_body && _body->getMotionState())
	{
		transform = _body->getCenterOfMassTransform();
	}

	transform.setRotation(btQuaternion(btScalar(inRotation.z), btScalar(inRotation.y), btScalar(inRotation.x)));
	_body->getMotionState()->setWorldTransform(transform);
}


void CBody3D::SetRadius(float radius)
{
	_radius = radius;
	_shape = SHAPE::SPHERE;
	if (_body)
	{
		_body->getCollisionShape()->setLocalScaling(btVector3(_radius, _radius, _radius));
	}
}


void CBody3D::SetScale(const fm::math::vec3 &inScale)
{
	_scale = inScale;
	_shape = SHAPE::BOX;
	if (_body)
	{
		_body->getCollisionShape()->setLocalScaling(btVector3(inScale.x, inScale.y, inScale.z));
	}
}


void CBody3D::GetPosition(fm::math::vec3& outVec) const
{
	btTransform transform;
	if (_body)
	{
		transform = _body->getWorldTransform();
	}
	btVector3 v = transform.getOrigin();
	
	outVec = fm::math::vec3(v.x(), v.y(), v.z());
}


void CBody3D::GetRotation(fm::math::vec3& outVec) const
{
	btTransform transform;
	if (_body)
	{
		transform = _body->getWorldTransform();
	}
	fm::math::vec3 v;
	transform.getRotation().getEulerZYX(outVec.z, outVec.y, outVec.x);
}



void CBody3D::AddToWorld(btDiscreteDynamicsWorld *inWorld)
{
	inWorld->addRigidBody(_body);
	_isInWorld = true;
}


const std::string& CBody3D::GetName() const
{
	return "Body3D";
}
void CBody3D::Destroy()
{
	EntityManager::get().removeComponent<fmc::CBody3D>(BaseComponent::_IDEntity);
}

bool CBody3D::Serialize(json &ioJson) const
{
	ioJson["shape"] = _shape;
	ioJson["mass"] = _mass;
	ioJson["radius"] = _radius;
	ioJson["scale"] = _scale;

	return true;
}
bool CBody3D::Read(const json &inJSON)
{
	_shape = inJSON["shape"];
	_mass = inJSON["mass"];
	_radius = inJSON["radius"];
	_scale = inJSON["scale"];

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
