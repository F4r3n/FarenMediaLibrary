#include "Export.hpp"
#include "Resource/ResourcesManager.h"
#include "Rendering/Shader.h"
#include <ranges>
#include <nlohmann/json.hpp>
#include "ShaderCompiler.h"
using namespace gui;

ExportManager::ExportManager(const ExportSettings& inSettings) : _settings(inSettings)
{

}

bool ExportManager::Run()
{
	//Copy internal resources
	fm::FilePath path(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "");
	fm::Folder(path).CopyTo(_settings.destination);
	gui::ShaderCompiler compiler;

	fm::Folder(path.ToSub("shaders")).Iterate(false, [&compiler](const fm::Folder* inFolder, const fm::File* inFile)
		{
			if (inFolder)
			{
				if (inFolder->GetPath().GetExtension() == ".vkshader")
				{
					ShaderCompiler::Reflection reflect;
					compiler.Compile(inFolder->GetPath(), reflect);
				}
			}
		});


	auto shaders = fm::ResourcesManager::get().getAll<fm::Shader>();
	for (const auto& shader : shaders | std::views::values)
	{
		if (!shader->IsInternal())
		{
			fm::FilePath currentPath = shader->GetPath();
			fm::Folder(currentPath).CopyTo(_settings.destination);
		}
	}
	return true;
}