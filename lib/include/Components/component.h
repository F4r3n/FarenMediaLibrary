#ifndef COMPONENT_H
#define COMPONENT_H
#include <Component.h>

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


template <class T>
class FMComponent : public Component<T>
{
    public:
        virtual bool Serialize(nlohmann::json &ioJson) const {return false;}
        virtual bool Read(const nlohmann::json &inJSON) {return false;}
        virtual const std::string & GetName() const {return Component<T>::GetName();}
        virtual ~FMComponent() = default;
        virtual size_t GetType() const {return kNone;}
        virtual void Destroy() = 0;

};

}

#endif // COMPONENT_H

