#pragma once
#include <typeinfo>
#include <string>
#include <iostream>

class BaseComponent {
public:
	BaseComponent() {}
	virtual ~BaseComponent() {}
};

template <class T>
class Component : public BaseComponent{
public:
	Component() {}
   static const std::string name()
    {
        return typeid(T).name();
    }

	virtual ~Component() {}
protected:

};