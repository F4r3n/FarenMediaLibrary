#pragma once
#include <memory>
#include <vector>
#include <string>
namespace fm
{
	class FilePath;
	class Resource;

	class ResourceImporter
	{
	public:
		virtual std::shared_ptr<Resource> Load(const fm::FilePath& inPath, bool inRegister) = 0;
		virtual std::shared_ptr<Resource> SaveImport(const fm::FilePath& inPath, bool inForce) = 0;
		virtual ~ResourceImporter() = default;
	};

	template <typename T>
	class ResourceObjectImporter : public ResourceImporter
	{
	public:
		ResourceObjectImporter(const std::vector<std::string>& inListExtensions, bool isImportNeeded);

		std::shared_ptr<Resource> Load(const fm::FilePath& inPath, bool inRegister);
		std::shared_ptr<Resource> SaveImport(const fm::FilePath& inPath, bool inForce);
		virtual ~ResourceObjectImporter() { ; }
	private:
		bool IsValidFileExtension(const std::string& inExtension);
		std::vector<std::string> _listExtensions;
		bool					 _isImportNeeded = true;
	};
}