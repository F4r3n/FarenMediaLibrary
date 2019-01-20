#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include "Component.h"
#include <vector>
#include "Config.h"


class BitSet
{
public:
	BitSet()
	{
		_value = 0;
	}

	BitSet(uint64_t inValue)
	{
		_value = inValue;
	}
	void Set(uint16_t bit)
	{
		_value |= ((uint64_t)1 << bit);
	}


	void Unset(uint16_t bit)
	{
		_value &= ~((uint64_t)1 << bit);
	}

	bool Test(uint16_t bit) const
	{
		return (_value & (((uint64_t)1 << bit)));
	}

	void Reset()
	{
		_value = 0;
	}

	bool operator==(const BitSet &b) const
	{
		return _value == b._value;
	}

	BitSet operator|(const BitSet &b) const
	{
		return BitSet(_value | b._value);
	}

	BitSet operator&(const BitSet &b) const
	{
		return BitSet(_value & b._value);
	}
private:
	std::uint64_t _value;
};

class ComponentManager
{

    public:
        explicit ComponentManager(id ID) 
		{
			_bits.Reset(); 
			currentEntityID = ID;
		}

        ComponentManager() = delete;

        ~ComponentManager() 
		{
            removeAll();
        }

        template <typename T> T* getComponent() const
        {
            if(!has<T>()) 
			{
                return nullptr;
            }
            return static_cast<T*>(_components[T::GetID()]);
        }

        template <typename T> T* add(Component<T> *c)
        {
            _bits.Set(T::GetID());
            c->_IDEntity = currentEntityID;
            _components[T::GetID()] = std::move(c);

            return static_cast<T*>(_components[T::GetID()]);
        }

        template <typename T, typename... Args> T* addComponent(Args&&... args)
        {
            _bits.Set(T::GetID());
            Component<T>* c = new T(std::forward<Args>(args)...);
            c->_IDEntity = currentEntityID;

            _components[T::GetID()] = std::move(c);

            return static_cast<T*>(_components[T::GetID()]);
        }

        template <typename T>
        bool has() const
        {
            return _bits.Test(T::GetID());
        }

        bool has(uint16_t id) const
        {
            return _bits.Test(id);
        }

        bool has(const BitSet &compo) const
		{
			return (_bits & compo) == compo;
        }

        template <typename T>
            bool remove() 
			{

                _bits.Unset(T::GetID());
                delete _components[T::GetID()];
                _components[T::GetID()] = nullptr;

                return true;
            }

        void removeAll() 
		{
            for(uint16_t i = 0; i < MAX_COMPONENTS; ++i) 
			{
                if(_bits.Test(i)) 
				{
                    delete _components[i];
                    _components[i] = nullptr;
                }
            }
            _bits.Reset();
        }

        void resetMask() 
		{
            _bits.Reset();
        }

        std::vector<BaseComponent*> getAllComponents() const
		{
            std::vector<BaseComponent*> temp;
            for(uint16_t i = 0; i < MAX_COMPONENTS; ++i) {
                if(_bits.Test(i)) 
				{
                    temp.push_back(_components[i]);
                }
            }
            return temp;
        }

        id currentEntityID = std::numeric_limits<id>::max();
    private:
		BitSet _bits;
        BaseComponent* _components[MAX_COMPONENTS];
};

#endif
