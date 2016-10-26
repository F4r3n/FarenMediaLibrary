#pragma once
#include <System.h>
#include <Entity.h>
#include <Box2D/Box2D.h>
#include "Body2D.h"
#include <iostream>
#include "Collider.h"
#include <queue>
#include "Event.h"
namespace fms {

class PhysicSystem : public System {
public:
    PhysicSystem();
    void update(float dt, EntityManager& em, EventManager& event);
    void over();
    void init(EntityManager& em, EventManager& event);
    void pre_update(EntityManager& em);
    ~PhysicSystem();
    
    static void beginEvent(size_t idA, size_t idB) {
        std::cout << idA << " " << idB << std::endl;
        EventManager::get().emit<Collider>(idA, idB, EVENT_COLLISION::BEGIN);
    }
    
    static void endEvent(size_t idA, size_t idB) {
        std::cout << idA << " " << idB << std::endl;
        EventManager::get().emit<Collider>(idA, idB, EVENT_COLLISION::END);
    }
    class ContactListener : public b2ContactListener {
        void BeginContact(b2Contact* contact) {

            // check if fixture A was a ball
            void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
            size_t idA = -1, idB = -1;
            if(bodyUserData) {
                fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                b->startContact();
                idA = *b->identity;
            }

            // check if fixture B was a ball
            bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
            if(bodyUserData) {
                  fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                b->startContact();
                idB = *b->identity;
            }
            PhysicSystem::beginEvent(idA, idB);
        }

        void EndContact(b2Contact* contact) {

            // check if fixture A was a ball
            void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
            size_t idA = -1, idB = -1;
            if(bodyUserData) {
                  fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                b->endContact();
                idA = *b->identity;
            }

            // check if fixture B was a ball
            bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
            if(bodyUserData) {
                   fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                b->endContact();
                idB = *b->identity;
            }
            PhysicSystem::endEvent(idA, idB);
        }
    };

private:
    b2Vec2 gravity;
    std::unique_ptr<b2World> world;
    ContactListener contactListener;
    
    static std::queue<Collider> collisions;
};
}