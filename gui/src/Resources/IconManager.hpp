#pragma once
#include <memory>

namespace fm
{
	class Texture2D;
	class FilePath;
}

namespace gui
{

	class IconManager
	{
	public:
		IconManager(const IconManager&) = delete;
		IconManager& operator=(const IconManager&) = delete;
		IconManager(IconManager&&) = delete;
		IconManager& operator=(IconManager&&) = delete;

		static auto& Get() {
			static IconManager icons;
			return icons;
		}
	public:
		std::shared_ptr<fm::Texture2D> GetIcon(const fm::FilePath& inPath) const;
		std::shared_ptr<fm::Texture2D> LoadIcon(const fm::FilePath& inPath);

	private:
		IconManager(); // Disallow instantiation outside of the class.

		std::unordered_map<std::string, std::shared_ptr<fm::Texture2D>> _icons;
	};



}

namespace gui
{
	std::shared_ptr<fm::Texture2D> LoadIconEditor(const std::string& inRelativePath);
	std::shared_ptr<fm::Texture2D> LoadIconUser(const std::string& inRelativePath);
}
