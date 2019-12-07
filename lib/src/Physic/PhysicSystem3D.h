#pragma once
#include "ECS.h"

class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btGhostPairCallback;
typedef float btScalar;
namespace fmc
{
	class CBody3D;
}
namespace fms
{
class PhysicSystem3D : public System<PhysicSystem3D> {
public:
	PhysicSystem3D();
	void update(float dt, EntityManager& em, EventManager& event);
	void over();
	void init(EntityManager& em, EventManager& event);
	void pre_update(EntityManager& em);
	~PhysicSystem3D();
	void Start();
	void Stop();

private:
	//void _CheckCollision(btDynamicsWorld *world, btScalar timeStep);
	btDiscreteDynamicsWorld*	_dynamicsWorld;
	btGhostPairCallback*		_ghostPairCallback;
};
}