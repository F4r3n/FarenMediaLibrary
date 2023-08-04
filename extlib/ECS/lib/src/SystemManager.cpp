#include "SystemManager.h"
#include "EntityManager.h"


SystemManager::SystemManager() 
{
	_mode = SYSTEM_MANAGER_MODE::STOPPED;
}

void SystemManager::init(EntityManager& em, EventManager &event) 
{
    for(auto &s : systems) 
	{
        s->init(em, event);
    }
}

void SystemManager::Free()
{
    systems.clear();
}

bool SystemManager::ShouldCallSystem(SYSTEM_MODE inSystemMode) const
{
	return _mode == SYSTEM_MANAGER_MODE::RUNNING && inSystemMode == SYSTEM_MODE::AT_START
		|| _mode == SYSTEM_MANAGER_MODE::STOPPED && inSystemMode == SYSTEM_MODE::AT_STOP
		|| inSystemMode == SYSTEM_MODE::ALWAYS;
}


void SystemManager::update(float dt, EntityManager& em, EventManager &event)
{
    for(auto &s : systems)
	{
		if (ShouldCallSystem(s->_type))
		{
			s->pre_update(em);
		}
    }

	for (auto &s : systems)
	{
		if (ShouldCallSystem(s->_type))
		{
			s->update(dt, em, event);
		}
	}

	for (auto &s : systems)
	{
		if (ShouldCallSystem(s->_type))
		{
			s->over();
		}
	}
}


void SystemManager::Stop()
{
	_mode = SYSTEM_MANAGER_MODE::STOPPED;
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

	_mode = SYSTEM_MANAGER_MODE::RUNNING;
}

