#pragma once

#include "Physic/Collider.h"
#include "Rendering/RenderingEvent.h"
#include <string>
class Entity;

namespace fm {
class CppScript;
}
namespace fmc
{
	class CTransform;
}

class Behaviour
{
        friend class fm::CppScript;

    public:
        Behaviour() {}
        virtual ~Behaviour() {}
        virtual void Update() {}
        virtual void Start() {}

		fmc::CTransform* GetTransform() { return nullptr; }
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
    virtual bool init(Entity*) {
        return true;
    }
    virtual void start() {
    }
    virtual void update(float dt) {
    }
    virtual SCRIPT_TYPE GetType() const
    {
        return NONE;
    }
    const std::string& GetScriptName() const
    {
        return _scriptName;
    }

	virtual bool Reload() { return false; }


    bool hasStarted = false;
    bool isInit = false;
   protected:
    std::string _scriptName;
private:
};
}
