#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include <array>
#include "ComponentManager.h"
#include <limits>
#include <queue>
#include <list>

#define ADD_SIZE 200
class Entity;
typedef Entity* pEntity;
typedef std::bitset<MAX_COMPONENTS> Mask;
class EntityManager {
public:
    EntityManager();
    static inline EntityManager& get() {
        return em;
    }
    ~EntityManager();

    Entity* createEntity();
    void getEntities(std::function<void(Entity*)> func);
    std::vector<BaseComponent*> getAllComponents(Entity *e) {
        return _entitiesComponents[_getID(e)]->getAllComponents();
    }
    void getEntitiesWithComponents(std::function<void(Entity*)> func, Mask& bits);
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
    bool hasComponents(Entity* e, const std::vector<std::size_t>& compo) const;
    bool hasComponents(Entity* e, const Mask& bits) const;
    void make();
    bool hasComponents(size_t id, const Mask& bits) const;
    
    template <typename T>
    bool hasComponent(size_t id) {
        if(checkID(id)) return false;
        return _entitiesComponents[id]->has<T>();
    }
    inline Entity* getEntity(const size_t id) {
        if(checkID(id)) return nullptr;
        return _entities_alive[id];
    }
    inline Entity* getEntityNotSafe(const size_t id) {
        return _entities_alive[id];
    }

    size_t GetID(Entity *e) const;

    inline std::list<pEntity>::iterator _DeleteEntity(const std::list<pEntity>::iterator &iterator )
    {
         return _listEntities.erase(iterator);
    }

    bool _IsEntityActive(Entity *e) const;
    
class EntityIteratorMask : public std::iterator<std::input_iterator_tag, size_t> {
    public:
        EntityIteratorMask(const Mask& mask, const std::list<pEntity>::iterator iterator,
                           const std::list<pEntity>::iterator begin,
                           const std::list<pEntity>::iterator end) {
            _mask = mask;
            _end = end;
            _begin = begin;
            _it = iterator;

               next();
        }
       
       Entity* operator*() {
           return *_it;
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
                if( !EntityManager::get()._IsEntityActive(*_it))
                {
                    if(_it == _begin)
                    {
                    _it = EntityManager::get()._DeleteEntity(_it);
                    _begin = _it;
                    }else
                    {
                        _it = EntityManager::get()._DeleteEntity(_it);

                    }
                }
                else
                {
                    if(valid(EntityManager::get()._getID(*_it)))
                        break;
                    ++_it;
                }
            }
        }

        inline bool valid(size_t index) const
        {
            return EntityManager::get().Exists(index)
            && EntityManager::get().hasComponents(index, _mask);
        }
        
    private:
        Mask _mask;
        std::list<pEntity>::iterator _it;
        std::list<pEntity>::iterator _end;
        std::list<pEntity>::iterator _begin;
    };
    
class EntityIterator : public std::iterator<std::input_iterator_tag, size_t> {
    public:
        EntityIterator(EntityManager &parent, const size_t capacity, const size_t index): parent(parent) {
            this->capacity = capacity;
            this->currentIndex = index;
            next();
        }
       
       Entity* operator*() {
           return parent.getEntityNotSafe(currentIndex);
       }
      
        EntityIterator operator++() {
            currentIndex++;
            next();
            return EntityIterator(parent, capacity, currentIndex);
        }
        
         EntityIterator begin(){
             return EntityIterator(parent, capacity, 0);
        }
        
         EntityIterator end() {
             return EntityIterator(parent, capacity, capacity);
        }
        
        bool operator!=(EntityIterator &i) {
            return i.currentIndex != currentIndex;
        }
        
        void next() {
            
            while(currentIndex < capacity && !parent.Exists(currentIndex)) {
                ++currentIndex;
            }
        }
        
         
    private:
        size_t currentIndex = 0;
        size_t capacity = 0;
        EntityManager& parent;
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
        EntityIteratorMask iterator(createMask<Args...>(),_listEntities.begin(), _listEntities.begin(), _listEntities.end());
        return iterator;
    }
    
    EntityIteratorMask iterate(const Mask &mask) {
        EntityIteratorMask iterator(mask, _listEntities.begin(),_listEntities.begin(), _listEntities.end());
        return iterator;
    }

    EntityIterator simpleIterate() {
        EntityIterator iterator(*this, _capacity, 0);
        return iterator;
    }
    void Free();
private:
    void _destroyEntity(size_t id, bool isActive);
    size_t _getID(Entity* e);

    const size_t _MAX_ID = std::numeric_limits<size_t>::max();

    std::queue<size_t> _free_id;

    std::vector<pEntity> _entities_alive;
    std::vector<pEntity> _temp_entities;
    std::vector<size_t> _entities_killed;
    std::vector<size_t> _entitiesToKill;
    std::list<pEntity> _listEntities;

    std::vector<std::unique_ptr<ComponentManager>> _entitiesComponents;
    static EntityManager em;

    size_t _capacity = 0;
    size_t _currentMAX = 0;
    
};

    

#endif
