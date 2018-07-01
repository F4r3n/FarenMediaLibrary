#include <Rendering/Shader.h>

#include "Rendering/material.hpp"
#include <nlohmann/json.hpp>
#include <Resource/ResourcesManager.h>

using namespace fm;

Material::Material(const std::string &id)
{
    _id = id;
    setValue<fm::Color>("mainColor", fm::Color(1,1,1,1));
}

bool Material::Serialize(nlohmann::json &ioJson) const
{
    ioJson["id"] = _id;
    ioJson["shaderName"] = shaderName;

    nlohmann::json valuesJSON;
    for(auto &value : _values)
    {
        nlohmann::json valueJSON;
        fm::ValuesType type = value.second.getType();

        valueJSON["name"] = value.first;
        valueJSON["type"] = type;
        if(type == fm::ValuesType::VALUE_INT)
            valueJSON["value"] = value.second.getInt();
        else if(type == fm::ValuesType::VALUE_COLOR)
            valueJSON["value"] = value.second.getColor();
        else if(type == fm::ValuesType::VALUE_FLOAT)
            valueJSON["value"] = value.second.getFloat();
        else if(type == fm::ValuesType::VALUE_MATRIX_FLOAT)
            valueJSON["value"] = value.second.getMatrix();
        else if(type == fm::ValuesType::VALUE_TEXTURE)
            valueJSON["value"] = value.second.getTexture();
        else if(type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
            valueJSON["value"] = value.second.getVector2();
        else if(type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
            valueJSON["value"] = value.second.getVector3();
        else if(type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
            valueJSON["value"] = value.second.getVector4();
        valuesJSON.push_back(valueJSON);
    }
    ioJson["materialValues"] = valuesJSON;

    return true;
}

bool Material::Read(const nlohmann::json &inJSON)
{
    _id = inJSON["id"];
    shaderName = inJSON["shaderName"];
    nlohmann::json values = inJSON["materialValues"];
    for (json::iterator it = values.begin(); it != values.end(); ++it)
    {
        nlohmann::json v = *it;
        fm::ValuesType type = v["type"];
        if(type == fm::ValuesType::VALUE_INT)
            setValue(v["name"], (int)v["value"]);
        else if(type == fm::ValuesType::VALUE_COLOR)
        {
            fm::Color c = v["value"];
            setValue(v["name"], c);
        }
        else if(type == fm::ValuesType::VALUE_FLOAT)
            setValue(v["name"], (float)v["value"]);
        else if(type == fm::ValuesType::VALUE_MATRIX_FLOAT)
        {
            fm::math::mat mat = v["value"];
            setValue(v["name"], mat);
        }
        else if(type == fm::ValuesType::VALUE_TEXTURE)
        {
            fm::TextureMat t = v["value"];
            setValue(v["name"], t);
        }
        else if(type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
        {
            fm::math::vec2 c = v["value"];
            setValue(v["name"], c);
        }
        else if(type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
        {
            fm::math::vec3 c = v["value"];
            setValue(v["name"], c);
        }
        else if(type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
        {
            fm::math::vec4 c = v["value"];
            setValue(v["name"], c);
        }
    }

    return true;
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


