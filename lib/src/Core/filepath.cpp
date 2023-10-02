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
	if (inString.empty()) return std::string::npos;

	size_t i = inString.size() - offset - 1;

	while (i > 0 && inString[i] != toFind)
	{
		i--;
	}
	return i == 0 && inString[i] != toFind ? std::string::npos : i;
}

FilePath::FilePath(const std::string& inPath)
{
	std::string path;
	fm::FileSystem::ConvertFileSystemToPath(inPath, path, _fileSystemID);
	_path = std::filesystem::path(path);
}


FilePath::FilePath(const fs::path& inPath)
{
	std::string path;
	fm::FileSystem::ConvertFileSystemToPath(inPath.string(), path, _fileSystemID);
	_path = std::filesystem::path(path);
}


FilePath::FilePath(fm::LOCATION inLocation, const std::string& inFollowingPath)
{
	_path = fm::FileSystem::ConvertFileSystemToPath(inLocation, inFollowingPath);
	_fileSystemID = inLocation;
	_path = _path.native();
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

	return _path.string();
}


char FilePath::GetFolderSeparator()
{
	return fs::path::preferred_separator;
}

bool FilePath::GetRelativeFromRoot(const fm::FilePath &inRoot, const fm::FilePath &other, std::string &outRelativePath)
{
	bool isRoot = false;
	std::string rootPath = inRoot.GetPathString();
	const std::string otherPath = other.GetPath().string();
	if (rootPath.size() < otherPath.size())
	{
		isRoot = (memcmp(rootPath.c_str(), otherPath.c_str(), rootPath.size()) == 0);

		if (isRoot)
		{
			rootPath += fm::FilePath::GetFolderSeparator();
			outRelativePath.clear();
			outRelativePath.append(otherPath.c_str(), rootPath.size(), (otherPath.size() - rootPath.size()));
		}
	}

	return isRoot;
}


FilePath& FilePath::ToSub(const std::string& inFolderName)
{
	if (inFolderName.empty())
		return *this;

	_path += GetFolderSeparator() + inFolderName;
	return *this;
}


const fs::path& FilePath::GetPath() const
{
	return _path;
}

std::string	FilePath::GetPathString() const
{
	return _path.string();
}

void FilePath::AddExtension(const std::string& inExtention)
{
	auto extension = _path.replace_extension();
	extension += inExtention;
}


std::string FilePath::GetName(bool withoutExtension) const
{
	std::string name = _GetName();

	if (withoutExtension)
	{
		const size_t index = name.rfind('.', name.size());
		if(index != std::string::npos)
			name = name.substr(0, index);
	}

	return name;
}

std::string FilePath::_GetName() const
{
	return _path.filename().string();
}


std::string FilePath::GetExtension() const
{
	return _path.extension().string();
}


FilePath FilePath::GetParent() const
{
	return fm::FilePath(_path.parent_path());
}

FilePath FilePath::GetWorkingDirectory()
{
	return FilePath(fs::current_path());
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
}

const fm::FilePath& Folder::GetPath() const
{
	return _path;
}

std::string	Folder::GetPathString() const
{
	return _path.GetPathString();
}

bool Folder::CreateFolder() const
{
	return fs::create_directory(_path.GetPath());
}


Folder Folder::Rename(const std::string& inNewName) const
{
	FilePath p(_path.GetPath() / inNewName);
	fs::rename(_path.GetPath(), p.GetPath());

	return Folder(p);
}


bool Folder::Exist() const
{
	return _path.IsValid() && fs::is_directory(_path.GetPath()) && fs::exists(_path.GetPath());
}


void Folder::Iterate(bool recursive, const std::function<void(const fm::Folder * inFolder, const fm::File * inFile)>& inCallback) const
{
	for (const auto& file : fs::directory_iterator(_path.GetPath(), fs::directory_options::none))
	{
		fm::FilePath currentPath(file.path());
		currentPath.SetSystemID(_path.GetFileSystemID());
		if (file.is_directory())
		{
			Folder f(currentPath);
			inCallback(&f, nullptr);
		}
		else if (file.is_regular_file())
		{
			File f(currentPath);
			inCallback(nullptr, &f);
		}
		

		if (recursive && file.is_directory())
		{
			Folder(currentPath).Iterate(recursive, inCallback);
		}
	}
}



File::File(const fm::FilePath& inFilePath)
{
	_path = inFilePath;
}

File::File(const fm::Folder& inRoot, const std::string& inRelativePath)
{
	_path = inRoot.GetPath();
	_path.ToSub(inRelativePath);
}



const fm::FilePath& File::GetPath() const
{
	return _path;
}

std::string	File::GetPathString() const
{
	return _path.GetPathString();
}


bool File::CreateFile() const
{
	std::ofstream ofs(_path.GetPath());
	ofs << "";
	ofs.close();
	return Exist();
}

std::filesystem::file_time_type File::GetTimeStamp() const
{
	return std::filesystem::last_write_time(_path.GetPath());
}



File File::Rename(const std::string& inNewName) const
{
	FilePath p(_path.GetParent().GetPath() / inNewName);
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
			path.ToSub(name + extension);
		}
		else
		{
			path.ToSub(name + "_" + std::to_string(i) + extension);
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

void File::SetContent(const std::string& inContent) const
{
	std::ofstream myfile;
	myfile.open(_path.GetPathString());
	myfile << inContent;
	myfile.close();
}


std::optional<std::vector<char>> File::GetBinaryContent() const
{
	std::ifstream file(_path.GetPath(), std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		return std::nullopt;
	}
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;

}


void FileSystem::ConvertFileSystemToPath(const std::string& inPath, std::string& outPath, fm::LOCATION &outID)
{
	if (!inPath.empty() && inPath.front() == '!')
	{
		const size_t index = inPath.find_first_of('/');
		const std::string number = inPath.substr(1, index - 1);
		outID = (LOCATION)std::stoi(number);

		outPath = ResourcesManager::get().GetFilePathResource(outID).GetPath().string();
		outPath += fm::FilePath::GetFolderSeparator();
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

	newPath.ToSub(inRelativePath);
	

	return newPath.GetPath().string();
}



std::string FileSystem::ConvertPathToFileSystem(const fm::FilePath& inPath)
{
	std::string path("!" + std::to_string((size_t)inPath.GetFileSystemID()) + "/");
	path += GetRelativePathOfFileSystemPath(inPath);

	return path;
}

std::string FileSystem::GetRelativePathOfFileSystemPath(const fm::FilePath& inPath)
{
	fm::FilePath pathFromID = ResourcesManager::get().GetFilePathResource(inPath.GetFileSystemID());

	std::string relative;
	fm::FilePath::GetRelativeFromRoot(pathFromID, inPath, relative);
	std::replace(relative.begin(), relative.end(), fm::FilePath::GetFolderSeparator(), '/');

	return relative;
}
