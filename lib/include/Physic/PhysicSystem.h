#pragma once
#include <System.h>
#include <Entity.h>
#include <Box2D/Box2D.h>
#include "Components/Body2D.h"
#include <iostream>
#include "Collider.h"
#include <queue>
#include "Event.h"
namespace fms {

class PhysicSystem : public System<PhysicSystem> {
public:
    PhysicSystem();
    void update(float dt, EntityManager& em, EventManager& event);
    void over();
    void init(EntityManager& em, EventManager& event);
    void pre_update(EntityManager& em);
    ~PhysicSystem();

    static void beginEvent(size_t idA, size_t idB) {
        EventManager::get().emit<Collider>(idA, idB, EVENT_COLLISION::BEGIN);
    }

    static void endEvent(size_t idA, size_t idB) {
        EventManager::get().emit<Collider>(idA, idB, EVENT_COLLISION::END);
    }
    class ContactListener : public b2ContactListener {

        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
            void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
           // size_t idA = -1, idB = -1;
            if(bodyUserData) {
                //fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                //contact->GetFixtureA()->SetFriction()
            }

            bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
            if(bodyUserData) {
                //fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
            }
        }

        void BeginContact(b2Contact* contact) {
            
            void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
            size_t idA = -1, idB = -1;
            if(bodyUserData) {
                fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                b->StartContact();
                idA = *b->identity;
            }

            bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
            if(bodyUserData) {
                fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                b->StartContact();
                idB = *b->identity;
            }
            PhysicSystem::beginEvent(idA, idB);
        }

        void EndContact(b2Contact* contact) {

            void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
            size_t idA = -1, idB = -1;
            if(bodyUserData) {
                fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                b->EndContact();
                idA = *b->identity;
            }

            bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
            if(bodyUserData) {
                fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                b->EndContact();
                idB = *b->identity;
            }
            PhysicSystem::endEvent(idA, idB);
        }

        void PostSolve(b2Contact * contact, const b2ContactImpulse * impulse) {
            
        }
    };

private:
    b2Vec2 gravity;
    std::unique_ptr<b2World> world;
    ContactListener contactListener;

    const float M2P = 60.0f;
    const float P2M = 1 / M2P;
};
}
