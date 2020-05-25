#pragma once
#include <System.h>
#include <EntityManager.h>

#include <Box2D/Box2D.h>
#include "Components/CBody2D.h"
#include "Collider.h"
#include "Event.h"
namespace fms {

class PhysicSystem2D : public System<PhysicSystem2D> {
public:
    PhysicSystem2D();
    void update(float dt, EntityManager& em, EventManager& event);
    void over();
    void init(EntityManager& em, EventManager& event);
    void pre_update(EntityManager& em);
    ~PhysicSystem2D();


    class ContactListener : public b2ContactListener {

        void PreSolve(b2Contact* contact, const b2Manifold*)
		{
            void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
           // size_t idA = -1, idB = -1;
            if(bodyUserData)
			{
                //fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
                //contact->GetFixtureA()->SetFriction()
            }

            bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
            if(bodyUserData)
			{
                //fmc::Body2D* b = static_cast<fmc::Body2D*>(bodyUserData);
            }
        }

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);

        void PostSolve(b2Contact * contact, const b2ContactImpulse * impulse) {
            
        }
    };
	void Start();
	void Stop();

	constexpr static float M2P() { return 60.0f; }
	constexpr static float P2M() { return 1 / M2P(); }
private:
    b2Vec2 _gravity;
    std::unique_ptr<b2World> _world;
    ContactListener _contactListener;
};
}
