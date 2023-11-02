#pragma once
#include <string>
#include <functional>
#include "Resource/FileSystem.h"
#include <filesystem>
#include <optional>
#include <vector>
#include <nlohmann/json_fwd.hpp>
namespace fs = std::filesystem;
namespace fm
{
	class FilePath
	{
	public:
		FilePath() {}
		~FilePath() {}
		FilePath(const fs::path& inPath);
		FilePath(const std::string& inPath);

		FilePath(const FilePath &inPath);
		FilePath(fm::LOCATION inLocation, const std::string& inFollowingPath);
		FilePath(fm::LOCATION inLocation);

		FilePath& ToSub(const std::string &inFolderName);

		FilePath GetParent() const;
		const fs::path& GetPath() const;
		std::string	GetPathString() const;
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
		void AddExtension(const std::string& inExtention);
	private:
		std::string		_GetName() const;

	private:
		fs::path		_path;
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
		std::string			GetPathString() const;
		bool CreateFile() const;
		File Rename(const std::string& inNewName) const;
		File CopyTo(const fm::FilePath& inDestination) const;
		bool Exist() const;
		std::string GetContent() const;
		void		SetContent(const std::string& inContent) const;
		void		SetContent(const std::vector<uint32_t>& inContent) const;
		bool		Delete();
		File		CreateUniqueFile();
		void		GetJSONContent(nlohmann::json& outJSON) const;
		std::filesystem::file_time_type GetTimeStamp() const;
		std::optional<std::vector<char>> GetBinaryContent() const;
	private:
		fm::FilePath _path;
	};

	class Folder
	{
	public:
		Folder(const fm::FilePath& inFilePath);
		Folder() {}
		const fm::FilePath& GetPath() const;
		std::string			GetPathString() const;

		bool CreateFolder() const;
		Folder Rename(const std::string& inNewName) const;
		Folder CopyTo(const fm::FilePath& inDestination) const;

		bool Exist() const;
		void Iterate(bool recursive, const std::function<void(const fm::Folder *inFolder, const fm::File *inFile)>& inCallback) const;
		bool Delete(bool recursive);
		Folder		CreateUniqueFolder();

	private:
		fm::FilePath _path;
	};

	class FileSystem
	{
	public:
		static void ConvertFileSystemToPath(const std::string& inPath, std::string &outPath, fm::LOCATION &outID);
		static std::string ConvertPathToFileSystem(const fm::FilePath& inPath);
		static std::string ConvertFileSystemToPath(fm::LOCATION inLocation, const std::string &inRelativePath);
		static std::string GetRelativePathOfFileSystemPath(const fm::FilePath& inPath);
	};
}

