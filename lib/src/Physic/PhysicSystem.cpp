#include "Physic/PhysicSystem.h"
#include "Components/Body2D.h"
#include "Components/CTransform.h"
#include "Event.h"

using namespace fms;
PhysicSystem::PhysicSystem()
{
    gravity = b2Vec2(0.0f, 5.0f);
    world = std::unique_ptr<b2World>(new b2World(gravity));
    world->SetContactListener(&contactListener);
}

void PhysicSystem::update(float dt, EntityManager& em, EventManager& event)
{
	assert(false);//Quaternion issue

    world->Step(dt, 8, 2);
    
    for(auto &&e : em.iterate<fmc::CTransform, fmc::Body2D>())
    {
        fmc::Body2D* body = e->get<fmc::Body2D>();
        if(!body->isReady) {
            body->Init(world.get(), P2M);
        }
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        transform->position = fm::math::vec2(body->body->GetPosition().x - body->size.x*P2M, 
        body->body->GetPosition().y - body->size.y*P2M)*M2P;
        //transform->rotation.x = body->body->GetAngle();
    }
}

void PhysicSystem::over()
{
}

void PhysicSystem::init(EntityManager& em, EventManager& event)
{
	assert(false);//Quaternion issue
    for(auto &&e : em.iterate<fmc::CTransform, fmc::Body2D>())
    {
        fmc::Body2D* body = e->get<fmc::Body2D>();
        fm::Transform &&transform = e->get<fmc::CTransform>()->GetTransform();
        body->identity = &e->ID;
        //body->body->SetTransform(b2Vec2((transform.position.x + body->size.x)*P2M, (transform.position.y + body->size.y)*P2M), transform.rotation.x);
        body->Init(world.get(), P2M);
    }
}

void PhysicSystem::pre_update(EntityManager& em)
{
}

PhysicSystem::~PhysicSystem()
{
}
