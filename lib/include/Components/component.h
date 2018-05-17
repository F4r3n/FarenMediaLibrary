#ifndef COMPONENT_H
#define COMPONENT_H
#include <Component.h>
#include "EntityManager.h"
#include <json.hpp>
namespace fmc
{
    template <typename T>
    class FMComponent : public Component<T>
    {
            virtual bool Serialize(json &ioJson) {return false;}
            virtual bool Read(const json &inJSON) {return false;}
    };

}

#endif // COMPONENT_H

