#include "Core/FilePath.h"
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;
using namespace fm;
FilePath::FilePath(const std::string &inPath)
{
	_path = inPath;
	_isInit = false;
	_isFolder = IsFolder();
	_isFile = IsFile();
	_isInit = true;
}

constexpr char FilePath::GetFolderSeparator()
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



FilePath::FilePath(const FilePath &inPath)
{
	_path = inPath._path;
}


bool FilePath::IsFolder() const
{
	if (_isInit)
	{
		return _isFolder;
	}
	return _IsFolder(_path);
}

bool FilePath::IsFile() const
{
	if (_isInit)
	{
		return _isFile;
	}
	return fs::is_regular_file(_path);
}

bool FilePath::_IsFolder(const std::string &inPath) const
{
	return fs::is_directory(inPath);
}


void FilePath::Append(const std::string &inFolderName)
{
	std::string newPath = "";

	newPath = _path + GetFolderSeparator() + inFolderName;

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
	return fs::exists(_path);
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

std::string FilePath::GetName(bool withoutExtension) const
{
	fs::path p(_path);
	if (p.has_filename())
	{
		if (withoutExtension)
		{
			return p.filename().stem().u8string();
		}
		else 
			return p.filename().u8string();
	}

	return "";
}

std::string FilePath::GetExtension() const
{
	fs::path p(_path);
	if (p.has_filename())
	{
		return p.extension().u8string();
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

	p = FilePath(currentPath.GetParent().GetPath() + GetFolderSeparator() + inNewName);
	fs::rename(currentPath.GetPath(), p.GetPath());
	
	return p;
}

FilePath FilePath::GetWorkingDirectory()
{
	return FilePath(fs::current_path().u8string());
}

bool FilePath::IsValid() const
{
	return !_path.empty();
}

void FilePath::Iterate(bool recursive, std::function<void(const fm::FilePath& inFilePath)> && inCallback) const
{
	for (auto &file : fs::directory_iterator(_path, fs::directory_options::none))
	{
		fm::FilePath currentPath(file.path().u8string());
		inCallback(currentPath);

		if (recursive && file.is_directory())
		{
			currentPath.Iterate(recursive, std::move(inCallback));
		}
	}
}


void FilePath::GetAllFiles(const FilePath &directory, const std::string &inExtension, bool recursive, std::vector<FilePath> &outPaths)
{
	for (auto &file : fs::directory_iterator(directory.GetPath(), fs::directory_options::skip_permission_denied))
	{

		if (file.is_regular_file())
		{
			if (!inExtension.empty())
			{
				fs::path p(file);
				if (p.extension() == inExtension)
				{
					outPaths.push_back(FilePath(p.u8string()));
				}
			}
			else
			{
				outPaths.push_back(FilePath(fs::path(file).u8string()));
			}
		}
		else if(recursive && file.is_directory())
		{
			GetAllFiles(FilePath(fs::path(file).u8string()), inExtension, recursive, outPaths);
		}

	}
}





