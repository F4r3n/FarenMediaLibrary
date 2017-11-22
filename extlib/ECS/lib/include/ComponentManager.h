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
        ~ComponentManager() {
            removeAll();
        }

        template <typename T> T* getComponent()
        {
            if(!has<T>()) {
                return nullptr;
            }
            return dynamic_cast<T*>(_components[T::id()]);
        }

        template <typename T> T* addComponent(Component<T> *c)
        {
            _bits.set(T::id(), 1);
            _components[T::id()] = std::move(c);
            return dynamic_cast<T*>(_components[T::id()]);
        }

        template <typename T>
            bool has()
            {
                return _bits.test(T::id());
            }
        bool has(size_t id)
        {
            return _bits.test(id);
        }

        bool has(const std::bitset<MAX_COMPONENTS> &compo) const{
            if(!compo.any()) return false;
            return ((_bits & compo)  == compo);
        }

        template <typename T>
            bool remove() {

                _bits.set(T::id(), 0);
                delete _components[T::id()];
                _components[T::id()] = nullptr;

                return true;
            }
        void removeAll() {
            for(int i = 0; i < MAX_COMPONENTS; ++i) {
                if(_bits.test(i)) {
                    delete _components[i];
                    _components[i] = nullptr;
                }
            }
            _bits.reset();
        }

        void resetMask() {
            _bits.reset();
        }

        std::vector<BaseComponent*> getAllComponents() {
            std::vector<BaseComponent*> temp;
            for(auto &c : _components) {
                temp.push_back(c.second);
            }
            return temp;
        }


    private:
        std::bitset<MAX_COMPONENTS> _bits;
        std::unordered_map<size_t, BaseComponent* > _components;
};

#endif
