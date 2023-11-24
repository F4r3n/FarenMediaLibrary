#include "Physic/PhysicSystem.h"
#include "btBulletDynamicsCommon.h"
#include "Components/CTransform.h"
#include "Components/CBody.h"
#include "Components/CCollider.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "Components/cevent.hpp"
#include <entt/entt.hpp>
using namespace fms;

PhysicSystem::PhysicSystem()
{
}




void PhysicSystem::init(EventManager&)
{
}

void _CheckCollision(btDynamicsWorld* world, [[maybe_unused]] btScalar timeStep)
{
	int numManifolds = world->getDispatcher()->getNumManifolds();
	bool hasFoundOneCollision = false;
	for (int i = 0; i < numManifolds; ++i)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; ++j)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() <= 0.f)
			{

				fmc::CBody* bodyA = static_cast<fmc::CBody*>(obA->getUserPointer());
				fmc::CBody* bodyB = static_cast<fmc::CBody*>(obB->getUserPointer());

				entt::handle entityA = bodyA->GetEntity();
				entt::handle entityB = bodyB->GetEntity();

				fm::math::vec3 normalB = fm::math::vec3(pt.m_normalWorldOnB.x(), pt.m_normalWorldOnB.y(), pt.m_normalWorldOnB.z());

				if (entityA.valid())
				{
					fmc::CEvent& event = entityA.get_or_emplace<fmc::CEvent>();

					fm::math::vec3 worldPosA = fm::math::vec3(pt.m_positionWorldOnA.x(), pt.m_positionWorldOnA.y(), pt.m_positionWorldOnA.z());
					event.AddEvent(new fm::CollisionEvent(entityB, worldPosA, normalB));
					hasFoundOneCollision = true;
				}

				if (entityB.valid())
				{
					fmc::CEvent& event = entityB.get_or_emplace<fmc::CEvent>();
					fm::math::vec3 worldPosB = fm::math::vec3(pt.m_positionWorldOnB.x(), pt.m_positionWorldOnB.y(), pt.m_positionWorldOnB.z());
					event.AddEvent(new fm::CollisionEvent(entityA, worldPosB, normalB));
				}
				if (hasFoundOneCollision)
					break;
			}
		}
		if (hasFoundOneCollision)
			break;
	}
}

void PhysicSystem::_InitAllBodies(entt::registry& registry)
{
	auto view = registry.view<fmc::CTransform, fmc::CBody, fmc::CCollider>();

	for (auto e : view)
	{
		fmc::CCollider& collider = registry.get<fmc::CCollider>(e);
		fmc::CTransform& ctransform = registry.get<fmc::CTransform>(e);


		fm::Transform tr = ctransform.GetTransform();
		if (!collider.IsInit())
		{
			collider.Init(tr.worldTransform.Scale());
		}

		if (collider.IsInit())
		{
			fmc::CBody& cbody = registry.get<fmc::CBody>(e);

			if (!cbody.IsInit())
			{
				cbody.Init(&collider, ctransform, entt::handle(registry, e));

				cbody.SetPosition(tr.worldTransform.Position());
				cbody.SetRotation(tr.worldRotation);
				cbody.AddToWorld(_dynamicsWorld);
			}
			
			//if(tr.position)
			cbody.SetPosition(tr.worldTransform.Position());
			cbody.SetRotation(tr.worldRotation);
		}
	}
}


void PhysicSystem::update(float dt, entt::registry& registry, EventManager&)
{
	if (!_dynamicsWorld)
		return;
	auto view = registry.view<fmc::CTransform, fmc::CBody, fmc::CCollider>();
	_InitAllBodies(registry);
	_dynamicsWorld->stepSimulation(dt, 10);

	for (auto&& e : view)
	{
		fmc::CBody& cbody = registry.get<fmc::CBody>(e);
		fmc::CTransform& ctransform = registry.get<fmc::CTransform>(e);

		fm::math::vec3 pos;
		cbody.GetPosition(pos);
		ctransform.SetWorldPosition(pos);

		fm::math::Quaternion q;
		cbody.GetRotation(q);
		ctransform.SetWorldRotation(q);
	}
}

void PhysicSystem::Start(entt::registry& registry)
{
	btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();

	//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);

	//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	//the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, config);
	_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	//_dynamicsWorld->setInternalTickCallback(_CheckCollision, this, true);
	_dynamicsWorld->setInternalTickCallback(_CheckCollision, this, false);
	_dynamicsWorld->getDispatchInfo().m_useContinuous = true;
	_dynamicsWorld->getSolverInfo().m_splitImpulse = false; // Disable by default for performance

	_ghostPairCallback = new btGhostPairCallback();
	_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(_ghostPairCallback);
	_dynamicsWorld->setSynchronizeAllMotionStates(true);


	_InitAllBodies(registry);

}

void PhysicSystem::Stop(entt::registry&)
{
	if (!_dynamicsWorld) return;

	for (int i = _dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body != nullptr && body->getMotionState() != nullptr)
		{
			delete body->getMotionState();
		}
		_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	delete static_cast<btCollisionDispatcher*>(_dynamicsWorld->getDispatcher())->getCollisionConfiguration();
	delete _dynamicsWorld->getDispatcher();
	delete _dynamicsWorld->getConstraintSolver();
	delete _ghostPairCallback;
	delete _dynamicsWorld->getBroadphase();
	delete _dynamicsWorld;
	_dynamicsWorld = nullptr;
	_ghostPairCallback = nullptr;
}

void PhysicSystem::over()
{
}


PhysicSystem::~PhysicSystem()
{

}