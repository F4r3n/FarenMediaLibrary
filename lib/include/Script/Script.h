#pragma once

#include "Physic/Collider.h"
#include "Rendering/RenderingEvent.h"
#include <string>
class Entity;

namespace fm {
class CppScript;
}

class Behaviour
{
        friend class fm::CppScript;

    public:
        Behaviour() {}
        virtual ~Behaviour() {}
        virtual void Update() {}
        virtual void Start() {}
    protected:
        Entity* entity;
};

namespace fm {


class Script {


public:
    enum SCRIPT_TYPE
    {
        LUA,
        CPP,
        NONE
    };

    Script() {}
    ~Script() {}
    virtual bool init(Entity* e) {
        return true;
    }
    virtual void start() {
    }
    virtual void update() {
    }
    virtual SCRIPT_TYPE GetType() const
    {
        return NONE;
    }
    const std::string& GetScriptName() const
    {
        return _scriptName;
    }
    
    //virtual void event(std::string name, const CameraInfo &camera) {}
    //virtual void event(std::string name, const ColliderInfo& collider) {}
    //virtual void event(std::string name, const Collider& collider) {}

    bool hasStarted = false;
    bool isInit = false;
   protected:
    std::string _scriptName;
private:
};
}
