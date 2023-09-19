#pragma once
#include <vector>
#include <memory>
#include "ResourceImporter.h"
namespace fm
{
	class Resource;
	class FilePath;
	class ResourceLoader
	{
	public:
		void Init();

		std::shared_ptr<Resource> Load(const fm::FilePath& inPath, bool inRegister);
		std::shared_ptr<Resource> SaveImport(const fm::FilePath& inPath, bool inForce);
	private:
		std::vector<std::unique_ptr<ResourceImporter>> _loaders;
	};
}