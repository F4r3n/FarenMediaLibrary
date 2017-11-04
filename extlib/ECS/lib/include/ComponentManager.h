#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include "Component.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <array>
#include <iostream>
#include <bitset>
#include "Config.h"

class ComponentManager
{
public:
    ComponentManager() {}
    ~ComponentManager() {}

    template <typename T> T* getComponent()
    {
        if(!has<T>()) {
            return nullptr;
        }
        return dynamic_cast<T*>(components[T::id()].get());
    }

    template <typename T> T* addComponent(Component<T> *c)
    {
        bits.set(T::id(), 1);
        components[T::id()].reset(c);
        return dynamic_cast<T*>(components[T::id()].get());
    }

    template <typename T>
    bool has()
    {
       return bits.test(T::id());
    }
    bool has(size_t id)
    {
       return bits.test(id);
    }
    
    bool has(const std::bitset<MAX_COMPONENTS> &compo) const{
        if(!compo.any()) return false;
        return ((bits & compo)  == compo);
    }
    
    template <typename T>
    bool remove() {
        
        bits.set(T::id(), 0);
        components[T::id()].reset();
        return true;
    }
    
    void resetMask() {
        bits.reset();
    }
    
    std::vector<BaseComponent*> getAllComponents() {
        std::vector<BaseComponent*> temp;
        for(auto &c : components) {
            temp.push_back(c.second.get());
        }
        return temp;
    }

private:
    std::bitset<MAX_COMPONENTS> bits;
    std::unordered_map<size_t, std::unique_ptr<BaseComponent> > components;
};

#endif
