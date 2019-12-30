#pragma once
#include <string>
#include <functional>
#include "Resource/FileSystem.h"
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
		void ToSubFolder(const std::string &inFolderName);
		void ToSubFile(const std::string& inFolderName);

		FilePath GetParent() const;
		const std::string& GetPath() const;
		std::string GetName(bool withoutExtension) const;
		std::string GetExtension() const;

		//static FilePath GetAbsolutePath(const std::string &inPath);

		static FilePath GetWorkingDirectory();
		//static FilePath	CreateUniqueFile(const FilePath& inDirectory, const std::string& inName, const std::string& inExtension);

		bool IsValid() const;
		static char GetFolderSeparator();

		static bool GetRelativeFromRoot(const fm::FilePath &inRoot, const fm::FilePath &otherPath, std::string &outRelativePath);
		bool operator==(const fm::FilePath& Other) const;
	private:
		std::string _GetName() const;
		std::string _path;
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
}

