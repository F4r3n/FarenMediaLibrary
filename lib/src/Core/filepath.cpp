#include "Core/FilePath.h"
#include <filesystem>
#include <fstream>
#include <cstring>
#include "Resource/ResourcesManager.h"
#include <cassert>
namespace fs = std::filesystem;
using namespace fm;

size_t FindFromEnd(const std::string& inString, char toFind, size_t offset)
{
	size_t i = inString.size() - offset - 1;
	while (i > 0 && inString[i] != toFind)
	{
		i--;
	}
	return i;
}


FilePath::FilePath(const std::string &inPath)
{
	fm::FileSystem::ConvertFileSystemToPath(inPath, _path, _fileSystemID);
}

FilePath::FilePath(fm::LOCATION inLocation, const std::string& inFollowingPath)
{
	_path = fm::FileSystem::ConvertFileSystemToPath(inLocation, inFollowingPath);
	_fileSystemID = inLocation;
	std::replace(_path.begin(), _path.end(), '/', GetFolderSeparator());
}


FilePath::FilePath(const FilePath& inPath)
	:
	_path(inPath._path),
	_fileSystemID(inPath._fileSystemID)
{
}


std::string FilePath::GetFileSystemPath() const
{
	if (_fileSystemID != fm::LOCATION::NONE)
	{
		return fm::FileSystem::ConvertPathToFileSystem(*this);
	}

	return _path;
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
			outRelativePath.append(otherPath.c_str(), rootPath.size(), otherPath.size() - rootPath.size());
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
	_path += inFolderName + GetFolderSeparator();
}
void FilePath::ToSubFile(const std::string& inFolderName)
{
	_path += inFolderName;
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
	size_t index = FindFromEnd(_path, GetFolderSeparator(), offset);
	std::string name = _path.substr(index + 1, _path.size() - index - offset - 1);

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
	assert(!_path.empty());

	size_t index = _path.rfind(GetFolderSeparator(), _path.size() - 1);

	std::string s = _path.substr(0, index);
	s += GetFolderSeparator();
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

Folder::Folder(const fm::FilePath& inFilePath)
{
	_path = inFilePath;
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
	_path = inFilePath;
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

std::time_t File::GetTimeStamp() const
{
#if defined ( _WIN32 )
	{
		struct _stat64 fileInfo;
		if (_wstati64(fs::path(_path.GetPath()).wstring().c_str(), &fileInfo) != 0)
		{
			throw std::runtime_error("Failed to get last write time.");
		}
		return fileInfo.st_mtime;
	}
#else
	{
		auto fsTime = std::filesystem::last_write_time(_path.GetPath());
		return decltype (fsTime)::clock::to_time_t(fsTime);
	}
#endif
}



File File::Rename(const std::string& inNewName) const
{
	FilePath p(_path.GetParent().GetPath() + fm::FilePath::GetFolderSeparator() + inNewName);
	fs::rename(_path.GetPath(), p.GetPath());

	return File(p);
}

File File::CreateUniqueFile()
{

	const fm::FilePath parent = _path.GetParent();
	fm::FilePath path(parent);

	const std::string name = _path.GetName(true);
	const std::string extension = _path.GetExtension();

	size_t i = 0;
	do
	{
		path = fm::FilePath(parent);
		if (i == 0)
		{
			path.ToSubFile(name + extension);
		}
		else
		{
			path.ToSubFile(name + "_" + std::to_string(i) + extension);
		}
		i++;
	} while (File(path).Exist());
	File(path).CreateFile();
	return File(path);

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


void FileSystem::ConvertFileSystemToPath(const std::string& inPath, std::string& outPath, fm::LOCATION &outID)
{
	if (!inPath.empty() && inPath.front() == '!')
	{
		const size_t index = inPath.find_first_of('/');
		const std::string number = inPath.substr(1, index - 1);
		outID = (LOCATION)std::stoi(number);

		outPath = ResourcesManager::get().GetFilePathResource(outID).GetPath();
		outPath += inPath.substr(index + 1, inPath.size() - index - 1);
		
		std::replace(outPath.begin(), outPath.end(), '/', fm::FilePath::GetFolderSeparator());

	}
	else
	{
		outPath = inPath;
	}
}

std::string FileSystem::ConvertFileSystemToPath(fm::LOCATION inLocation, const std::string& inRelativePath)
{
	fm::FilePath newPath(ResourcesManager::get().GetFilePathResource(inLocation).GetPath());
	newPath.SetSystemID(inLocation);
	if (fm::FilePath(inRelativePath).IsFolder())
	{
		newPath.ToSubFolder(inRelativePath);
	}
	else
	{
		newPath.ToSubFile(inRelativePath);
	}

	return newPath.GetPath();
}



std::string FileSystem::ConvertPathToFileSystem(const fm::FilePath& inPath)
{
	std::string path;
	path += "!" + std::to_string((size_t)inPath.GetFileSystemID()) + "/";
	fm::FilePath pathFromID = ResourcesManager::get().GetFilePathResource(inPath.GetFileSystemID());

	std::string relative;
	fm::FilePath::GetRelativeFromRoot(pathFromID, inPath, relative);
	std::replace(relative.begin(), relative.end(), fm::FilePath::GetFolderSeparator(), '/');

	path += relative;

	return path;
}

