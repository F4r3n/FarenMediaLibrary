#include "Texture.h"
#include "nlohmann/json.hpp"
using namespace fm;

Texture::Texture(const fm::FilePath& inPath, const std::string& inName)
	: _path(inPath)
{

}

void to_json(nlohmann::json& j, const Texture& p) {
	j["path"] = fm::FileSystem::ConvertPathToFileSystem(p._path);
}

void from_json(const nlohmann::json& j, Texture& p) {
	std::string path = j["path"];
	p._path = fm::FilePath(path);
}
