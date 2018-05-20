#ifndef COMPONENT_H
#define COMPONENT_H
#include <Component.h>
#include "EntityManager.h"
#include <json.hpp>
#include "Core/serializer.hpp"
namespace fmc
{
const std::string kEmpty("");

enum ComponentType
{
    kTransform,
    kMaterial,
    KMesh,
    kCamera,
    kDirectionalLight,
    kIdentity,
    kPointLight,
    kScriptManager,
    kSource,
    kText,
    kEND,
    kNone
};


template <typename T>
class FMComponent : public Component<T>
{
    public:
        virtual bool Serialize(json &ioJson) const {return false;}
        virtual bool Read(const json &inJSON) {return false;}
        virtual const std::string & GetName() const {return Component<T>::GetName();}
        virtual ~FMComponent() = default;
        virtual size_t GetType() const {return kNone;}
};

}

#endif // COMPONENT_H

