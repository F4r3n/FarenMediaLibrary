#include "PhysicSystem.h"
#include "Body2D.h"
#include "CTransform.h"
#include "Event.h"
using namespace fms;
std::queue<Collider> PhysicSystem::collisions;
PhysicSystem::PhysicSystem() {
    gravity = b2Vec2(0.0f, 10.0f);
    world = std::unique_ptr<b2World>(new b2World(gravity));
    world->SetContactListener(&contactListener);
}

void PhysicSystem::update(float dt, EntityManager& em, EventManager& event) {
    world->Step(dt, 8, 2);
    //while(!collisions.empty()) {
    //    Collider c = collisions.front();
    //    collisions.pop();
    //    event.emit<Collider>(c);
    //}
    
    for(auto e : em.iterate<fmc::CTransform, fmc::Body2D>()) {
        fmc::Body2D* body = e->get<fmc::Body2D>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        //std::cout << body->body->GetPosition().y << std::endl;
        transform->position = fm::Vector2f(body->body->GetPosition().x, body->body->GetPosition().y);
        if(body->number_contact) {
           // std::cout << "Contact " << e->ID << std::endl;
        }
    }
}

void PhysicSystem::over() {
}

void PhysicSystem::init(EntityManager& em, EventManager& event) {

    for(auto e : em.iterate<fmc::CTransform, fmc::Body2D>()) {
        fmc::Body2D* body = e->get<fmc::Body2D>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        body->identity = &e->ID;
        body->bodyDef.position.Set(transform->position.x, transform->position.y);
        body->init(world.get());
    }
}

void PhysicSystem::pre_update(EntityManager& em) {
}

PhysicSystem::~PhysicSystem() {
}