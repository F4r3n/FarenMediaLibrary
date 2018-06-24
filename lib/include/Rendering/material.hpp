#ifndef MATERIAL_HPP
#define MATERIAL_HPP


#include <memory>
#include <map>
#include <Rendering/MaterialValue.h>
#include <nlohmann/json_fwd.hpp>
namespace fm
{
class Shader;
}

namespace fm
{
class Material
{

    public:
        Material();
        template <typename T>
        void setValue(const std::string& name, T value) {
            fm::MaterialValue materialValue;
            materialValue = value;
            _values.insert(std::pair<std::string, fm::MaterialValue>(name, value));
        }
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

void to_json(nlohmann::json& j, const Material &mat);
void from_json(const nlohmann::json& j, Material &mat);

}

#endif // MATERIAL_HPP

