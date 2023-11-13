#pragma once
#include <System.h>
#include <EntityManager.h>

#include <Event.h>


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

    
};
}
