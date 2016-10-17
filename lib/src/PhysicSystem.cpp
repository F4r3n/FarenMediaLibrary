#include "PhysicSystem.h"
#include "Body2D.h"
#include "CTransform.h"
using namespace fms;
PhysicSystem::PhysicSystem() {
    gravity = b2Vec2(0.0f, 10.0f);
    world = std::unique_ptr<b2World>(new b2World(gravity));
}

void PhysicSystem::update(float dt, EntityManager& em, EventManager& event) {
    world->Step(dt, 8, 2);
    for(auto e : em.iterate<fmc::CTransform, fmc::Body2D>()) {
        fmc::Body2D* body = e->get<fmc::Body2D>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        //std::cout << body->body->GetPosition().y << std::endl;
        transform->position = fm::Vector2f(body->body->GetPosition().x, body->body->GetPosition().y);
    }
}

void PhysicSystem::over() {
}

void PhysicSystem::init(EntityManager& em, EventManager& event) {
    std::cout << "Physic running ..." << std::endl;
    for(auto e : em.iterate<fmc::CTransform, fmc::Body2D>()) {
        fmc::Body2D* body = e->get<fmc::Body2D>();
        fmc::CTransform* transform = e->get<fmc::CTransform>();

        body->bodyDef.position.Set(transform->position.x, transform->position.y);
        body->init(world.get());
    }
}

void PhysicSystem::pre_update(EntityManager& em) {
}

PhysicSystem::~PhysicSystem() {
}