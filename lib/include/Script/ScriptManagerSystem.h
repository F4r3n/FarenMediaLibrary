#pragma once
#include <System.h>
#include <EntityManager.h>

#include <Event.h>
#include "EventComponents.h"
#include "Physic/Collider.h"
#include "Rendering/RenderingEvent.h"
namespace fms {

class ScriptManagerSystem : public System<ScriptManagerSystem> {
public:
    ScriptManagerSystem();
    ~ScriptManagerSystem();
    void pre_update(EntityManager&) {
    }
    void update(float dt, EntityManager& em, EventManager& event);
    void init(EntityManager& em, EventManager& event);
    void over() {
    }

	void Start();
	void Stop();
private:

    void processCollision(size_t idA, size_t idB, EVENT_COLLISION event);
    
};
}
