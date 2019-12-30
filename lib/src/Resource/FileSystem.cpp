#include "Resource/FileSystem.h"
#include "Core/FilePath.h"
#include "Resource/ResourcesManager.h"
using namespace fm;
std::string FileSystem::ConvertFileSystemToPath(const std::string& inPath)
{
	std::string path(inPath);

	if (inPath.front() == '!')
	{
		size_t index = inPath.find_first_of(fm::FilePath::GetFolderSeparator());
		std::string number = inPath.substr(1, index - 1);
		LOCATION i = (LOCATION)std::stoi(number);

		fm::FilePath newPath = ResourcesManager::get().GetFilePathResource(i);
		if (fm::FilePath(inPath).IsFolder())
		{
			newPath.ToSubFolder(inPath.substr(index, inPath.size() - index));
		}
		else
		{
			newPath.ToSubFile(inPath.substr(index, inPath.size() - index));
		}

		path = newPath.GetPath();
	}

	return path;
}
