#include "Physic/PhysicSystem.h"
#include "Components/CBody2D.h"
#include "Components/CTransform.h"
#include "Event.h"
#include "Components/CCollider2D.h"
#include "Components/cevent.hpp"
#include "Components/CBody3D.h"
using namespace fms;
PhysicSystem2D::PhysicSystem2D()
{
    _gravity = b2Vec2(0.0f, 9.81f);

}

void PhysicSystem2D::Start()
{
	_world = std::unique_ptr<b2World>(new b2World(_gravity));
	_world->SetContactListener(&_contactListener);
}

void PhysicSystem2D::Stop()
{
	_world.reset();
}

void PhysicSystem2D::update(float dt, EntityManager& em, EventManager& event)
{
    _world->Step(dt, 8, 2);
    
    for(auto &&e : em.iterate<fmc::CTransform, fmc::CBody2D, fmc::CCollider2D>())
    {
        fmc::CBody2D* body = e->get<fmc::CBody2D>();
		fmc::CCollider2D* collider = e->get<fmc::CCollider2D>();

        if(!body->GetBody())
		{
            body->Init(collider, _world.get());
        }
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        transform->position = fm::math::vec2(body->GetBody()->GetPosition().x - collider->GetScale().x*P2M(), 
											 body->GetBody()->GetPosition().y - collider->GetScale().x *P2M())*M2P();
		fm::math::vec3 euler = transform->GetRotation().GetEulerAngles();
		euler.z = body->GetBody()->GetAngle();
		transform->SetRotation(fm::math::Quaternion::FromEulerAngles(euler));
    }
}

void PhysicSystem2D::over()
{
}

void PhysicSystem2D::init(EntityManager& em, EventManager& event)
{

}

void PhysicSystem2D::pre_update(EntityManager& em)
{
	for (auto&& e : em.iterate<fmc::CTransform, fmc::CBody2D, fmc::CCollider2D>())
	{
		fmc::CBody2D* body = e->get<fmc::CBody2D>();
		fm::Transform&& transform = e->get<fmc::CTransform>()->GetTransform();
		fmc::CCollider2D* collider = e->get<fmc::CCollider2D>();

		
		//body->identity = &e->ID;
		//body->body->SetTransform(b2Vec2((transform.position.x + body->size.x)*P2M, (transform.position.y + body->size.y)*P2M), transform.rotation.x);
		if (body->GetBody() != nullptr)
		{
			body->Init(collider, _world.get());

			body->GetBody()->SetTransform(b2Vec2(transform.position.x, transform.position.y), transform.rotation.GetEulerAngles().z);
		}
	}
}

PhysicSystem2D::~PhysicSystem2D()
{
}

void PhysicSystem2D::ContactListener::BeginContact(b2Contact* contact)
{
	fmc::CBody2D* bodyA = static_cast<fmc::CBody2D*>(contact->GetFixtureA()->GetBody()->GetUserData());
	fmc::CBody2D* bodyB = static_cast<fmc::CBody2D*>(contact->GetFixtureA()->GetBody()->GetUserData());

	if (bodyA != nullptr && bodyB != nullptr)
	{
		Entity* entityA = EntityManager::get().getEntity(bodyA->GetIDEntity());
		Entity* entityB = EntityManager::get().getEntity(bodyB->GetIDEntity());

		fmc::CEvent* eventA = nullptr;
		if (!entityA->has<fmc::CEvent>())
		{
			eventA = entityA->addComponent<fmc::CEvent>();
		}
		else
		{
			eventA = entityA->get<fmc::CEvent>();
		}

		fmc::CEvent* eventB = nullptr;
		if (!entityB->has<fmc::CEvent>())
		{
			eventB = entityB->addComponent<fmc::CEvent>();
		}
		else
		{
			eventB = entityB->get<fmc::CEvent>();
		}

		fm::math::vec3 worldPosA = fm::math::vec3(contact->GetManifold()->localPoint.x, contact->GetManifold()->localPoint.y, entityA->get<fmc::CTransform>()->GetTransform().position.z);
		eventA->AddEvent(new fm::CollisionEvent(entityB->ID, worldPosA, fm::math::vec3(0,0,0)));
		eventB->AddEvent(new fm::CollisionEvent(entityA->ID, worldPosA, fm::math::vec3(0, 0, 0)));

	}


}

void PhysicSystem2D::ContactListener::EndContact(b2Contact* contact)
{

	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	size_t idA = -1, idB = -1;
	if (bodyUserData)
	{
		fmc::CBody2D* b = static_cast<fmc::CBody2D*>(bodyUserData);
	}

	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData) {
		fmc::CBody2D* b = static_cast<fmc::CBody2D*>(bodyUserData);
	}
}
