#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include "Entity.h"
#include <array>
#include "ComponentManager.h"
#include <limits>
#include <queue>
#include <list>
#include <functional>
#define ADD_SIZE 200

typedef std::bitset<MAX_COMPONENTS> Mask;
class EntityManager 
{
public:
    EntityManager();
    static inline EntityManager& get() 
	{
		static EntityManager em;
        return em;
    }
    ~EntityManager();

    Entity* createEntity();
    void getEntities(std::function<void(Entity&)> func);
    std::vector<BaseComponent*> getAllComponents(Entity *e) {
        return _entitiesComponents[e->ID]->getAllComponents();
    }
    void getEntitiesWithComponents(std::function<void(Entity&)> func, Mask& bits);
    void killAll();
    bool Exists(size_t id) const;
    inline bool checkID(size_t ID) const {
        return ID == _MAX_ID;
    }
    
    template <typename T> bool removeComponent(size_t ID) {
         if(checkID(ID))
            return false;
        if(_entitiesComponents[ID] != nullptr) {
            return _entitiesComponents[ID]->remove<T>();
        }
        return false;
    }

    template <typename T> T* add(size_t ID, Component<T>* c) {
        if(checkID(ID))
            return nullptr;
        if(_entitiesComponents[ID] != nullptr) {
            return _entitiesComponents[ID]->add<T>(c);
        } else {
            _entitiesComponents[ID] = std::make_unique<ComponentManager>(ID);
            return _entitiesComponents[ID]->add<T>(c);
        }
    }

    template <typename T, typename... Args> T* addComponent(size_t ID, Args &&... args) {
        if(checkID(ID))
            return nullptr;
        if(_entitiesComponents[ID] != nullptr) {
            return _entitiesComponents[ID]->addComponent<T>(args...);
        } else {
            _entitiesComponents[ID] = std::make_unique<ComponentManager>(ID);
            return _entitiesComponents[ID]->addComponent<T>(args...);
        }
    }

    template <typename T> T* add(Entity* e, Component<T>* c) {
        return add<T>(_getID(e), c);
    }

    template <typename T, typename ...Args> T* addComponent(Entity* e, Args&& ...args) {
        return addComponent<T>(_getID(e), args...);
    }
    

    std::vector<std::unique_ptr<ComponentManager>>& getEntitiesComponents() {
        return _entitiesComponents;
    }

    template <typename T> T* get(Entity* e) {
        if(!_entitiesComponents[_getID(e)])
            return nullptr;
        return _entitiesComponents[_getID(e)]->getComponent<T>();
    }

    void deleteEntity(Entity* e);
    std::vector<size_t> getEntitiesAlive();
    bool hasComponents(const Entity& e, const std::vector<std::size_t>& compo) const;
    bool hasComponents(const Entity& e, const Mask& bits) const;
    void make();
    bool hasComponents(size_t id, const Mask& bits) const;
    
    template <typename T>
    bool hasComponent(size_t id) {
        if(checkID(id)) return false;
        return _entitiesComponents[id]->has<T>();
    }
    inline Entity* getEntity(const size_t id) {
        if(checkID(id)) return nullptr;
        return &_entities_alive[id];
    }
    inline Entity* getEntityNotSafe(const size_t id) {
        return &_entities_alive[id];
    }

    size_t GetID(Entity *e) const;


    bool _IsEntityActive(Entity *e) const;
	bool IsActive(size_t id) const;

class EntityIteratorMask : public std::iterator<std::input_iterator_tag, size_t> {
    public:
        EntityIteratorMask(const Mask& mask, const std::vector<Entity>::iterator iterator,
                           const std::vector<Entity>::iterator begin,
                           const std::vector<Entity>::iterator end) {
            _mask = mask;
            _end = end;
            _begin = begin;
            _it = iterator;

               next();
        }
       
       Entity* operator*() {
           return &(*_it);
       }
      
        EntityIteratorMask operator++() {
            _it++;
            next();
            return EntityIteratorMask(_mask, _it, _begin, _end);
        }
        
         EntityIteratorMask begin(){
             return EntityIteratorMask(_mask, _begin, _begin, _end);
        }
        
         EntityIteratorMask end() {
             return EntityIteratorMask(_mask, _end, _begin, _end);
        }
        
        bool operator!=(EntityIteratorMask &i) {
            return _it != i._it;
        }
        
        void next() {

            while(_it != _end)
            {
               

                
                 if( valid(_it->ID))
                     break;
                 ++_it;
                
            }
        }

        inline bool valid(size_t index) const
        {
            return EntityManager::get().Exists(index) && EntityManager::get().IsActive(index) && EntityManager::get().hasComponents(index, _mask);
        }
        
    private:
        Mask _mask;
        std::vector<Entity>::iterator _it;
        std::vector<Entity>::iterator _end;
        std::vector<Entity>::iterator _begin;
    };
    

     template <typename T>
    Mask createMask() {
        Mask mask;
        mask.set(T::id());
        return mask;
    }
    
     template <typename C1, typename C2, typename ... Components>
    Mask createMask() {
        return createMask<C1>() | createMask<C2, Components...>();
    }
    
    template <typename ...Args>
    EntityIteratorMask iterate() {
        EntityIteratorMask iterator(createMask<Args...>(), _entities_alive.begin(), _entities_alive.begin(), _entities_alive.end());
        return iterator;
    }
    
    EntityIteratorMask iterate(const Mask &mask) {
        EntityIteratorMask iterator(mask, _entities_alive.begin(), _entities_alive.begin(), _entities_alive.end());
        return iterator;
    }


    void Free();
	void killInativeEntities();

private:
    void _destroyEntity(size_t id, bool isActive);
    size_t _getID(Entity* e);

    const size_t _MAX_ID = std::numeric_limits<size_t>::max();

    std::queue<size_t> _free_id;

    std::vector<Entity> _entities_alive;
    std::vector<size_t> _entities_killed;

    std::vector<std::unique_ptr<ComponentManager>> _entitiesComponents;

    size_t _capacity = 0;
    size_t _currentMAX = 0;
    
};

template <typename T, typename ...Args> T* Entity::addComponent(Args&&... args)
{
	return EntityManager::get().addComponent<T>(this, args...);
}
template <typename T> T* Entity::add()
{
	return EntityManager::get().add<T>(this, new T());
}

template <typename T> T* Entity::addEmpty()
{
	return EntityManager::get().add<T>(this, new T());
}
template <typename T> T* Entity::add(Component<T> *c)
{
	return EntityManager::get().add<T>(this, c);
}

template <typename T> T* Entity::get()
{
	return EntityManager::get().get<T>(this);
}

template <typename T> bool Entity::has()
{
	return EntityManager::get().hasComponent<T>(ID);
}

template <typename T> bool Entity::remove()
{
	return EntityManager::get().removeComponent<T>(ID);
}



#endif
