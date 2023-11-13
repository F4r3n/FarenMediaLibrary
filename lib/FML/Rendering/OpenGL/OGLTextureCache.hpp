#pragma once
#include <memory>
#include <unordered_map>
namespace fm
{
	class OGLTexture;
	class Texture;
	struct TextureCache
	{
		std::shared_ptr<fm::OGLTexture>	_blankTexture;
		std::unordered_map<uint32_t, std::shared_ptr<fm::OGLTexture>> _textures;
		std::shared_ptr<fm::OGLTexture> FindOrCreateTexture(std::shared_ptr<fm::Texture> inTexture);
		std::shared_ptr<fm::OGLTexture> GetBlank() const { return _blankTexture; }
	};
}
