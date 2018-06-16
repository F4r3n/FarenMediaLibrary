#ifndef SYSTEM_H
#define SYSTEM_H
#include <vector>
#include <string>
#include <memory>
#include <bitset>
#include "Config.h"
#include <cstddef>
#include <typeinfo>
class EventManager;
class ComponentManager;
class Entity;
class EntityManager;

class BaseSystem {
public:
    BaseSystem(){}
    virtual ~BaseSystem() {}
    virtual void pre_update(EntityManager &manager) = 0;
    virtual void update(float dt, EntityManager &manager, EventManager &event) = 0;
    virtual void init(EntityManager &manager, EventManager &event) = 0;
    virtual void over() = 0;
protected:
    static std::size_t family_counter;
};


template <class T> class System : public BaseSystem {

public:
    System() {
    }
    virtual ~System() {
    }
    
    
    static size_t id() {
        static size_t i = family_counter++;
        return i;
    }
    
    
    friend class SystemManager;
   

protected:
    std::bitset<MAX_COMPONENTS> bits;

private:
    std::vector<size_t> componentsNeeded;
};

#endif
