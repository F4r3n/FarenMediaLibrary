#pragma once
#include <Entity.h>
#include "Collider.h"
namespace fm {
class Script {
public:
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
    //TODO events
    //template <typename T> 
    virtual void event(std::string name, const ColliderInfo& collider) {}
    virtual void event(std::string name, const Collider& collider) {}

    //    
    //}
    bool hasStarted = false;

private:
};
}
