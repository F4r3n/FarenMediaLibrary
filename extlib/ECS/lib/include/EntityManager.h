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
	friend BaseComponent;
    EntityManager();
    static inline EntityManager& get() 
	{
		static EntityManager em;
        return em;
    }
    ~EntityManager();

    Entity createEntity();
    std::vector<BaseComponent*> getAllComponents(const Entity& e) {
        return _entitiesComponents[e.id().index()]->getAllComponents();
    }
    void killAll();
    bool Valid(const Entity::Id& e) const;

    
    template <typename T> bool removeComponent(Entity::Id id) {

        if(_entitiesComponents[id.index()] != nullptr) {
            return _entitiesComponents[id.index()]->remove<T>();
        }
        return false;
    }

    template <typename T> T* add(Entity::Id id, Component<T>* c) {

        if(_entitiesComponents[id.index()] != nullptr) {
            return _entitiesComponents[id.index()]->add<T>(c);
        } else {
            _entitiesComponents[id.index()] = std::make_unique<ComponentManager>(id.index());
            return _entitiesComponents[id.index()]->add<T>(c);
        }
    }

    template <typename T, typename... Args> T* addComponent(Entity::Id id, Args &&... args) {

        if(_entitiesComponents[id.index()] != nullptr) {
            return _entitiesComponents[id.index()]->addComponent<T>(args...);
        } else {
            _entitiesComponents[id.index()] = std::make_unique<ComponentManager>();
            return _entitiesComponents[id.index()]->addComponent<T>(args...);
        }
    }

    template <typename T> T* add(const Entity& e, Component<T>* c) {
        return add<T>(e.id(), c);
    }

    template <typename T, typename ...Args> T* addComponent(const Entity& e, Args&& ...args) {
        return addComponent<T>(e.id(), args...);
    }
    

    std::vector<std::unique_ptr<ComponentManager>>& getEntitiesComponents() {
        return _entitiesComponents;
    }

    template <typename T> T* get(const Entity& e) {
        if(!_entitiesComponents[e.id().index()])
            return nullptr;
        return _entitiesComponents[e.id().index()]->getComponent<T>();
    }

    void deleteEntity(const Entity& e);
    bool hasComponents(const Entity& e, const std::vector<uint16_t>& compo) const;
	bool hasComponents(const Entity::Id& id, const Mask& bits) const;
	template <typename T>
	bool hasComponent(const Entity& e)
	{
		if (_entitiesComponents[e.id().index()])
			return _entitiesComponents[e.id().index()]->has<T>();
		return false;
	}

	Entity::Id CreateID(uint32_t inID);
	Entity	   GetEntity(Entity::Id inID);


class EntityIteratorMask {
    public:
        EntityIteratorMask(const Mask& mask, uint32_t inCursor) {
            _mask = mask;
            _cursor = inCursor;
			_manager = &EntityManager::get();
            next();
        }
       
       Entity operator*() {
		   return Entity(_manager, _manager->CreateID(_cursor));
       }
      
        EntityIteratorMask operator++() {
			_cursor++;
            next();
            return EntityIteratorMask(_mask, _cursor);
        }
        
         EntityIteratorMask begin(){
             return EntityIteratorMask(_mask, 0);
        }
        
         EntityIteratorMask end() {
             return EntityIteratorMask(_mask, _manager->_capacity);
        }
        
        bool operator!=(EntityIteratorMask &i) {
            return _cursor < i._cursor;
        }
        
        void next() {

            while(_cursor < _manager->_capacity && !predicate())
            {
				++_cursor;
            }
        }

		bool predicate()
		{
			Entity::Id id = _manager->CreateID(_cursor);
			if (_manager->Valid(id))
			{
				if (_manager->hasComponents(id, _mask))
					return true;
			}
			return false;
		}
        
    private:
		EntityManager* _manager;
        Mask _mask;
        uint32_t _cursor;
        
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
        EntityIteratorMask iterator(createMask<Args...>(), 0);
        return iterator;
    }
    
    EntityIteratorMask iterate(const Mask &mask) {
        EntityIteratorMask iterator(mask, 0);
        return iterator;
    }

    void Free();
	void killInativeEntities();

private:


    std::vector<uint32_t> _free_id;
	std::vector<uint32_t> _entity_version;
    std::vector<std::unique_ptr<ComponentManager>> _entitiesComponents;

    size_t _capacity = 0;
    
};

template <typename T, typename ...Args> T* Entity::addComponent(Args&&... args)
{
	return _manager->addComponent<T>(this->id(), args...);
}


template <typename T> T* Entity::addEmpty()
{
	return _manager->add<T>(this, new T());
}
template <typename T> T* Entity::add(Component<T> *c)
{
	return _manager->add<T>(this, c);
}

template <typename T> T* Entity::get()
{
	return _manager->get<T>(*this);
}

template <typename T> bool Entity::has()
{
	return _manager->hasComponent<T>(*this);
}

template <typename T> bool Entity::remove()
{
	return _manager->removeComponent<T>(_id);
}



#endif
