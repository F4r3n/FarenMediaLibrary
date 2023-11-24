#pragma once
#include "Core/System/System.hpp"


namespace fms {

class ScriptManagerSystem : public System<ScriptManagerSystem> {
public:
    ScriptManagerSystem();
    ~ScriptManagerSystem();
	virtual void update(float dt, entt::registry& registry, EventManager&) override;
	virtual void init(EventManager& event) override;

	virtual void Start(entt::registry& registry) override;
	virtual void Stop(entt::registry& registry) override;
private:

    
};
}
