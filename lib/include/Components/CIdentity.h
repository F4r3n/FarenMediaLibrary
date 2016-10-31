#pragma once
#include "Component.h"
#include <string>
namespace fmc {
class CIdentity : public Component<CIdentity> {
    public:
    CIdentity() {}
    ~CIdentity() {}
    std::string name;
    bool toSerialize = true;
    };
}