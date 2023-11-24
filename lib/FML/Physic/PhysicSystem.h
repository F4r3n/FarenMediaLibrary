#pragma once
#include "Core/System/System.hpp"

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
		void update(float dt, entt::registry& registry, EventManager&) override;
		void init(EventManager& event);
		~PhysicSystem();
		void Start(entt::registry& registry) override;
		void Stop(entt::registry& registry) override;
		void over();

	private:
		void _InitAllBodies(entt::registry& registry);
		//void _CheckCollision(btDynamicsWorld *world, btScalar timeStep);
		btDiscreteDynamicsWorld* _dynamicsWorld;
		btGhostPairCallback* _ghostPairCallback;
	};
}