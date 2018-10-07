#pragma once
#include "System.h"
#include <memory>
#include <typeinfo>
#include <cassert>
#include "EntityManager.h"
#include "Event.h"
class SystemManager {
public:
    
    SystemManager() {}
    ~SystemManager() {
    }

template <typename T> 
    T* addSystem(System<T> *system) {
        systems[T::id()].reset(system);
        return dynamic_cast<T*>(systems[T::id()].get());
    }
    
    template <typename T>
    T* getSystem() {
        if(systems.find(T::id()) == systems.end()) return nullptr;
        return dynamic_cast<T*>(systems[T::id()].get()); 
    }
    

    void update(float dt, EntityManager& em, EventManager &event);
    void init(EntityManager& em, EventManager &event);
    void Free();

private:
    std::unordered_map<size_t, std::unique_ptr<BaseSystem> > systems;
};
