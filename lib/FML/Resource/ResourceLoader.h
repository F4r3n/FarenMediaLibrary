#pragma once
#include <vector>
#include <memory>

namespace fm
{
	class Resource;
	class FilePath;
	class ResourceImporter;
	class ResourceLoader
	{
	public:
		void Init();

		std::shared_ptr<Resource> Load(const fm::FilePath& inPath, bool inRegister);
		std::shared_ptr<Resource> SaveImport(const fm::FilePath& inPath, bool inForce);
		~ResourceLoader();
	private:
		std::vector<std::shared_ptr<ResourceImporter>> _loaders;
	};
}