#pragma once
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
	kBody,
    kSource,
    kText,
    kEvent,
	kCollider,
    kEND,
    kNone
};

/*
template <class T>
class FMComponent : public Component<T>
{
    public:
        virtual bool Serialize(nlohmann::json &) const {return false;}
        virtual bool Read(const nlohmann::json &) {return false;}
        virtual const std::string& GetName() const {return Component<T>::GetName();}
        virtual uint16_t GetType() const {return kNone;}
		bool Enabled = true;
protected:
		virtual ~FMComponent() = default;
		
};
*/

}


