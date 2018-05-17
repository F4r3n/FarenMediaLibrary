#pragma once
#include "component.h"
#include <string>
namespace fmc {
    class CIdentity : public FMComponent<CIdentity>
    {
        public:
            CIdentity() {_name = "Identity";}

            std::string name = "Entity";
            bool toSerialize = true;
            bool display = true;
    };
}
