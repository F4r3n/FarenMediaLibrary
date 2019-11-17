#include "Physic/PhysicSystem3D.h"
#include "btBulletDynamicsCommon.h"
#include "Components/CTransform.h"
#include "Components/CBody3D.h"
#include "Components/CCollider.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

using namespace fms;

PhysicSystem3D::PhysicSystem3D()
{
}




void PhysicSystem3D::init(EntityManager& em, EventManager& event)
{
}

void _CheckCollision(btDynamicsWorld *world, btScalar timeStep)
{
	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; ++j)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				fmc::CBody3D* bodyA = static_cast<fmc::CBody3D*>(obA->getUserPointer());
				fmc::CBody3D* bodyB = static_cast<fmc::CBody3D*>(obA->getUserPointer());

			}
		}
	}
}



void PhysicSystem3D::pre_update(EntityManager& em)
{

	for (auto&& e : em.iterate<fmc::CTransform, fmc::CCollider, fmc::CBody3D>())
	{
		fmc::CCollider* collider = e->get<fmc::CCollider>();
		fmc::CTransform *ctransform = e->get<fmc::CTransform>();


		fm::Transform tr = ctransform->GetTransform();
		if (collider != nullptr && !collider->IsInit())
		{
			collider->Init(tr);
		}

		if (collider != nullptr && collider->IsInit())
		{
			fmc::CBody3D *cbody = e->get<fmc::CBody3D>();
			if (cbody != nullptr)
			{
				if (!cbody->IsInit())
				{
					cbody->Init(collider);


					cbody->SetPosition(tr.position);
					cbody->SetRotation(tr.rotation);
					cbody->AddToWorld(_dynamicsWorld);


				}
			}
			fmc::CTransform *ctransform = e->get<fmc::CTransform>();


			cbody->SetPosition(ctransform->position);
			cbody->SetRotation(ctransform->GetRotation());
		}


		//cbody->SetScale(ctransform->scale);

	}
}

void PhysicSystem3D::update(float dt, EntityManager& em, EventManager& event)
{
	_dynamicsWorld->stepSimulation(1 / 60.0f, 10);

	for (auto &&e : em.iterate<fmc::CTransform, fmc::CBody3D, fmc::CCollider>())
	{
		fmc::CBody3D *cbody = e->get<fmc::CBody3D>();
		fmc::CTransform *ctransform = e->get<fmc::CTransform>();

		cbody->GetPosition(ctransform->position);
		fm::math::Quaternion q = ctransform->GetRotation();
		cbody->GetRotation(q);
		ctransform->SetRotation(q);
	}
}

void PhysicSystem3D::Start()
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
	_dynamicsWorld->setInternalTickCallback(_CheckCollision, this, true);
	_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

}

void PhysicSystem3D::Stop() 
{
	if (!_dynamicsWorld) return;

	for (int i = _dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		
		_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	delete static_cast<btCollisionDispatcher*>(_dynamicsWorld->getDispatcher())->getCollisionConfiguration();
	delete _dynamicsWorld->getDispatcher();
	delete _dynamicsWorld->getConstraintSolver();
	delete _dynamicsWorld->getBroadphase();
	delete _dynamicsWorld;
}

void PhysicSystem3D::over()
{
}


PhysicSystem3D::~PhysicSystem3D()
{

}