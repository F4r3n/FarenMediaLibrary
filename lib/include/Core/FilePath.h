#pragma once
#include <string>
#include <vector>
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
		void CreateFile();
		void CreateFolder();

		static FilePath GetAbsolutePath(const std::string &inPath);

		static FilePath Rename(const FilePath &currentPath, const std::string &inNewName);
		static FilePath GetWorkingDirectory();
		static void		GetAllFiles(const FilePath &inDirectory, const std::string &inExtension, bool recursive, std::vector<FilePath> &outPaths);
	private:
		bool _IsFolder(const std::string &inPath) const;

		std::string _path;
	};
}

