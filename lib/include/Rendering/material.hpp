#ifndef MATERIAL_HPP
#define MATERIAL_HPP


#include <memory>
#include <map>
#include <Rendering/MaterialValue.h>
#include <Resource/Resource.h>
#include <Core/serializer.hpp>
namespace fm
{
class Shader;
}

namespace fm
{
class Material : public Resource, public Serializer
{

    public:
        bool Serialize(nlohmann::json &ioJson) const override;
        bool Read(const nlohmann::json &inJSON) override;


        Material(const std::string &id);
        template <typename T>
        void setValue(const std::string& name, T value) {
            fm::MaterialValue materialValue;
            materialValue = value;
            _values.insert(std::pair<std::string, fm::MaterialValue>(name, value));
        }
        static constexpr fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::MATERIAL;}

        std::map<std::string, fm::MaterialValue>& getValues();

        std::string shaderName = "default";

        fm::Shader* shader = nullptr;

        inline void SetFlagHasChanged() {_hasChanged = true;}
        bool Reload();
        const std::string& GetID() const {return _id;}

    private:
        std::string _id = "none";
        std::map<std::string, fm::MaterialValue> _values;
        bool _hasChanged = true;
};


}


#endif // MATERIAL_HPP

