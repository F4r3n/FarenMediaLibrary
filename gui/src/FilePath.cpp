#include "FilePath.h"
#include <filesystem>
#include <fstream>
#if __linux__
const char FOLDER_SEPARATOR = '/';
#elif __APPLE__
const char FOLDER_SEPARATOR = ':';
#else
const char FOLDER_SEPARATOR = '\\';
#endif

namespace fs = std::filesystem;

FilePath::FilePath(const std::string &inPath)
{
	_path = inPath;
}

FilePath::FilePath(const FilePath &inPath)
{
	_path = inPath._path;
}


bool FilePath::IsFolder() const
{
	return _IsFolder(_path);
}

bool FilePath::IsFile() const
{
	return !fs::is_regular_file(_path);
}

bool FilePath::_IsFolder(const std::string &inPath) const
{
	return fs::is_directory(inPath);
}


void FilePath::Append(const std::string &inFolderName)
{
	std::string newPath = "";

	newPath = _path + FOLDER_SEPARATOR + inFolderName;

	_path = newPath;
}

void FilePath::CreateFile()
{
	std::ofstream ofs(_path);
	ofs << "";
	ofs.close();
}

void FilePath::CreateFolder()
{
	fs::create_directory(_path);
}


bool FilePath::Exist() const
{
	return IsFolder() || IsFile();
}


const std::string& FilePath::GetPath() const
{
	return _path;
}

FilePath FilePath::GetAbsolutePath(const std::string &inPath)
{
	std::string path = fs::absolute(inPath).u8string();
	FilePath p(path);
	if (fs::is_directory(path))
	{
		p = FilePath(path);
	}
	return p;
}

std::string FilePath::GetName() const
{
	fs::path p(_path);
	if (p.has_filename())
	{
		return p.filename().u8string();
	}

	return "";
}



FilePath FilePath::GetParent() const
{
	return FilePath(fs::path(_path).parent_path().u8string());
}

FilePath FilePath::Rename(const FilePath &currentPath, const std::string &inNewName)
{
	FilePath p(currentPath.GetPath());

	p = FilePath(currentPath.GetParent().GetPath() +FOLDER_SEPARATOR + inNewName);
	fs::rename(currentPath.GetPath(), p.GetPath());
	
	return p;
}



