#include "Components/CBody3D.h"
#include "btBulletDynamicsCommon.h"

using namespace fmc;

CBody3D::CBody3D(float w, float h, float l)
{
	_shape = new btBoxShape(btVector3(btScalar(w), btScalar(h), btScalar(l)));
	_isInWorld = false;
}
void CBody3D::SetMass(float inMass)
{
	_mass = inMass;
}

bool CBody3D::IsInit() const
{
	return _body != nullptr && !_isInWorld;
}


void CBody3D::Init()
{
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (_mass != 0.f);


	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 0, 0));

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		_shape->calculateLocalInertia(_mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, myMotionState, _shape, localInertia);
	_body = new btRigidBody(rbInfo);

}

void CBody3D::SetPosition(const fm::math::vec3 &inPosition)
{
	btTransform transform;
	if (_body && _body->getMotionState())
	{
		_body->getMotionState()->getWorldTransform(transform);
	}
	
	transform.setOrigin(btVector3(inPosition.x, inPosition.y, inPosition.y));
	_body->getMotionState()->setWorldTransform(transform);
}


void CBody3D::SetRotation(const fm::math::vec3 &inRotation)
{
	btTransform transform;
	if (_body && _body->getMotionState())
	{
		_body->getMotionState()->getWorldTransform(transform);
	}

	transform.setRotation(btQuaternion(btScalar(inRotation.z), btScalar(inRotation.y), btScalar(inRotation.x)));
	_body->getMotionState()->setWorldTransform(transform);
}


void CBody3D::GetPosition(fm::math::vec3& outVec) const
{
	btTransform transform;
	if (_body && _body->getMotionState())
	{
		_body->getMotionState()->getWorldTransform(transform);
	}
	btVector3 v = transform.getOrigin();
	
	outVec = fm::math::vec3(v.x(), v.y(), v.z());
}


void CBody3D::GetRotation(fm::math::vec3& outVec) const
{
	btTransform transform;
	if (_body && _body->getMotionState())
	{
		_body->getMotionState()->getWorldTransform(transform);
	}
	fm::math::vec3 v;
	transform.getRotation().getEulerZYX(outVec.z, outVec.y, outVec.x);
}

void CBody3D::AddToWorld(btDiscreteDynamicsWorld *inWorld)
{
	inWorld->addRigidBody(_body);
	_isInWorld = true;
}



CBody3D::~CBody3D()
{
	delete _shape;
}
