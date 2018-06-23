#pragma once
#include <string>
#include <memory>
#include <typeinfo> //for 'typeid' to work
#include <limits>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
class ComponentManager;



#include <cstddef>
class BaseComponent {
public:
    BaseComponent();
    virtual ~BaseComponent();
    const std::string& GetName() const {return _name;}
    virtual bool Serialize(nlohmann::json &ioJson) const = 0;
    virtual bool Read(const nlohmann::json &inJSON) = 0;
    virtual size_t GetType() const = 0;
    virtual void Destroy() = 0;
protected:
    size_t _IDEntity = std::numeric_limits<size_t>::max();

    std::string _name = "";
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
    virtual bool Serialize(nlohmann::json &ioJson) const = 0;
    virtual bool Read(const nlohmann::json &inJSON) = 0;
    virtual void Destroy() = 0;

    virtual size_t GetType() const
    {
        static size_t type = id();
        return type;
    }



    friend class ComponentManager;


private:

};


