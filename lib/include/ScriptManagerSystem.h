#pragma once
#include <System.h>
#include <Entity.h>
#include <Event.h>
#include "EventComponents.h"
#include "Collider.h"
namespace fms {

class ScriptManagerSystem : public System<ScriptManagerSystem>, public Receiver<ScriptManagerSystem> {
public:
    ScriptManagerSystem();
    ~ScriptManagerSystem();
    void pre_update(EntityManager& em) {
    }
    void update(float dt, EntityManager& em, EventManager& event);
    void init(EntityManager& em, EventManager& event);
    void over() {
    }
    void receive(const Collider& collider);
    
private:

    void processCollision(size_t idA, size_t idB, EVENT_COLLISION event);
    
};
}