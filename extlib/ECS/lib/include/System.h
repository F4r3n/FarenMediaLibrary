#ifndef SYSTEM_H
#define SYSTEM_H
#include <cstddef>
#include "Config.h"

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
	friend class BaseComponent;

    BaseSystem(){}
    virtual ~BaseSystem() {}
    virtual void pre_update(EntityManager &manager) = 0;
    virtual void update(float dt, EntityManager &manager, EventManager &event) = 0;
    virtual void init(EntityManager &manager, EventManager &event) = 0;
    virtual void over() = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
protected:
    static std::size_t family_counter;
	//bool _IsAffectedByStartAndStop = true;
	SYSTEM_MODE _type = AT_START;
};


template <class T> class System : public BaseSystem {

public:
    System() {}
    virtual ~System(){}
    
    
    static size_t id() 
	{
        static size_t i = family_counter++;
        return i;
    }
    
    
    friend class SystemManager;

};

#endif
