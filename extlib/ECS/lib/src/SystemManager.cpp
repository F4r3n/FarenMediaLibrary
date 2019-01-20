#include "SystemManager.h"
#include "EntityManager.h"


SystemManager::SystemManager() 
{
	_isRunning = false;
}

void SystemManager::init(EntityManager& em, EventManager &event) 
{
    EntityManager::get().make();
    for(auto &s : systems) 
	{
        s->init(em, event);
    }
    em.make();
}

void SystemManager::Free()
{
    systems.clear();
}

void SystemManager::update(float dt, EntityManager& em, EventManager &event)
{
    for(auto &s : systems)
	{
		if (!s->_IsAffectedByStartAndStop || (s->_IsAffectedByStartAndStop && _isRunning))
		{
			s->pre_update(em);
		}
        em.make();
    }

	for (auto &s : systems)
	{
		if (!s->_IsAffectedByStartAndStop || (s->_IsAffectedByStartAndStop && _isRunning))
		{
			s->update(dt, em, event);
		}
		em.make();
	}

	for (auto &s : systems)
	{
		if (!s->_IsAffectedByStartAndStop || (s->_IsAffectedByStartAndStop && _isRunning))
		{
			s->over();
		}
		em.make();
	}
}


void SystemManager::Stop()
{
	_isRunning = false;
	for (auto &s : systems)
	{
		s->Stop();
	}
}


void SystemManager::Start()
{
	for (auto &s : systems)
	{
		s->Start();
	}
	_isRunning = true;
}
