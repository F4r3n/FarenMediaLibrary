#pragma once
#include <Entity.h>
#include "Physic/Collider.h"
#include "Rendering/RenderingEvent.h"

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
    Script() {
    }
    ~Script() {
    }
    virtual bool init(Entity* e) {
        return true;
    }
    virtual void start() {
    }
    virtual void update() {
    }
    virtual SCRIPT_TYPE GetType()
    {
        return NONE;
    }
    
    //TODO events
    //template <typename T> 
    virtual void event(std::string name, const CameraInfo &camera) {}
    virtual void event(std::string name, const ColliderInfo& collider) {}
    virtual void event(std::string name, const Collider& collider) {}

    //    
    //}
    bool hasStarted = false;
    bool isInit = false;
private:
};
}
