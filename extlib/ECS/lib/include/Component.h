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
    virtual uint16_t GetType() const = 0;
    virtual void Destroy() = 0;
protected:
	ecs::id _IDEntity = std::numeric_limits<ecs::id>::max();
	void operator delete (void* obj) { delete obj; }

    std::string _name = "";
    static uint16_t family_counter;
};


template <class T> class Component : public BaseComponent {

public:
    Component() {
    }

    static uint16_t GetID() {
        static uint16_t i = family_counter++;

        return i;
    }



    virtual bool Serialize(nlohmann::json &ioJson) const = 0;
    virtual bool Read(const nlohmann::json &inJSON) = 0;
    virtual void Destroy() = 0;

    virtual uint16_t GetType() const
    {
        static uint16_t type = GetID();
        return type;
    }



    friend class ComponentManager;
protected:
	virtual ~Component() {}

private:

};


