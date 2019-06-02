#include "SystemManager.h"
#include "EntityManager.h"


SystemManager::SystemManager() 
{
	_mode = SYSTEM_MANAGER_MODE::STOP;
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
		if (s->_type == SYSTEM_MODE::ALWAYS || (_mode == s->_type))
		{
			s->pre_update(em);
		}
        em.make();
    }

	for (auto &s : systems)
	{
		if (s->_type == SYSTEM_MODE::ALWAYS || (_mode == s->_type))
		{
			s->update(dt, em, event);
		}
		em.make();
	}

	for (auto &s : systems)
	{
		if (s->_type == SYSTEM_MODE::ALWAYS || (_mode == s->_type))
		{
			s->over();
		}
		em.make();
	}
}


void SystemManager::Stop()
{
	_mode = SYSTEM_MANAGER_MODE::STOP;
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
	_mode = SYSTEM_MANAGER_MODE::START;
}
