#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <iostream>
#include <typeinfo>
#include <memory>

#include "EntityManager.h"
#include "Component.h"

class Entity
{

public:
    Entity();
    Entity(size_t ID);
    ~Entity();
    size_t ID = std::numeric_limits<size_t>::max();
    bool active = false;
    bool toCreate = true;


    template <typename T, typename ...Args> T* addComponent(Args&&... args)
    {
       return EntityManager::get().addComponent<T>(this, args...);
    }
    template <typename T> T* add()
    {
       return EntityManager::get().add<T>(this, new T());
    }

    template <typename T> T* addEmpty()
    {
       return EntityManager::get().add<T>(this, new T());
    }
    template <typename T> T* add(Component<T> *c)
    {
       return EntityManager::get().add<T>(this, c);
    }

    template <typename T> T* get()
    {
        return EntityManager::get().get<T>(this);
    }
    
    template <typename T> bool has()
    {
        return EntityManager::get().hasComponent<T>(ID);
    }
    
    template <typename T> bool remove()
    {
        return EntityManager::get().removeComponent<T>(ID);
    }
    
    std::vector<BaseComponent*> getAllComponents() {
        return EntityManager::get().getAllComponents(this);
    }
    
    void destroy();

private:
};

#endif
