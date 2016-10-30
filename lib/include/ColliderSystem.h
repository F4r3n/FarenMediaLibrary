#pragma once
#include "System.h"
#include "QuadTree.h"
#include "Components/CCollider.h"
namespace fms {
class ColliderSystem : public System {
public:
    ColliderSystem();
    ~ColliderSystem();
    void update(float dt, EntityManager& em, EventManager &event);
    void init(EntityManager& em, EventManager &event);
    void over();
    void pre_update(EntityManager& em);

private:
    std::unique_ptr<fm::QuadTree> quadTree;
};
}