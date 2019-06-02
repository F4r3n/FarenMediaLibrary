#pragma once
#include "System.h"
#include <memory>
#include <vector>
typedef std::vector<std::unique_ptr<BaseSystem> > MapOfSystems;

enum SYSTEM_MANAGER_MODE
{
	START = 1,
	STOP = 2
};

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
	SYSTEM_MANAGER_MODE _mode;
};
