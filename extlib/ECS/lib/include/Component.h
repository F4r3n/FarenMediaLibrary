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
    const std::string& getName() const {return _name;}
    virtual int* get(int v) = 0;
protected:
    std::string _name;

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
    virtual int* get(int v){return nullptr;};
    virtual ~Component() {
    }
    friend class ComponentManager;

private:
};


