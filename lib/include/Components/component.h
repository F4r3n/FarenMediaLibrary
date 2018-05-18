#ifndef COMPONENT_H
#define COMPONENT_H
#include <Component.h>
#include "EntityManager.h"
#include <json.hpp>
namespace fmc
{
class SerializerComponent
{
    public:
        virtual bool Serialize(json &ioJson) = 0;
        virtual bool Read(const json &inJSON) = 0;
        virtual void GetName(std::string &outName) = 0;
        virtual ~SerializerComponent() = default;
};


template <typename T>
class FMComponent : public Component<T>, public SerializerComponent
{
    public:
        virtual bool Serialize(json &ioJson) {return false;}
        virtual bool Read(const json &inJSON) {return false;}
        virtual void GetName(std::string &outName) {}
        virtual ~FMComponent() = default;

};

}

#endif // COMPONENT_H

