#pragma once
#include <cstddef>
#include <entt/fwd.hpp>
class EventManager;
class ComponentManager;
class Entity;
class EntityManager;

enum SYSTEM_MODE
{
	ALWAYS,
	AT_START,
	AT_STOP
};


class BaseSystem {
	friend class SystemManager;
public:
	BaseSystem() {}
	virtual ~BaseSystem() {}
	virtual void update(float dt, entt::registry& manager, EventManager& event) = 0;
	virtual void init(EventManager& event) = 0;
	virtual void Start(entt::registry& registry) = 0;
	virtual void Stop(entt::registry& registry) = 0;
protected:
	static std::size_t family_counter;
	//bool _IsAffectedByStartAndStop = true;
	SYSTEM_MODE _type = AT_START;
};


template <class T> class System : public BaseSystem {

public:
	System() {}
	virtual ~System() {}


	static size_t id()
	{
		static size_t i = family_counter++;
		return i;
	}


	friend class SystemManager;

};

