#pragma once
#include "Component.h"
#include <string>
namespace fmc {
class CIdentity : public Component<CIdentity> {
    public:
    CIdentity() {_name = "Identity";}
    ~CIdentity() {}
    std::string name = "Entity";
    bool toSerialize = true;
    bool display = true;
        int* get(int v) {return nullptr;}

    };
}