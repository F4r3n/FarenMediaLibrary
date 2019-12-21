#pragma once
#include "System.h"
#include <memory>
#include <vector>
typedef std::vector<std::unique_ptr<BaseSystem> > MapOfSystems;

enum class SYSTEM_MANAGER_MODE
{
	RUNNING,
	STOPPED
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
	inline SYSTEM_MANAGER_MODE GetStatus() const { return _mode; }
private:
	bool ShouldCallSystem(SYSTEM_MODE inSystemMode) const;


	MapOfSystems systems;
	SYSTEM_MANAGER_MODE _mode;
};
