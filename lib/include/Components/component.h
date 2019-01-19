#ifndef COMPONENT_H
#define COMPONENT_H
#include <Component.h>

#include "Core/serializer.hpp"
#include "Config.h"
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
	kBody3D,
    kSource,
    kText,
    kEvent,
    kEND,
    kNone
};


template <class T>
class FMComponent : public Component<T>
{
    public:
        virtual bool Serialize(nlohmann::json &) const {return false;}
        virtual bool Read(const nlohmann::json &) {return false;}
        virtual const std::string & GetName() const {return Component<T>::GetName();}
        virtual ~FMComponent() = default;
        virtual size_t GetType() const {return kNone;}
        virtual void Destroy() = 0;
		bool Enabled = true;
};

}

#endif // COMPONENT_H

