#include "Components/CMesh.h"
#include "Resource/ResourcesManager.h"
#include <memory>
#include "Core/Math/Functions.h"
#include <EntityManager.h>
#include "Rendering/Model.hpp"
#include <nlohmann/json.hpp>
using namespace fmc;

CMesh::~CMesh()
{
}

CMesh::CMesh() 
{
	_name = "Mesh";
	_modelPath = fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Quad");
	_model = fm::ResourcesManager::get().getResource<fm::Model>(_modelPath);
}

bool CMesh::Serialize(nlohmann::json &ioJson) const
{
    ioJson["modelPath"] = fm::FileSystem::ConvertPathToFileSystem(_modelPath);
    return true;
}
bool CMesh::Read(const nlohmann::json &inJSON)
{
	if (inJSON.contains("modelPath"))
	{
		_modelPath = fm::FilePath((std::string)inJSON["modelPath"]);
		_model = fm::ResourcesManager::get().getResource<fm::Model>(_modelPath);
	}

    return true;
}

void CMesh::SetModelPath(const fm::FilePath& inPath)
{
	_modelPath = inPath;
	_model = fm::ResourcesManager::get().getResource<fm::Model>(_modelPath);
}


const std::string& CMesh::GetName() const
{
    return _name;
}

