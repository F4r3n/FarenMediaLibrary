#pragma once
#include "ECS.h"

class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btGhostPairCallback;
typedef float btScalar;
namespace fmc
{
	class CBody;
}
namespace fms
{
	class PhysicSystem : public System<PhysicSystem> {
	public:
		PhysicSystem();
		void update(float dt, EntityManager& em, EventManager& event);
		void over();
		void init(EntityManager& em, EventManager& event);
		void pre_update(EntityManager& em);
		~PhysicSystem();
		void Start();
		void Stop();

	private:
		void _InitAllBodies();
		//void _CheckCollision(btDynamicsWorld *world, btScalar timeStep);
		btDiscreteDynamicsWorld* _dynamicsWorld;
		btGhostPairCallback* _ghostPairCallback;
	};
}