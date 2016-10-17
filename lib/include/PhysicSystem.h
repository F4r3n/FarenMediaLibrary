#pragma once
#include <System.h>
#include <Entity.h>
#include <Box2D/Box2D.h>

namespace fms {
class PhysicSystem : public System {
public:
    PhysicSystem();
    void update(float dt, EntityManager& em, EventManager& event);
    void over();
    void init(EntityManager& em, EventManager& event);
    void pre_update(EntityManager& em);
    ~PhysicSystem();
    private:
    b2Vec2 gravity;
    std::unique_ptr<b2World> world;
};
}