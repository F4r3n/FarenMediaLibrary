#include "Components/CMaterial.h"
#include "Core/serializer.hpp"
#include <Rendering/Shader.h>
#include <Resource/ResourcesManager.h>
#include <EntityManager.h>
#include "Rendering/material.hpp"
#include <nlohmann/json.hpp>
using namespace fmc;
using namespace fm;


CMaterial::CMaterial()
{
	_name = "Material";
   _materials.push_back(fm::Material::GetDefaultStandardMaterial());
}


CMaterial::~CMaterial()
{
}



bool CMaterial::Serialize(nlohmann::json &ioJson) const
{
    nlohmann::json j;
    for(auto s : _materials)
    {
        nlohmann::json m = fm::FileSystem::ConvertPathToFileSystem(s->GetPath());
       j.push_back(m);
    }
    ioJson["Materials"] = j;
    return true;
}

bool CMaterial::Read(const nlohmann::json &inJSON)
{
	_materials.clear();
	for (const auto& m : inJSON["Materials"])
	{
		fm::FilePath p((std::string)m);
		auto mat = fm::ResourcesManager::get().getResource<fm::Material>(p);
		if (mat != nullptr)
		{
			_materials.push_back(mat);
		}
	}
    return true;
}

void CMaterial::SetMainMaterial(const fm::FilePath& inPath)
{
	std::shared_ptr<fm::Material> mat = fm::ResourcesManager::get().getResource<fm::Material>(inPath);
	if (_materials.empty())
	{
		_materials.push_back(mat);
	}
	else
	{
		_materials[0] = mat;
	}
}



const std::string& CMaterial::GetName() const
{
    return _name;
}


