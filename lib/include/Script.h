#pragma once
#include <Entity.h>
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
    //virtual void event(std::string name, const T& t) {
    //    
    //}
    bool hasStarted = false;

private:
};
}
