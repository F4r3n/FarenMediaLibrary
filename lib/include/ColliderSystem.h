#pragma once
#include "System.h"
#include "QuadTree.h"
#include "CCollider.h"
namespace fms {
class ColliderSystem : public System {
public:
    ColliderSystem();
    ~ColliderSystem();
    void update(float dt, std::shared_ptr<Entity> e);
    void init(std::shared_ptr<Entity> e);
    void over();

private:
    std::unique_ptr<fm::QuadTree<fmc::CCollider>> quadTree;
};
}