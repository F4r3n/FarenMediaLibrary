#pragma once
#include "System.h"
#include <memory>
#include <typeinfo>
#include <cassert>
#include "EntityManager.h"
#include "Event.h"
typedef std::vector<std::unique_ptr<BaseSystem> > MapOfSystems;

class SystemManager
{
public:
    
	SystemManager();
    ~SystemManager() {}

template <typename T> 
    void addSystem(System<T> *system) 
	{
		systems.emplace_back(system);
    }
    

    void update(float dt, EntityManager& em, EventManager &event);
	void init(EntityManager& em, EventManager &inEvent);
    void Free();
	void Stop();
	void Start();

private:
	MapOfSystems systems;
	bool _isRunning = false;
};
