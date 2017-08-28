#pragma once
#include <string>
#include <memory>
#include <typeinfo> //for 'typeid' to work
class ComponentManager;

#include <cstddef>
class BaseComponent {
public:
    BaseComponent();
    virtual ~BaseComponent();

protected:
    static std::size_t family_counter;
};


template <class T> class Component : public BaseComponent {

public:
    Component() {
    }

    static size_t id() {
        static size_t i = family_counter++;
        return i;
    }

    virtual ~Component() {
    }
    friend class ComponentManager;

private:
};


