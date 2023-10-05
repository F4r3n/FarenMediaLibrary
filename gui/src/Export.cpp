#include "Export.hpp"
#include "Resource/ResourcesManager.h"
#include "Rendering/Shader.h"
#include <ranges>
#include <nlohmann/json.hpp>
using namespace gui;

ExportManager::ExportManager(const ExportSettings& inSettings) : _settings(inSettings)
{

}

bool ExportManager::Run()
{
	auto shaders = fm::ResourcesManager::get().getAll<fm::Shader>();
	for (const auto& shader : shaders | std::views::values)
	{
		fm::FilePath currentPath = shader->GetPath();
		fm::Folder(currentPath).CopyTo(_settings.destination);
		//shader->Save();
	}
	return true;
}