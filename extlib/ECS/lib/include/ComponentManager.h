#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include "Component.h"
#include <vector>
#include <bitset>
#include "Config.h"

class ComponentManager
{
    public:
        explicit ComponentManager(id ID) {_bits.reset(); currentEntityID = ID;}
        ComponentManager() = delete;
        ~ComponentManager() {
            removeAll();
        }

        template <typename T> T* getComponent()
        {
            if(!has<T>()) {
                return nullptr;
            }
            return static_cast<T*>(_components[T::GetID()]);
        }

        template <typename T> T* add(Component<T> *c)
        {
            _bits.set(T::GetID(), 1);
            c->_IDEntity = currentEntityID;
            _components[T::GetID()] = std::move(c);

            return static_cast<T*>(_components[T::GetID()]);
        }

        template <typename T, typename... Args> T* addComponent(Args&&... args)
        {
            _bits.set(T::GetID(), 1);
            Component<T>* c = new T(std::forward<Args>(args)...);
            c->_IDEntity = currentEntityID;

            _components[T::GetID()] = std::move(c);

            return static_cast<T*>(_components[T::GetID()]);
        }

        template <typename T>
        bool has()
        {
            return _bits.test(T::GetID());
        }

        bool has(id id)
        {
            return _bits.test(id);
        }

        bool has(const std::bitset<MAX_COMPONENTS> &compo) const{
            if(!compo.any()) return false;
            return ((_bits & compo)  == compo);
        }

        template <typename T>
            bool remove() 
			{

                _bits.set(T::GetID(), 0);
                delete _components[T::GetID()];
                _components[T::GetID()] = nullptr;

                return true;
            }

        void removeAll() 
		{
            for(int i = 0; i < MAX_COMPONENTS; ++i) 
			{
                if(_bits.test(i)) 
				{
                    delete _components[i];
                    _components[i] = nullptr;
                }
            }
            _bits.reset();
        }

        void resetMask() 
		{
            _bits.reset();
        }

        std::vector<BaseComponent*> getAllComponents() 
		{
            std::vector<BaseComponent*> temp;
            for(int i = 0; i < MAX_COMPONENTS; ++i) {
                if(_bits.test(i)) 
				{
                    temp.push_back(_components[i]);
                }
            }
            return temp;
        }

        id currentEntityID = std::numeric_limits<id>::max();
    private:
        std::bitset<MAX_COMPONENTS> _bits;
        BaseComponent* _components[MAX_COMPONENTS];
};

#endif
