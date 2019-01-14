#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <iostream>
#include <typeinfo>
#include <memory>

#include "Component.h"

class Entity
{

public:
    Entity();
    Entity(size_t ID);
    ~Entity();
    size_t ID = std::numeric_limits<size_t>::max();
    bool active = false;

	template <typename T, typename ...Args> T* addComponent(Args&&... args);

	template <typename T> T* add();
	template <typename T> T* addEmpty();
	template <typename T> T* add(Component<T> *c);
	template <typename T> T* get();
	template <typename T> bool has();
	template <typename T> bool remove();
	std::vector<BaseComponent*> getAllComponents();

    
    void destroy();

private:
};

#endif
