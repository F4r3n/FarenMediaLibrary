#pragma once
#include <string>
#include <functional>
namespace fm
{
	class FilePath
	{
	public:
		FilePath() {}
		~FilePath() {}
		FilePath(const std::string &inPath);
		FilePath(const FilePath &inPath);

		bool IsFolder() const;
		bool Exist() const;
		bool IsFile() const;
		void Append(const std::string &inFolderName);
		FilePath GetParent() const;
		const std::string& GetPath() const;
		std::string GetName(bool withoutExtension) const;
		std::string GetExtension() const;
		void CreateFile();
		void CreateFolder();

		static FilePath GetAbsolutePath(const std::string &inPath);

		static FilePath Rename(const FilePath &currentPath, const std::string &inNewName);
		static FilePath GetWorkingDirectory();
		static void		GetAllFiles(const FilePath &inDirectory, const std::string &inExtension, bool recursive, std::vector<FilePath> &outPaths);

		void Iterate(bool recursive, std::function<void(const fm::FilePath& inFilePath)> && inCallback) const;
		bool IsValid() const;
		static char GetFolderSeparator();

		static bool GetRelativeFromRoot(const fm::FilePath &inRoot, const fm::FilePath &otherPath, std::string &outRelativePath);
	private:
		void _Init();
		bool _IsFolder(const std::string &inPath) const;

		std::string _path;
		bool _isFolder;
		bool _isFile;
		bool _isInit;
	};
}

