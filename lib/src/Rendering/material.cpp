#include <Rendering/Shader.h>

#include "Rendering/material.hpp"
#include <nlohmann/json.hpp>
#include <Resource/ResourcesManager.h>
using namespace fm;

Material::Material()
{
setValue<fm::Color>("mainColor", fm::Color(1,1,1,1));
}

void to_json(nlohmann::json& j, const Material &ma)
{
     j["shaderName"] = ma.shaderName;
        //j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, Material &ma) {
     ma.shaderName = j["shaderName"];
}

bool Material::Reload()
{
    if(_hasChanged)
        {
            if((shader == nullptr && shaderName != "")
              || (shader && shaderName != shader->GetName()))
            {
                shader = fm::ResourcesManager::get().getResource<fm::Shader>(shaderName);
                _hasChanged = false;
                return true;
            }

        }

        _hasChanged = false;
        return false;
}



std::map<std::string, fm::MaterialValue>& Material::getValues() {
    return _values;
}


