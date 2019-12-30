#include "Core/FilePath.h"
#include <filesystem>
#include <fstream>
#include <cstring>
#include "Resource/ResourcesManager.h"
#include <cassert>
namespace fs = std::filesystem;
using namespace fm;


FilePath::FilePath(const std::string &inPath)
	:
_path(inPath)
{
	std::replace(_path.begin(), _path.end(), '/', GetFolderSeparator());
}

FilePath::FilePath(fm::LOCATION inLocation, const std::string& inFollowingPath)
{
	_path = "!" + std::to_string((int)inLocation) + "/" + inFollowingPath;
	std::replace(_path.begin(), _path.end(), '/', GetFolderSeparator());
}



FilePath::FilePath(const FilePath& inPath)
	:
	_path(inPath._path)
{
}



char FilePath::GetFolderSeparator()
{
	return fs::path::preferred_separator;
}

bool FilePath::GetRelativeFromRoot(const fm::FilePath &inRoot, const fm::FilePath &other, std::string &outRelativePath)
{
	bool isRoot = false;
	const std::string rootPath = inRoot.GetPath();
	const std::string otherPath = other.GetPath();
	if (rootPath.size() < otherPath.size())
	{
		isRoot = (memcmp(rootPath.c_str(), otherPath.c_str(), rootPath.size()) == 0);

		if (isRoot)
		{
			outRelativePath.clear();
			outRelativePath.append(otherPath.c_str(), rootPath.size() + 1, otherPath.size() - rootPath.size() - 1);
		}
	}

	return isRoot;
}


bool FilePath::IsFolder() const
{
	return IsValid() && _path.back() == GetFolderSeparator();
}

bool FilePath::IsFile() const
{
	return IsValid() && _path.back() != GetFolderSeparator();
}


void FilePath::ToSubFolder(const std::string& inFolderName)
{
	if (IsFolder())
	{
		_path += inFolderName + GetFolderSeparator();
	}
}
void FilePath::ToSubFile(const std::string& inFolderName)
{
	if (IsFolder())
	{
		_path += inFolderName;
	}
}

const std::string& FilePath::GetPath() const
{
	return _path;
}


std::string FilePath::GetName(bool withoutExtension) const
{
	std::string name = _GetName();

	if (withoutExtension)
	{
		size_t index = name.rfind('.', name.size());
		name = name.substr(0, index);
	}

	return name;
}

std::string FilePath::_GetName() const
{
	size_t offset = IsFolder() ? 1 : 0;
	std::string path = _path.substr(0, _path.size() - offset);

	size_t index = path.rfind(GetFolderSeparator());
	std::string name = path.substr(index + 1, path.size() - index - 1);

	return name;
}


std::string FilePath::GetExtension() const
{
	std::string name = _GetName();

	size_t index = name.rfind('.', name.size());
	name = name.substr(index, name.size() - index);

	return name;
}


FilePath FilePath::GetParent() const
{
	const size_t offset = IsFolder() ? 1 : 0;

	size_t index = _path.find_last_of(GetFolderSeparator(), _path.size() - offset);

	std::string s = _path.substr(0, index);
	return fm::FilePath(s);
}

FilePath FilePath::GetWorkingDirectory()
{
	return FilePath(fs::current_path().u8string() + GetFolderSeparator());
}

bool FilePath::IsValid() const
{
	return !_path.empty();
}


bool FilePath::operator==(const fm::FilePath& Other) const
{
	return _path == Other._path;
}


//fm::FilePath FilePath::CreateUniqueFile(const FilePath& inDirectory, const std::string& inName, const std::string& inExtension)
//{
//	fm::FilePath path(inDirectory);
//
//	size_t i = 0;
//	do
//	{
//		path = fm::FilePath(inDirectory);
//		if (i == 0)
//		{
//			path.Append(inName + inExtension);
//		}
//		else
//		{
//			path.Append(inName + "_" + std::to_string(i) + inExtension);
//		}
//		i++;
//	} while (path.Exist());
//	path.CreateFile();
//	return path;
//}

Folder::Folder(const fm::FilePath& inFilePath)
{
	_path = fm::FilePath(fm::FileSystem::ConvertFileSystemToPath(inFilePath.GetPath()));
	assert(_path.IsFolder());
}

const fm::FilePath& Folder::GetPath() const
{
	return _path;
}
bool Folder::CreateFolder() const
{
	return fs::create_directory(_path.GetPath());
}


Folder Folder::Rename(const std::string& inNewName) const
{
	FilePath p(_path.GetParent().GetPath() + fm::FilePath::GetFolderSeparator() + inNewName);
	fs::rename(_path.GetPath(), p.GetPath());

	return Folder(p);
}


bool Folder::Exist() const
{
	return _path.IsValid() && fs::is_directory(_path.GetPath()) && fs::exists(_path.GetPath());
}


void Folder::Iterate(bool recursive, std::function<void(const fm::Folder * inFolder, const fm::File * inFile)>&& inCallback) const
{
	for (const auto& file : fs::directory_iterator(_path.GetPath(), fs::directory_options::none))
	{
		const fm::FilePath currentPath(file.path().u8string());
		if (file.is_directory())
		{
			Folder f(file.path().u8string() + fm::FilePath::GetFolderSeparator());
			inCallback(&f, nullptr);
		}
		else if (file.is_regular_file())
		{
			File f(file.path().u8string());
			inCallback(nullptr, &f);
		}
		

		if (recursive && file.is_directory())
		{
			Folder(currentPath).Iterate(recursive, std::move(inCallback));
		}
	}
}



File::File(const fm::FilePath& inFilePath)
{
	_path = fm::FilePath(fm::FileSystem::ConvertFileSystemToPath(inFilePath.GetPath()));
	assert(_path.IsFile());
}

File::File(const fm::Folder& inRoot, const std::string& inRelativePath)
{
	_path = inRoot.GetPath();
	_path.ToSubFile(inRelativePath);
}



const fm::FilePath& File::GetPath() const
{
	return _path;
}


bool File::CreateFile() const
{
	std::ofstream ofs(_path.GetPath());
	ofs << "";
	ofs.close();
	return Exist();
}


File File::Rename(const std::string& inNewName) const
{
	FilePath p(_path.GetParent().GetPath() + fm::FilePath::GetFolderSeparator() + inNewName);
	fs::rename(_path.GetPath(), p.GetPath());

	return File(p);
}


bool File::Exist() const
{
	return _path.IsValid() && fs::is_regular_file(_path.GetPath()) && fs::exists(_path.GetPath());
}

std::string File::GetContent() const
{
	if (Exist())
	{
		std::ifstream stream(_path.GetPath());
		std::string s((std::istreambuf_iterator<char>(stream)),
			std::istreambuf_iterator<char>());
		return s;
	}

	return "";
}
