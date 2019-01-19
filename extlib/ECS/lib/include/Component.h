#pragma once
#include <string>
#include <cstddef>
#include <nlohmann/json_fwd.hpp>
#include "Config.h"
#include <limits>
class ComponentManager;


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
	id _IDEntity = std::numeric_limits<id>::max();

    std::string _name = "";
    static size_t family_counter;
};


template <class T> class Component : public BaseComponent {

public:
    Component() {
    }

    static size_t GetID() {
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
        static size_t type = GetID();
        return type;
    }



    friend class ComponentManager;


private:

};


