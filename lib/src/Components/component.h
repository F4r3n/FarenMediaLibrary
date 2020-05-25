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
	kBody2D,
	kBody3D,
    kSource,
    kText,
    kEvent,
	kCollider3D,
	kCollider2D,
    kEND,
    kNone
};


template <class T>
class FMComponent : public Component<T>
{
    public:
        virtual bool Serialize(nlohmann::json &) const {return false;}
        virtual bool Read(const nlohmann::json &) {return false;}
        virtual const std::string& GetName() const {return Component<T>::GetName();}
        virtual uint16_t GetType() const {return kNone;}
        virtual void Destroy() = 0;
		ecs::id GetIDEntity() const {return BaseComponent::_IDEntity;}
		bool Enabled = true;
protected:
		virtual ~FMComponent() = default;
		
};

}

#endif // COMPONENT_H

