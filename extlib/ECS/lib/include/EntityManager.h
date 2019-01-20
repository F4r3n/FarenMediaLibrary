#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include "Entity.h"
#include "ComponentManager.h"
#include <queue>
#define ADD_SIZE 200

typedef BitSet Mask;
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
    std::vector<BaseComponent*> getAllComponents(Entity *e) {
        return _entitiesComponents[e->ID]->getAllComponents();
    }
    void killAll();
    bool Exists(id id) const;
    inline bool checkID(id ID) const {
        return ID == _MAX_ID;
    }
    
    template <typename T> bool removeComponent(id ID) {
         if(checkID(ID))
            return false;
        if(_entitiesComponents[ID] != nullptr) {
            return _entitiesComponents[ID]->remove<T>();
        }
        return false;
    }

    template <typename T> T* add(id ID, Component<T>* c) {
        if(checkID(ID))
            return nullptr;
        if(_entitiesComponents[ID] != nullptr) {
            return _entitiesComponents[ID]->add<T>(c);
        } else {
            _entitiesComponents[ID] = std::make_unique<ComponentManager>(ID);
            return _entitiesComponents[ID]->add<T>(c);
        }
    }

    template <typename T, typename... Args> T* addComponent(id ID, Args &&... args) {
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
        return add<T>(e->ID, c);
    }

    template <typename T, typename ...Args> T* addComponent(Entity* e, Args&& ...args) {
        return addComponent<T>(e->ID, args...);
    }
    

    std::vector<std::unique_ptr<ComponentManager>>& getEntitiesComponents() {
        return _entitiesComponents;
    }

    template <typename T> T* get(Entity* e) {
        if(!_entitiesComponents[e->ID])
            return nullptr;
        return _entitiesComponents[e->ID]->getComponent<T>();
    }

    void deleteEntity(Entity* e);
    std::vector<id> getEntitiesAlive();
    bool hasComponents(const Entity& e, const std::vector<uint16_t>& compo) const;
    bool hasComponents(const Entity& e, const Mask& bits) const;
    void make();
    bool hasComponents(id id, const Mask& bits) const;
    
    template <typename T>
    bool hasComponent(id id) {
        if(checkID(id)) return false;
        return _entitiesComponents[id]->has<T>();
    }
    inline Entity* getEntity(const id id) {
        if(checkID(id)) return nullptr;
        return &_entities_alive[id];
    }
    inline Entity* getEntityNotSafe(const id id) {
        return &_entities_alive[id];
    }

	id GetID(Entity *e) const;


    bool _IsEntityActive(Entity *e) const;
	bool IsActive(id id) const;

class EntityIteratorMask : public std::iterator<std::input_iterator_tag, id> {
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

        inline bool valid(id index) const
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
        mask.Set(T::GetID());
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
    void _destroyEntity(id id, bool isActive);

    const id _MAX_ID = std::numeric_limits<id>::max();

    std::queue<id> _free_id;

    std::vector<Entity> _entities_alive;
    std::vector<id> _entities_killed;

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
