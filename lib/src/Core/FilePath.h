#pragma once
#include <string>
#include <functional>
#include "Resource/FileSystem.h"
#include <filesystem>
namespace fm
{
	class FilePath
	{
	public:
		FilePath() {}
		~FilePath() {}
		FilePath(const std::string &inPath);
		FilePath(const FilePath &inPath);
		FilePath(fm::LOCATION inLocation, const std::string& inFollowingPath);

		bool IsFolder() const;
		bool IsFile() const;
		FilePath& ToSubFolder(const std::string &inFolderName);
		FilePath& ToSubFile(const std::string& inFolderName);

		FilePath GetParent() const;
		const std::string& GetPath() const;
		std::string GetName(bool withoutExtension) const;
		std::string GetExtension() const;

		static FilePath GetWorkingDirectory();

		bool IsValid() const;
		static char GetFolderSeparator();

		static bool GetRelativeFromRoot(const fm::FilePath &inRoot, const fm::FilePath &otherPath, std::string &outRelativePath);
		bool operator==(const fm::FilePath& Other) const;
		std::string GetFileSystemPath() const;
		fm::LOCATION GetFileSystemID() const{ return _fileSystemID; }
		void SetSystemID(fm::LOCATION inSystemID) { _fileSystemID = inSystemID; }
	private:
		std::string		_GetName() const;

	private:
		std::string		_path = "";
		fm::LOCATION	_fileSystemID = fm::LOCATION::NONE;
	};

	class Folder;
	class File
	{
	public:
		File(const fm::FilePath& inFilePath);
		File(const fm::Folder& inRoot, const std::string& inRelativePath);
		File() {}
		
		const fm::FilePath& GetPath() const;
		bool CreateFile() const;
		File Rename(const std::string& inNewName) const;
		bool Exist() const;
		std::string GetContent() const;
		File CreateUniqueFile();
		std::filesystem::file_time_type GetTimeStamp() const;
	private:
		fm::FilePath _path;
	};

	class Folder
	{
	public:
		Folder(const fm::FilePath& inFilePath);
		Folder() {}
		const fm::FilePath& GetPath() const;
		bool CreateFolder() const;
		Folder Rename(const std::string& inNewName) const;
		bool Exist() const;
		void Iterate(bool recursive, std::function<void(const fm::Folder *inFolder, const fm::File *inFile)>&& inCallback) const;

	private:
		fm::FilePath _path;
	};

	class FileSystem
	{
	public:
		static void ConvertFileSystemToPath(const std::string& inPath, std::string &outPath, fm::LOCATION &outID);
		static std::string ConvertPathToFileSystem(const fm::FilePath& inPath);
		static std::string ConvertFileSystemToPath(fm::LOCATION inLocation, const std::string &inRelativePath);
	};
}

