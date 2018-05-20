#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <iostream>
#include <typeinfo>
#include <memory>

#include "EntityManager.h"
#include "Component.h"
#include "Serializer.hpp"
class Entity
{

public:
    Entity();
    Entity(size_t ID);
    ~Entity();
    size_t ID;
    bool active = false;
    bool toCreate = true;
    bool allocated = false;
    template <typename T> T* addComponent(Component<T> *c = new T())
    {
       return EntityManager::get().addComponent<T>(this, c);
    }
    template <typename T> T* add()
    {
       return EntityManager::get().addComponent<T>(this, new T());
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
