#include "IconManager.hpp"
#include "Core/FilePath.h"
#include "Rendering/Texture2D.hpp"
using namespace gui;

std::shared_ptr<fm::Texture2D> IconManager::GetIcon(const fm::FilePath& inPath) const
{
	if (auto it = _icons.find(fm::FileSystem::ConvertPathToFileSystem(inPath));
		it != _icons.end())
	{
		return it->second;
	}
	return nullptr;
}

IconManager::IconManager()
{

}

std::shared_ptr<fm::Texture2D> IconManager::LoadIcon(const fm::FilePath& inPath)
{
	if (auto icon = GetIcon(inPath); icon != nullptr)
	{
		return icon;
	}
	std::shared_ptr<fm::Texture2D> icon = fm::Texture2D::CreateTexture2D(GRAPHIC_API::OPENGL, inPath);
	_icons.emplace(fm::FileSystem::ConvertPathToFileSystem(inPath), icon);
	return icon;
}

std::shared_ptr<fm::Texture2D> gui::LoadIconEditor(const std::string& inRelativePath)
{
	return gui::IconManager::Get().LoadIcon(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "editor/images/" + inRelativePath));

}

std::shared_ptr<fm::Texture2D> gui::LoadIconUser(const std::string& inRelativePath)
{
	return gui::IconManager::Get().LoadIcon(fm::FilePath(fm::LOCATION::USER_LOCATION, inRelativePath));
}